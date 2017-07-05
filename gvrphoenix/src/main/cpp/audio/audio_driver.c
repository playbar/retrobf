/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include <retro_assert.h>

#include <lists/string_list.h>
#include <audio/conversion/float_to_s16.h>
#include <audio/conversion/s16_to_float.h>
#include <audio/audio_resampler.h>
#include <audio/dsp_filter.h>
#include <file/file_path.h>
#include <lists/dir_list.h>
#include <string/stdstring.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "audio_driver.h"
#include "audio_thread_wrapper.h"
#include "../gfx/video_driver.h"
#include "../record/record_driver.h"
#include "../frontend/frontend_driver.h"

#include "../src/command.h"
#include "../src/driver.h"
#include "../src/configuration.h"
#include "../src/retroarch.h"
#include "../src/verbosity.h"
#include "../src/list_special.h"

#define AUDIO_BUFFER_FREE_SAMPLES_COUNT (8 * 1024)

#define AUDIO_MIXER_MAX_STREAMS 8

static const audio_driver_t *audio_drivers[] = {
   &audio_opensl,
};

struct audio_mixer_stream
{
   audio_mixer_sound_t *handle;
   audio_mixer_voice_t *voice;
   audio_mixer_stop_cb_t stop_cb;
   enum audio_mixer_state state;
   float volume;
   void *buf;
   size_t bufsize;
};

static unsigned audio_mixer_current_max_idx              = 0;
static struct audio_mixer_stream audio_mixer_streams[AUDIO_MIXER_MAX_STREAMS] = {{0}};

static size_t audio_driver_chunk_size                    = 0;
static size_t audio_driver_chunk_nonblock_size           = 0;
static size_t audio_driver_chunk_block_size              = 0;

static size_t audio_driver_rewind_ptr                    = 0;
static size_t audio_driver_rewind_size                   = 0;

static int16_t *audio_driver_rewind_buf                  = NULL;
static int16_t *audio_driver_output_samples_conv_buf     = NULL;

static unsigned audio_driver_free_samples_buf[AUDIO_BUFFER_FREE_SAMPLES_COUNT];
static uint64_t audio_driver_free_samples_count          = 0;

static size_t audio_driver_buffer_size                   = 0;
static size_t audio_driver_data_ptr                      = 0;

static bool audio_driver_control                         = false;
static bool audio_driver_mute_enable                     = false;
static bool audio_driver_use_float                       = false;
static bool audio_driver_active                          = false;
static bool audio_driver_data_own                        = false;
static bool audio_mixer_active                           = false;

static float audio_driver_rate_control_delta             = 0.0f;
static float audio_driver_input                          = 0.0f;
static float audio_driver_volume_gain                    = 0.0f;

static float *audio_driver_input_data                    = NULL;
static float *audio_driver_output_samples_buf            = NULL;

static double audio_source_ratio_original                = 0.0f;
static double audio_source_ratio_current                 = 0.0f;

static struct retro_audio_callback audio_callback        = {0};

static retro_dsp_filter_t *audio_driver_dsp              = NULL;
static struct string_list *audio_driver_devices_list     = NULL;
static const retro_resampler_t *audio_driver_resampler   = NULL;

static void *audio_driver_resampler_data                 = NULL;
static const audio_driver_t *current_audio               = NULL;
static void *audio_driver_context_audio_data             = NULL;

/**
 * compute_audio_buffer_statistics:
 *
 * Computes audio buffer statistics.
 *
 **/
static void compute_audio_buffer_statistics(void)
{
   unsigned i, low_water_size, high_water_size, avg, stddev;
   float avg_filled              = 0.0f;
   float deviation               = 0.0f;
   uint64_t accum                = 0;
   uint64_t accum_var            = 0;
   unsigned low_water_count      = 0;
   unsigned high_water_count     = 0;
   unsigned samples              = MIN(
         (unsigned)audio_driver_free_samples_count,
         AUDIO_BUFFER_FREE_SAMPLES_COUNT);

   if (samples < 3)
      return;

   for (i = 1; i < samples; i++)
      accum += audio_driver_free_samples_buf[i];

   avg = (unsigned)accum / (samples - 1);

   for (i = 1; i < samples; i++)
   {
      int diff = avg - audio_driver_free_samples_buf[i];
      accum_var += diff * diff;
   }

   stddev          = (unsigned)sqrt((double)accum_var / (samples - 2));
   avg_filled      = 1.0f - (float)avg / audio_driver_buffer_size;
   deviation       = (float)stddev / audio_driver_buffer_size;

   low_water_size  = (unsigned)(audio_driver_buffer_size * 3 / 4);
   high_water_size = (unsigned)(audio_driver_buffer_size     / 4);

   for (i = 1; i < samples; i++)
   {
      if (audio_driver_free_samples_buf[i] >= low_water_size)
         low_water_count++;
      else if (audio_driver_free_samples_buf[i] <= high_water_size)
         high_water_count++;
   }

   RARCH_LOG("[Audio]: Average audio buffer saturation: %.2f %%, standard deviation (percentage points): %.2f %%.\n",
         avg_filled * 100.0, deviation * 100.0);
   RARCH_LOG("[Audio]: Amount of time spent close to underrun: %.2f %%. Close to blocking: %.2f %%.\n",
         (100.0 * low_water_count) / (samples - 1),
         (100.0 * high_water_count) / (samples - 1));
}

/**
 * audio_driver_find_handle:
 * @idx                : index of driver to get handle to.
 *
 * Returns: handle to audio driver at index. Can be NULL
 * if nothing found.
 **/
const void *audio_driver_find_handle(int idx)
{
   const void *drv = audio_drivers[idx];
   if (!drv)
      return NULL;
   return drv;
}

/**
 * audio_driver_find_ident:
 * @idx                : index of driver to get handle to.
 *
 * Returns: Human-readable identifier of audio driver at index. Can be NULL
 * if nothing found.
 **/
const char *audio_driver_find_ident(int idx)
{
   const audio_driver_t *drv = audio_drivers[idx];
   if (!drv)
      return NULL;
   return drv->ident;
}

/**
 * config_get_audio_driver_options:
 *
 * Get an enumerated list of all audio driver names, separated by '|'.
 *
 * Returns: string listing of all audio driver names, separated by '|'.
 **/
const char *config_get_audio_driver_options(void)
{
   return char_list_new_special(STRING_LIST_AUDIO_DRIVERS, NULL);
}

static bool audio_driver_deinit_internal(void)
{
   settings_t *settings = config_get_ptr();

   if (current_audio && current_audio->free)
   {
      if (audio_driver_context_audio_data)
         current_audio->free(audio_driver_context_audio_data);
      audio_driver_context_audio_data = NULL;
   }

   if (audio_driver_output_samples_conv_buf)
      free(audio_driver_output_samples_conv_buf);
   audio_driver_output_samples_conv_buf = NULL;

   audio_driver_data_ptr                = 0;

   if (audio_driver_rewind_buf)
      free(audio_driver_rewind_buf);
   audio_driver_rewind_buf   = NULL;

   audio_driver_rewind_size  = 0;

   if (settings && !settings->bools.audio_enable)
   {
      audio_driver_active = false;
      return false;
   }

   audio_driver_deinit_resampler();

   if (audio_driver_input_data)
      free(audio_driver_input_data);
   audio_driver_input_data = NULL;

   if (audio_driver_output_samples_buf)
      free(audio_driver_output_samples_buf);
   audio_driver_output_samples_buf = NULL;

   command_event(CMD_EVENT_DSP_FILTER_DEINIT, NULL);

   compute_audio_buffer_statistics();

   return true;
}

static bool audio_driver_init_internal(bool audio_cb_inited)
{
   unsigned new_rate     = 0;
   float   *aud_inp_data = NULL;
   float *samples_buf    = NULL;
   int16_t *conv_buf     = NULL;
   int16_t *rewind_buf   = NULL;
   size_t max_bufsamples = AUDIO_CHUNK_SIZE_NONBLOCKING * 2;
   settings_t *settings  = config_get_ptr();
   /* Accomodate rewind since at some point we might have two full buffers. */
   size_t outsamples_max = AUDIO_CHUNK_SIZE_NONBLOCKING * 2 * AUDIO_MAX_RATIO * settings->floats.slowmotion_ratio;

   convert_s16_to_float_init_simd();
   convert_float_to_s16_init_simd();

   conv_buf = (int16_t*)malloc(outsamples_max * sizeof(int16_t));
   /* Used for recording even if audio isn't enabled. */
   retro_assert(conv_buf != NULL);

   if (!conv_buf)
      goto error;

   audio_driver_output_samples_conv_buf = conv_buf;
   audio_driver_chunk_block_size = AUDIO_CHUNK_SIZE_BLOCKING;
   audio_driver_chunk_nonblock_size = AUDIO_CHUNK_SIZE_NONBLOCKING;
   audio_driver_chunk_size = audio_driver_chunk_block_size;

   /* Needs to be able to hold full content of a full max_bufsamples
    * in addition to its own. */
   rewind_buf = (int16_t*)malloc(max_bufsamples * sizeof(int16_t));
   retro_assert(rewind_buf != NULL);

   if (!rewind_buf)
      goto error;

   audio_driver_rewind_buf              = rewind_buf;
   audio_driver_rewind_size             = max_bufsamples;

   if (!settings->bools.audio_enable)
   {
      audio_driver_active = false;
      return false;
   }

   audio_driver_find_driver();
#ifdef HAVE_THREADS
   if (audio_cb_inited)
   {
      RARCH_LOG("[Audio]: Starting threaded audio driver ...\n");
      if (!audio_init_thread(
               &current_audio,
               &audio_driver_context_audio_data,
               *settings->arrays.audio_device
               ? settings->arrays.audio_device : NULL,
               settings->uints.audio_out_rate, &new_rate,
               settings->uints.audio_latency,
               settings->uints.audio_block_frames,
               current_audio))
      {
         RARCH_ERR("Cannot open threaded audio driver ... Exiting ...\n");
         retroarch_fail(1, "audio_driver_init_internal()");
      }
   }
   else
#endif
   {
      audio_driver_context_audio_data =
         current_audio->init(*settings->arrays.audio_device ?
               settings->arrays.audio_device : NULL,
               settings->uints.audio_out_rate,
               settings->uints.audio_latency,
               settings->uints.audio_block_frames,
               &new_rate);
   }

   if (new_rate != 0)
   {
      configuration_set_int(settings, settings->uints.audio_out_rate, new_rate);
   }

   if (!audio_driver_context_audio_data)
   {
      RARCH_ERR("Failed to initialize audio driver. Will continue without audio.\n");
      audio_driver_active = false;
   }

   audio_driver_use_float = false;
   if (     audio_driver_active
         && current_audio->use_float(audio_driver_context_audio_data))
      audio_driver_use_float = true;

   if (!settings->bools.audio_sync && audio_driver_active)
   {
      command_event(CMD_EVENT_AUDIO_SET_NONBLOCKING_STATE, NULL);
      audio_driver_chunk_size = audio_driver_chunk_nonblock_size;
   }

   if (audio_driver_input <= 0.0f)
   {
      /* Should never happen. */
      RARCH_WARN("Input rate is invalid (%.3f Hz). Using output rate (%u Hz).\n",
            audio_driver_input, settings->uints.audio_out_rate);
      audio_driver_input = settings->uints.audio_out_rate;
   }

   audio_source_ratio_original   = audio_source_ratio_current =
      (double)settings->uints.audio_out_rate / audio_driver_input;

   if (!retro_resampler_realloc(
            &audio_driver_resampler_data,
            &audio_driver_resampler,
            settings->arrays.audio_resampler,
            audio_source_ratio_original))
   {
      RARCH_ERR("Failed to initialize resampler \"%s\".\n",
            settings->arrays.audio_resampler);
      audio_driver_active = false;
   }

   aud_inp_data = (float*)malloc(max_bufsamples * sizeof(float));
   retro_assert(aud_inp_data != NULL);

   if (!aud_inp_data)
      goto error;

   audio_driver_input_data = aud_inp_data;
   audio_driver_data_ptr   = 0;

   retro_assert(settings->uints.audio_out_rate <
         audio_driver_input * AUDIO_MAX_RATIO);

   samples_buf = (float*)malloc(outsamples_max * sizeof(float));

   retro_assert(samples_buf != NULL);

   if (!samples_buf)
      goto error;

   audio_driver_output_samples_buf = samples_buf;
   audio_driver_control            = false;

   if (
         !audio_cb_inited
         && audio_driver_active
         && settings->bools.audio_rate_control
         )
   {
      /* Audio rate control requires write_avail
       * and buffer_size to be implemented. */
      if (current_audio->buffer_size)
      {
         audio_driver_buffer_size =
            current_audio->buffer_size(audio_driver_context_audio_data);
         audio_driver_control = true;
      }
      else
         RARCH_WARN("Audio rate control was desired, but driver does not support needed features.\n");
   }

   command_event(CMD_EVENT_DSP_FILTER_INIT, NULL);

   audio_driver_free_samples_count = 0;

   audio_mixer_init(settings->uints.audio_out_rate);

   /* Threaded driver is initially stopped. */
   if (
         audio_driver_active
         && audio_cb_inited
         )
      audio_driver_start(false);

   return true;

error:
   return audio_driver_deinit();
}

void audio_driver_set_nonblocking_state(bool enable)
{
   settings_t *settings = config_get_ptr();
   if (
         audio_driver_active
         && audio_driver_context_audio_data
      )
      current_audio->set_nonblock_state(
            audio_driver_context_audio_data,
            settings->bools.audio_sync ? enable : true);

   audio_driver_chunk_size = enable ?
      audio_driver_chunk_nonblock_size :
      audio_driver_chunk_block_size;
}

/**
 * audio_driver_flush:
 * @data                 : pointer to audio buffer.
 * @right                : amount of samples to write.
 *
 * Writes audio samples to audio driver. Will first
 * perform DSP processing (if enabled) and resampling.
 *
 * Returns: true (1) if audio samples were written to the audio
 * driver, false (0) in case of an error.
 **/
static bool audio_driver_flush(const int16_t *data, size_t samples)
{
   struct resampler_data src_data;
   bool is_perfcnt_enable                               = false;
   bool is_paused                                       = false;
   bool is_idle                                         = false;
   bool is_slowmotion                                   = false;
   const void *output_data                              = NULL;
   unsigned output_frames                               = 0;

   src_data.data_in                                     = NULL;
   src_data.data_out                                    = NULL;
   src_data.input_frames                                = 0;
   src_data.output_frames                               = 0;
   src_data.ratio                                       = 0.0f;

   if (recording_data)
      recording_push_audio(data, samples);

   runloop_get_status(&is_paused, &is_idle, &is_slowmotion,
         &is_perfcnt_enable);

   if (is_paused || audio_driver_mute_enable)
      return true;
   if (!audio_driver_active || !audio_driver_input_data)
      return false;

   convert_s16_to_float(audio_driver_input_data, data, samples,
         audio_driver_volume_gain);

   src_data.data_in               = audio_driver_input_data;
   src_data.input_frames          = samples >> 1;


   if (audio_driver_dsp)
   {
      struct retro_dsp_data dsp_data;

      dsp_data.input                 = NULL;
      dsp_data.input_frames          = 0;
      dsp_data.output                = NULL;
      dsp_data.output_frames         = 0;

      dsp_data.input                 = audio_driver_input_data;
      dsp_data.input_frames          = (unsigned)(samples >> 1);

      retro_dsp_filter_process(audio_driver_dsp, &dsp_data);

      if (dsp_data.output)
      {
         src_data.data_in            = dsp_data.output;
         src_data.input_frames       = dsp_data.output_frames;
      }
   }

   src_data.data_out = audio_driver_output_samples_buf;

   if (audio_driver_control)
   {
      /* Readjust the audio input rate. */
      unsigned write_idx   = audio_driver_free_samples_count++ &
         (AUDIO_BUFFER_FREE_SAMPLES_COUNT - 1);
      int      half_size   = (int)(audio_driver_buffer_size / 2);
      int      avail       =
         (int)current_audio->write_avail(audio_driver_context_audio_data);
      int      delta_mid   = avail - half_size;
      double   direction   = (double)delta_mid / half_size;
      double   adjust      = 1.0 + audio_driver_rate_control_delta * direction;

#if 0
      RARCH_LOG_OUTPUT("[Audio]: Audio buffer is %u%% full\n",
            (unsigned)(100 - (avail * 100) / audio_driver_buffer_size));
#endif

      audio_driver_free_samples_buf
         [write_idx]               = avail;
      audio_source_ratio_current   =
         audio_source_ratio_original * adjust;

#if 0
      RARCH_LOG_OUTPUT("[Audio]: New rate: %lf, Orig rate: %lf\n",
            audio_source_ratio_current,
            audio_source_ratio_original);
#endif
   }

   src_data.ratio     = audio_source_ratio_current;

   if (is_slowmotion)
   {
      settings_t *settings  = config_get_ptr();
      src_data.ratio       *= settings->floats.slowmotion_ratio;
   }

   audio_driver_resampler->process(audio_driver_resampler_data, &src_data);

   if (audio_mixer_active)
      audio_mixer_mix(audio_driver_output_samples_buf, src_data.output_frames);

   output_data        = audio_driver_output_samples_buf;
   output_frames      = (unsigned)src_data.output_frames;

   if (audio_driver_use_float)
      output_frames  *= sizeof(float);
   else
   {
      convert_float_to_s16(audio_driver_output_samples_conv_buf,
            (const float*)output_data, output_frames * 2);

      output_data     = audio_driver_output_samples_conv_buf;
      output_frames  *= sizeof(int16_t);
   }

   if (current_audio->write(audio_driver_context_audio_data,
            output_data, output_frames * 2) < 0)
   {
      audio_driver_active = false;
      return false;
   }

   return true;
}

/**
 * audio_driver_sample:
 * @left                 : value of the left audio channel.
 * @right                : value of the right audio channel.
 *
 * Audio sample render callback function.
 **/
void audio_driver_sample(int16_t left, int16_t right)
{
   audio_driver_output_samples_conv_buf[audio_driver_data_ptr++] = left;
   audio_driver_output_samples_conv_buf[audio_driver_data_ptr++] = right;

   if (audio_driver_data_ptr < audio_driver_chunk_size)
      return;

   audio_driver_flush(audio_driver_output_samples_conv_buf,
         audio_driver_data_ptr);

   audio_driver_data_ptr = 0;
}

/**
 * audio_driver_sample_batch:
 * @data                 : pointer to audio buffer.
 * @frames               : amount of audio frames to push.
 *
 * Batched audio sample render callback function.
 *
 * Returns: amount of frames sampled. Will be equal to @frames
 * unless @frames exceeds (AUDIO_CHUNK_SIZE_NONBLOCKING / 2).
 **/
extern int gViewType;

size_t audio_driver_sample_batch(const int16_t *data, size_t frames)
{
//    if( gViewType == 1 )
//        return 0;
   if (frames > (AUDIO_CHUNK_SIZE_NONBLOCKING >> 1))
      frames = AUDIO_CHUNK_SIZE_NONBLOCKING >> 1;

   audio_driver_flush(data, frames << 1);

   return frames;
}

/**
 * audio_driver_sample_rewind:
 * @left                 : value of the left audio channel.
 * @right                : value of the right audio channel.
 *
 * Audio sample render callback function (rewind version).
 * This callback function will be used instead of
 * audio_driver_sample when rewinding is activated.
 **/
void audio_driver_sample_rewind(int16_t left, int16_t right)
{
   audio_driver_rewind_buf[--audio_driver_rewind_ptr] = right;
   audio_driver_rewind_buf[--audio_driver_rewind_ptr] = left;
}

/**
 * audio_driver_sample_batch_rewind:
 * @data                 : pointer to audio buffer.
 * @frames               : amount of audio frames to push.
 *
 * Batched audio sample render callback function (rewind version).
 *
 * This callback function will be used instead of
 * audio_driver_sample_batch when rewinding is activated.
 *
 * Returns: amount of frames sampled. Will be equal to @frames
 * unless @frames exceeds (AUDIO_CHUNK_SIZE_NONBLOCKING / 2).
 **/
size_t audio_driver_sample_batch_rewind(const int16_t *data, size_t frames)
{
   size_t i;
   size_t samples   = frames << 1;

   for (i = 0; i < samples; i++)
      audio_driver_rewind_buf[--audio_driver_rewind_ptr] = data[i];

   return frames;
}

void audio_driver_set_volume_gain(float gain)
{
   audio_driver_volume_gain = gain;
}

void audio_driver_dsp_filter_free(void)
{
   if (audio_driver_dsp)
      retro_dsp_filter_free(audio_driver_dsp);
   audio_driver_dsp = NULL;
}

void audio_driver_dsp_filter_init(const char *device)
{
#if defined(HAVE_DYLIB) && !defined(HAVE_FILTERS_BUILTIN)
   char basedir[PATH_MAX_LENGTH];
   char ext_name[PATH_MAX_LENGTH];
#endif
   struct string_list *plugs     = NULL;
#if defined(HAVE_DYLIB) && !defined(HAVE_FILTERS_BUILTIN)
   fill_pathname_basedir(basedir, device, sizeof(basedir));

   if (!frontend_driver_get_core_extension(ext_name, sizeof(ext_name)))
      goto error;

   plugs = dir_list_new(basedir, ext_name, false, true, false, false);
   if (!plugs)
      goto error;
#endif
   audio_driver_dsp = retro_dsp_filter_new(
         device, plugs, audio_driver_input);
   if (!audio_driver_dsp)
      goto error;

   return;

error:
   if (!audio_driver_dsp)
      RARCH_ERR("[DSP]: Failed to initialize DSP filter \"%s\".\n", device);
}

void audio_driver_set_buffer_size(size_t bufsize)
{
   audio_driver_buffer_size = bufsize;
}

void audio_driver_monitor_adjust_system_rates(void)
{
   float timing_skew;
   settings_t                   *settings = config_get_ptr();
   struct retro_system_av_info   *av_info = video_viewport_get_system_av_info();
   float video_refresh_rate               = settings->floats.video_refresh_rate;
   float max_timing_skew                  = settings->floats.audio_max_timing_skew;
   const struct retro_system_timing *info = av_info ?
      (const struct retro_system_timing*)&av_info->timing : NULL;

   if (!info || info->sample_rate <= 0.0)
      return;

   timing_skew             = fabs(1.0f - info->fps / video_refresh_rate);
   audio_driver_input      = info->sample_rate;

   if (timing_skew <= max_timing_skew)
      audio_driver_input *= (video_refresh_rate / info->fps);

   RARCH_LOG("[Audio]: Set audio input rate to: %.2f Hz.\n",
         audio_driver_input);
}

void audio_driver_setup_rewind(void)
{
   unsigned i;

   /* Push audio ready to be played. */
   audio_driver_rewind_ptr = audio_driver_rewind_size;

   for (i = 0; i < audio_driver_data_ptr; i += 2)
   {
      audio_driver_rewind_buf[--audio_driver_rewind_ptr] =
         audio_driver_output_samples_conv_buf[i + 1];

      audio_driver_rewind_buf[--audio_driver_rewind_ptr] =
         audio_driver_output_samples_conv_buf[i + 0];
   }

   audio_driver_data_ptr = 0;
}

bool audio_driver_find_driver(void)
{
   int i;
   driver_ctx_info_t drv;
   settings_t *settings = config_get_ptr();

   drv.label = "audio_driver";
   drv.s     = settings->arrays.audio_driver;

   driver_ctl(RARCH_DRIVER_CTL_FIND_INDEX, &drv);

   i = (int)drv.len;

   if (i >= 0)
      current_audio = (const audio_driver_t*)audio_driver_find_handle(i);
   else
   {
      unsigned d;
      RARCH_ERR("Couldn't find any audio driver named \"%s\"\n",
            settings->arrays.audio_driver);
      RARCH_LOG_OUTPUT("Available audio drivers are:\n");
      for (d = 0; audio_driver_find_handle(d); d++)
         RARCH_LOG_OUTPUT("\t%s\n", audio_driver_find_ident(d));
      RARCH_WARN("Going to default to first audio driver...\n");

      current_audio = (const audio_driver_t*)audio_driver_find_handle(0);

      if (!current_audio)
         retroarch_fail(1, "audio_driver_find()");
   }

   return true;
}

void audio_driver_deinit_resampler(void)
{
   if (audio_driver_resampler && audio_driver_resampler_data)
      audio_driver_resampler->free(audio_driver_resampler_data);
   audio_driver_resampler      = NULL;
   audio_driver_resampler_data = NULL;
}

bool audio_driver_free_devices_list(void)
{
   if (!current_audio || !current_audio->device_list_free
         || !audio_driver_context_audio_data)
      return false;
   current_audio->device_list_free(audio_driver_context_audio_data,
         audio_driver_devices_list);
   audio_driver_devices_list = NULL;
   return true;
}

bool audio_driver_new_devices_list(void)
{
   if (!current_audio || !current_audio->device_list_new
         || !audio_driver_context_audio_data)
      return false;
   audio_driver_devices_list = (struct string_list*)
      current_audio->device_list_new(audio_driver_context_audio_data);
   if (!audio_driver_devices_list)
      return false;
   return true;
}

bool audio_driver_init(void)
{
   return audio_driver_init_internal(audio_callback.callback != NULL);
}

bool audio_driver_get_devices_list(void **data)
{
   struct string_list**ptr = (struct string_list**)data;
   if (!ptr)
      return false;
   *ptr = audio_driver_devices_list;
   return true;
}

bool audio_driver_mixer_extension_supported(const char *ext)
{
   union string_list_elem_attr attr;
   unsigned i;
   bool ret                      = false;
   struct string_list *str_list  = string_list_new();

   attr.i = 0;

#ifdef HAVE_STB_VORBIS
   string_list_append(str_list, "ogg", attr);
#endif
   string_list_append(str_list, "wav", attr);

   for (i = 0; i < str_list->size; i++)
   {
      const char *str_ext = str_list->elems[i].data;
      if (string_is_equal_noncase(str_ext, ext))
      {
         ret = true;
         break;
      }
   }

   string_list_free(str_list);

   return ret;
}

static int audio_mixer_find_index(audio_mixer_sound_t *sound)
{
   unsigned i;

   for (i = 0; i < AUDIO_MIXER_MAX_STREAMS; i++)
   {
      audio_mixer_sound_t *handle = audio_mixer_streams[i].handle;
      if (handle == sound)
         return i;
   }
   return -1;
}

static void audio_mixer_play_stop_cb(audio_mixer_sound_t *sound, unsigned reason)
{
   int idx = audio_mixer_find_index(sound);

   switch (reason)
   {
      case AUDIO_MIXER_SOUND_FINISHED:
         audio_mixer_destroy(sound);

         if (audio_mixer_streams[idx].buf != NULL)
            free(audio_mixer_streams[idx].buf);
         audio_mixer_streams[idx].state   = AUDIO_STREAM_STATE_NONE;
         audio_mixer_streams[idx].volume  = 0.0f;
         audio_mixer_streams[idx].buf     = NULL;
         audio_mixer_streams[idx].stop_cb = NULL;
         audio_mixer_streams[idx].handle  = NULL;
         audio_mixer_streams[idx].voice   = NULL;
         audio_mixer_current_max_idx--;
         break;
      case AUDIO_MIXER_SOUND_STOPPED:
         break;
      case AUDIO_MIXER_SOUND_REPEATED:
         break;
   }
}

bool audio_driver_mixer_add_stream(audio_mixer_stream_params_t *params)
{
   audio_mixer_voice_t *voice    = NULL;
   audio_mixer_sound_t *handle   = NULL;
   audio_mixer_stop_cb_t stop_cb = audio_mixer_play_stop_cb;
   bool looped                   = false;
   void *buf                     = NULL;

   if (audio_mixer_current_max_idx >= AUDIO_MIXER_MAX_STREAMS)
      return false;

   if (params->state == AUDIO_STREAM_STATE_NONE)
      return false;

   buf = malloc(params->bufsize);

   if (!buf)
      return false;

   memcpy(buf, params->buf, params->bufsize);

   switch (params->type)
   {
      case AUDIO_MIXER_TYPE_WAV:
         handle = audio_mixer_load_wav(buf, params->bufsize);
         break;
      case AUDIO_MIXER_TYPE_OGG:
         handle = audio_mixer_load_ogg(buf, params->bufsize);
         break;
      case AUDIO_MIXER_TYPE_NONE:
         free(buf);
         return false;
   }

   if (!handle)
   {
      free(buf);
      return false;
   }

   if (params->state == AUDIO_STREAM_STATE_PLAYING)
   {
      voice = audio_mixer_play(handle, looped, params->volume, stop_cb);
      audio_set_bool(AUDIO_ACTION_MIXER, true);
   }
   else if (params->state == AUDIO_STREAM_STATE_PLAYING_LOOPED)
   {
      looped = true;
      voice  = audio_mixer_play(handle, looped, params->volume, stop_cb);
      audio_set_bool(AUDIO_ACTION_MIXER, true);
   }

   audio_mixer_streams[audio_mixer_current_max_idx].buf     = buf;
   audio_mixer_streams[audio_mixer_current_max_idx].handle  = handle;
   audio_mixer_streams[audio_mixer_current_max_idx].voice   = voice;
   audio_mixer_streams[audio_mixer_current_max_idx].state   = params->state;
   audio_mixer_streams[audio_mixer_current_max_idx].volume  = params->volume;
   audio_mixer_streams[audio_mixer_current_max_idx].stop_cb = stop_cb;

   audio_mixer_current_max_idx++;

   return true;
}

static void audio_driver_mixer_remove_stream(unsigned i)
{
   audio_mixer_sound_t *handle = audio_mixer_streams[i].handle;
   audio_mixer_voice_t *voice  = audio_mixer_streams[i].voice;

   switch (audio_mixer_streams[i].state)
   {
      case AUDIO_STREAM_STATE_PLAYING:
         if (voice)
            audio_mixer_stop(voice);
         if (handle)
            audio_mixer_destroy(handle);
         break;
      case AUDIO_STREAM_STATE_PLAYING_LOOPED:
         if (voice)
            audio_mixer_stop(voice);
         if (handle)
            audio_mixer_destroy(handle);
         break;
      case AUDIO_STREAM_STATE_STOPPED:
         if (handle)
            audio_mixer_destroy(handle);
         break;
      case AUDIO_STREAM_STATE_NONE:
         break;
   }

   audio_mixer_streams[i].state   = AUDIO_STREAM_STATE_NONE;
   audio_mixer_streams[i].volume  = 0.0f;
   audio_mixer_streams[i].stop_cb = NULL;
   audio_mixer_streams[i].handle  = NULL;
   audio_mixer_streams[i].voice   = NULL;
}

static void audio_driver_mixer_deinit(void)
{
   unsigned i;

   audio_set_bool(AUDIO_ACTION_MIXER, false);

   for (i = 0; i < AUDIO_MIXER_MAX_STREAMS; i++)
      audio_driver_mixer_remove_stream(i);

   audio_mixer_current_max_idx = 0;
   audio_mixer_done();
}

bool audio_driver_deinit(void)
{
   audio_driver_mixer_deinit();
   audio_driver_free_devices_list();
   if (!audio_driver_deinit_internal())
      return false;
   return true;
}

bool audio_driver_set_callback(const void *data)
{
   const struct retro_audio_callback *cb = (const struct retro_audio_callback*)data;

   if (cb)
      audio_callback = *cb;

   return true;
}

bool audio_driver_enable_callback(void)
{
   if (!audio_callback.callback)
      return false;
   if (audio_callback.set_state)
      audio_callback.set_state(true);
   return true;
}

bool audio_driver_disable_callback(void)
{
   if (!audio_callback.callback)
      return false;

   if (audio_callback.set_state)
      audio_callback.set_state(false);
   return true;
}

/* Sets audio monitor rate to new value. */
void audio_driver_monitor_set_rate(void)
{
   settings_t *settings = config_get_ptr();
   double new_src_ratio = (double)settings->uints.audio_out_rate /
      audio_driver_input;

   audio_source_ratio_original = new_src_ratio;
   audio_source_ratio_current  = new_src_ratio;
}

bool audio_driver_callback(void)
{
   if (!audio_callback.callback)
      return false;

   if (audio_callback.callback)
      audio_callback.callback();

   return true;
}

bool audio_driver_has_callback(void)
{
   return audio_callback.callback;
}

bool audio_driver_toggle_mute(void)
{
   bool new_mute_state  = !audio_driver_mute_enable;
   if (!audio_driver_context_audio_data)
      return false;
   if (!audio_driver_active)
      return false;

   audio_driver_mute_enable = new_mute_state;

   if (new_mute_state)
      command_event(CMD_EVENT_AUDIO_STOP, NULL);
   else if (!command_event(CMD_EVENT_AUDIO_START, NULL))
   {
      audio_driver_active = false;
      return false;
   }
   return true;
}

static INLINE bool audio_driver_alive(void)
{
   if (     current_audio
         && current_audio->alive
         && audio_driver_context_audio_data)
      return current_audio->alive(audio_driver_context_audio_data);
   return false;
}

bool audio_driver_start(bool is_shutdown)
{
   if (!current_audio || !current_audio->start
         || !audio_driver_context_audio_data)
      goto error;
   if (!current_audio->start(audio_driver_context_audio_data, is_shutdown))
      goto error;

   return true;

error:
   RARCH_ERR("%s\n",
         msg_hash_to_str(MSG_FAILED_TO_START_AUDIO_DRIVER));
   audio_driver_active = false;
   return false;
}


bool audio_driver_stop(void)
{
   if (!current_audio || !current_audio->stop
         || !audio_driver_context_audio_data)
      return false;
   if (!audio_driver_alive())
      return false;
   return current_audio->stop(audio_driver_context_audio_data);
}

void audio_driver_unset_callback(void)
{
   audio_callback.callback  = NULL;
   audio_callback.set_state = NULL;
}

void audio_driver_frame_is_reverse(void)
{
   /* We just rewound. Flush rewind audio buffer. */
   audio_driver_flush(
         audio_driver_rewind_buf + audio_driver_rewind_ptr,
         audio_driver_rewind_size - audio_driver_rewind_ptr);
}

void audio_driver_destroy_data(void)
{
   audio_driver_context_audio_data = NULL;
}

void audio_driver_set_own_driver(void)
{
   audio_driver_data_own = true;
}

void audio_driver_unset_own_driver(void)
{
   audio_driver_data_own = false;
}

bool audio_driver_owns_driver(void)
{
   return audio_driver_data_own;
}

void audio_driver_set_active(void)
{
   audio_driver_active = true;
}

void audio_driver_destroy(void)
{
   audio_driver_active   = false;
   audio_driver_data_own = false;
   current_audio         = NULL;
}

void audio_set_bool(enum audio_action action, bool val)
{
   switch (action)
   {
      case AUDIO_ACTION_MIXER:
         audio_mixer_active = val;
         break;
      case AUDIO_ACTION_NONE:
      default:
         break;
   }
}

void audio_set_float(enum audio_action action, float val)
{
   switch (action)
   {
      case AUDIO_ACTION_RATE_CONTROL_DELTA:
         audio_driver_rate_control_delta = val;
         break;
      case AUDIO_ACTION_NONE:
      default:
         break;
   }
}

float *audio_get_float_ptr(enum audio_action action)
{
   switch (action)
   {
      case AUDIO_ACTION_RATE_CONTROL_DELTA:
         return &audio_driver_rate_control_delta;
      case AUDIO_ACTION_NONE:
      default:
         break;
   }

   return NULL;
}

bool *audio_get_bool_ptr(enum audio_action action)
{
   switch (action)
   {
      case AUDIO_ACTION_MUTE_ENABLE:
         return &audio_driver_mute_enable;
      case AUDIO_ACTION_NONE:
      default:
         break;
   }

   return NULL;
}
