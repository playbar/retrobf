#ifndef __AUDIO_DRIVER__H
#define __AUDIO_DRIVER__H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include <boolean.h>
#include <audio/audio_mixer.h>
#include <retro_common_api.h>

RETRO_BEGIN_DECLS

#define AUDIO_CHUNK_SIZE_BLOCKING      512

/* So we don't get complete line-noise when fast-forwarding audio. */
#define AUDIO_CHUNK_SIZE_NONBLOCKING   2048

#define AUDIO_MAX_RATIO                16

enum audio_action
{
   AUDIO_ACTION_NONE = 0,
   AUDIO_ACTION_RATE_CONTROL_DELTA,
   AUDIO_ACTION_MUTE_ENABLE,
   AUDIO_ACTION_MIXER
};

typedef struct audio_driver
{
   /* Creates and initializes handle to audio driver.
    *
    * Returns: audio driver handle on success, otherwise NULL.
    **/
   void *(*init)(const char *device, unsigned rate,
         unsigned latency, unsigned block_frames, unsigned *new_rate);

   /*
    * @data         : Pointer to audio data handle.
    * @buf          : Audio buffer data.
    * @size         : Size of audio buffer.
    *
    * Write samples to audio driver.
    *
    * Write data in buffer to audio driver.
    * A frame here is defined as one combined sample of left and right
    * channels. (I.e. 44.1kHz, 16-bit stereo has 88.2k samples/s, and
    * 44.1k frames/s.)
    *
    * Samples are interleaved in format LRLRLRLRLR ...
    * If the driver returns true in use_float(), a floating point
    * format will be used, with range [-1.0, 1.0].
    * If not, signed 16-bit samples in native byte ordering will be used.
    *
    * This function returns the number of frames successfully written.
    * If an error occurs, -1 should be returned.
    * Note that non-blocking behavior that cannot write at this time
    * should return 0 as returning -1 will terminate the driver.
    *
    * Unless said otherwise with set_nonblock_state(), all writes
    * are blocking, and it should block till it has written all frames.
    */
   ssize_t (*write)(void *data, const void *buf, size_t size);

   /* Temporarily pauses the audio driver. */
   bool (*stop)(void *data);

   /* Resumes audio driver from the paused state. */
   bool (*start)(void *data, bool is_shutdown);

   /* Is the audio driver currently running? */
   bool (*alive)(void *data);

   /* Should we care about blocking in audio thread? Fast forwarding. 
    *
    * If state is true, nonblocking operation is assumed.
    * This is typically used for fast-forwarding. If driver cannot
    * implement nonblocking writes, this can be disregarded, but should
    * log a message to stderr.
    * */
   void (*set_nonblock_state)(void *data, bool toggle);

   /* Stops and frees driver data. */
   void (*free)(void *data);

   /* Defines if driver will take standard floating point samples,
    * or int16_t samples. 
    *
    * If true is returned, the audio driver is capable of using
    * floating point data. This will likely increase performance as the
    * resampler unit uses floating point. The sample range is
    * [-1.0, 1.0].
    * */
   bool (*use_float)(void *data);

   /* Human-readable identifier. */
   const char *ident;

   /* Optional. Get audio device list (allocates, caller has to free this) */
   void *(*device_list_new)(void *data);

   /* Optional. Frees audio device list */
   void (*device_list_free)(void *data, void *data2);

   /* Optional. */
   size_t (*write_avail)(void *data);

   size_t (*buffer_size)(void *data);
} audio_driver_t;

enum audio_mixer_state
{
   AUDIO_STREAM_STATE_NONE = 0,
   AUDIO_STREAM_STATE_STOPPED,
   AUDIO_STREAM_STATE_PLAYING,
   AUDIO_STREAM_STATE_PLAYING_LOOPED
};

typedef struct audio_mixer_stream_params
{
   float volume;
   enum audio_mixer_type  type;
   enum audio_mixer_state state;
   void *buf;
   size_t bufsize;
   audio_mixer_stop_cb_t cb;
} audio_mixer_stream_params_t;

void audio_driver_destroy_data(void);

void audio_driver_set_own_driver(void);

void audio_driver_unset_own_driver(void);

void audio_driver_set_active(void);

void audio_driver_destroy(void);

void audio_driver_deinit_resampler(void);

bool audio_driver_free_devices_list(void);

bool audio_driver_new_devices_list(void);

bool audio_driver_enable_callback(void);

bool audio_driver_disable_callback(void);

/**
 * audio_driver_find_handle:
 * @index              : index of driver to get handle to.
 *
 * Returns: handle to audio driver at index. Can be NULL
 * if nothing found.
 **/
const void *audio_driver_find_handle(int index);

/**
 * audio_driver_find_ident:
 * @index              : index of driver to get handle to.
 *
 * Returns: Human-readable identifier of audio driver at index. Can be NULL
 * if nothing found.
 **/
const char *audio_driver_find_ident(int index);

void audio_driver_set_nonblocking_state(bool enable);

/**
 * config_get_audio_driver_options:
 *
 * Get an enumerated list of all audio driver names, separated by '|'.
 *
 * Returns: string listing of all audio driver names, separated by '|'.
 **/
const char* config_get_audio_driver_options(void);

void audio_driver_sample(int16_t left, int16_t right);

size_t audio_driver_sample_batch(const int16_t *data, size_t frames);

void audio_driver_sample_rewind(int16_t left, int16_t right);

size_t audio_driver_sample_batch_rewind(const int16_t *data, size_t frames);

bool audio_driver_mixer_extension_supported(const char *ext);

void audio_driver_set_volume_gain(float gain);

void audio_driver_dsp_filter_free(void);

void audio_driver_dsp_filter_init(const char *device);

void audio_driver_set_buffer_size(size_t bufsize);

bool audio_driver_get_devices_list(void **ptr);

void audio_driver_setup_rewind(void);

void audio_driver_monitor_adjust_system_rates(void);

bool audio_driver_set_callback(const void *data);

bool audio_driver_callback(void);

bool audio_driver_has_callback(void);

/* Sets audio monitor rate to new value. */
void audio_driver_monitor_set_rate(void);

bool audio_driver_find_driver(void);

bool audio_driver_toggle_mute(void);

bool audio_driver_start(bool is_shutdown);

bool audio_driver_stop(void);

bool audio_driver_owns_driver(void);

void audio_driver_unset_callback(void);

void audio_driver_frame_is_reverse(void);

void audio_set_float(enum audio_action action, float val);

void audio_set_bool(enum audio_action action, bool val);

void audio_unset_bool(enum audio_action action, bool val);

float *audio_get_float_ptr(enum audio_action action);

bool *audio_get_bool_ptr(enum audio_action action);

bool audio_driver_deinit(void);

bool audio_driver_init(void);

bool audio_driver_mixer_add_stream(audio_mixer_stream_params_t *params);

extern audio_driver_t audio_opensl;


RETRO_END_DECLS

#endif
