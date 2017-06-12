/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *  Copyright (C) 2014-2017 - Jean-André Santoni
 *  Copyright (C) 2016-2017 - Brad Parker
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

#include <ctype.h>

#include <file/config_file.h>
#include <file/file_path.h>
#include <compat/strl.h>
#include <compat/posix_string.h>
#include <retro_stat.h>
#include <retro_assert.h>
#include <string/stdstring.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "file_path_special.h"
#include "audio/audio_driver.h"
#include "input/input_driver.h"
#include "configuration.h"
#include "content.h"
#include "config.def.h"
#include "input/input_config.h"
#include "input/input_keymaps.h"
#include "input/input_remapping.h"
#include "defaults.h"
#include "core.h"
#include "dirs.h"
#include "paths.h"
#include "retroarch.h"
#include "verbosity.h"
#include "lakka.h"

#include "tasks/tasks_internal.h"

/* All config related settings go here. */

struct config_bool_setting
{ 
   const char *ident;
   bool *ptr;
   bool def_enable;
   bool def;
   bool handle;
};

struct config_int_setting
{ 
   const char *ident;
   int *ptr;
   bool def_enable;
   int def;
   bool handle;
};

struct config_uint_setting
{ 
   const char *ident;
   unsigned *ptr;
   bool def_enable;
   unsigned def;
   bool handle;
};

struct config_float_setting
{ 
   const char *ident;
   float *ptr;
   bool def_enable;
   float def;
   bool handle;
};

struct config_array_setting
{ 
   const char *ident;
   char *ptr;
   bool def_enable;
   const char *def;
   bool handle;
};

struct config_path_setting
{ 
   const char *ident;
   char *ptr;
   bool def_enable;
   char *def;
   bool handle;
};

enum video_driver_enum
{
   VIDEO_GL                 = 0,
   VIDEO_VULKAN,
   VIDEO_DRM,
   VIDEO_XVIDEO,
   VIDEO_SDL,
   VIDEO_SDL2,
   VIDEO_EXT,
   VIDEO_WII,
   VIDEO_WIIU,
   VIDEO_XENON360,
   VIDEO_XDK_D3D,
   VIDEO_PSP1,
   VIDEO_VITA2D,
   VIDEO_CTR,
   VIDEO_D3D9,
   VIDEO_VG,
   VIDEO_OMAP,
   VIDEO_EXYNOS,
   VIDEO_SUNXI,
   VIDEO_DISPMANX,
   VIDEO_CACA,
   VIDEO_GDI,
   VIDEO_VGA,
   VIDEO_NULL
};

enum audio_driver_enum
{
   AUDIO_RSOUND             = VIDEO_NULL + 1,
   AUDIO_OSS,
   AUDIO_ALSA,
   AUDIO_ALSATHREAD,
   AUDIO_ROAR,
   AUDIO_AL,
   AUDIO_SL,
   AUDIO_JACK,
   AUDIO_SDL,
   AUDIO_SDL2,
   AUDIO_XAUDIO,
   AUDIO_PULSE,
   AUDIO_EXT,
   AUDIO_DSOUND,
   AUDIO_WASAPI,
   AUDIO_COREAUDIO,
   AUDIO_PS3,
   AUDIO_XENON360,
   AUDIO_WII,
   AUDIO_WIIU,
   AUDIO_RWEBAUDIO,
   AUDIO_PSP,
   AUDIO_CTR,
   AUDIO_NULL
};

enum audio_resampler_driver_enum
{
   AUDIO_RESAMPLER_CC       = AUDIO_NULL + 1,
   AUDIO_RESAMPLER_SINC,
   AUDIO_RESAMPLER_NEAREST,
   AUDIO_RESAMPLER_NULL
};

enum input_driver_enum
{
   INPUT_ANDROID            = AUDIO_RESAMPLER_NULL + 1,
   INPUT_SDL,
   INPUT_SDL2,
   INPUT_X,
   INPUT_WAYLAND,
   INPUT_DINPUT,
   INPUT_PS3,
   INPUT_PSP,
   INPUT_CTR,
   INPUT_XENON360,
   INPUT_WII,
   INPUT_WIIU,
   INPUT_XINPUT,
   INPUT_UDEV,
   INPUT_LINUXRAW,
   INPUT_COCOA,
   INPUT_QNX,
   INPUT_RWEBINPUT,
   INPUT_DOS,
   INPUT_NULL
};

enum joypad_driver_enum
{
   JOYPAD_PS3               = INPUT_NULL + 1,
   JOYPAD_XINPUT,
   JOYPAD_GX,
   JOYPAD_WIIU,
   JOYPAD_XDK,
   JOYPAD_PSP,
   JOYPAD_CTR,
   JOYPAD_DINPUT,
   JOYPAD_UDEV,
   JOYPAD_LINUXRAW,
   JOYPAD_ANDROID,
   JOYPAD_SDL,
   JOYPAD_DOS,
   JOYPAD_HID,
   JOYPAD_QNX,
   JOYPAD_NULL
};

enum camera_driver_enum
{
   CAMERA_V4L2              = JOYPAD_NULL + 1,
   CAMERA_RWEBCAM,
   CAMERA_ANDROID,
   CAMERA_AVFOUNDATION,
   CAMERA_NULL
};

enum wifi_driver_enum
{
   WIFI_CONNMANCTL          = CAMERA_NULL + 1,
   WIFI_NULL
};

enum location_driver_enum
{
   LOCATION_ANDROID         = WIFI_NULL + 1,
   LOCATION_CORELOCATION,
   LOCATION_NULL
};

enum osk_driver_enum
{
   OSK_PS3                  = LOCATION_NULL + 1,
   OSK_NULL
};

enum menu_driver_enum
{
   MENU_RGUI                = OSK_NULL + 1,
   MENU_XUI,
   MENU_MATERIALUI,
   MENU_XMB,
   MENU_NUKLEAR,
   MENU_NULL
};

enum record_driver_enum
{
   RECORD_FFMPEG            = MENU_NULL + 1,
   RECORD_NULL
};


#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) || defined(__CELLOS_LV2__)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_GL;
#elif defined(GEKKO)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_WII;
#elif defined(WIIU)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_WIIU;
#elif defined(XENON)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_XENON360;
#elif (defined(_XBOX1) || defined(_XBOX360)) && (defined(HAVE_D3D8) || defined(HAVE_D3D9))
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_XDK_D3D;
#elif defined(HAVE_D3D9)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_D3D9;
#elif defined(HAVE_VG)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_VG;
#elif defined(HAVE_VITA2D)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_VITA2D;
#elif defined(PSP)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_PSP1;
#elif defined(_3DS)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_CTR;
#elif defined(HAVE_XVIDEO)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_XVIDEO;
#elif defined(HAVE_SDL)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_SDL;
#elif defined(HAVE_SDL2)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_SDL2;
#elif defined(_WIN32) && !defined(_XBOX)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_GDI;
#elif defined(DJGPP)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_VGA;
#elif defined(HAVE_DYLIB) && !defined(ANDROID)
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_EXT;
#else
static enum video_driver_enum VIDEO_DEFAULT_DRIVER = VIDEO_NULL;
#endif

#if defined(__CELLOS_LV2__)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_PS3;
#elif defined(XENON)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_XENON360;
#elif defined(GEKKO)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_WII;
#elif defined(WIIU)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_WIIU;
#elif defined(PSP) || defined(VITA)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_PSP;
#elif defined(_3DS)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_CTR;
#elif defined(HAVE_PULSE)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_PULSE;
#elif defined(HAVE_ALSA) && defined(HAVE_VIDEOCORE)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_ALSATHREAD;
#elif defined(HAVE_ALSA)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_ALSA;
#elif defined(HAVE_OSS)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_OSS;
#elif defined(HAVE_JACK)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_JACK;
#elif defined(HAVE_COREAUDIO)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_COREAUDIO;
#elif defined(HAVE_XAUDIO)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_XAUDIO;
#elif defined(HAVE_DSOUND)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_DSOUND;
#elif defined(HAVE_WASAPI)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_WASAPI;
#elif defined(HAVE_AL)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_AL;
#elif defined(HAVE_SL)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_SL;
#elif defined(EMSCRIPTEN)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_RWEBAUDIO;
#elif defined(HAVE_SDL)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_SDL;
#elif defined(HAVE_SDL2)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_SDL2;
#elif defined(HAVE_RSOUND)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_RSOUND;
#elif defined(HAVE_ROAR)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_ROAR;
#elif defined(HAVE_DYLIB) && !defined(ANDROID)
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_EXT;
#else
static enum audio_driver_enum AUDIO_DEFAULT_DRIVER = AUDIO_NULL;
#endif

#if defined(PSP) || defined(EMSCRIPTEN)
static enum audio_resampler_driver_enum AUDIO_DEFAULT_RESAMPLER_DRIVER = AUDIO_RESAMPLER_CC;
#else
static enum audio_resampler_driver_enum AUDIO_DEFAULT_RESAMPLER_DRIVER = AUDIO_RESAMPLER_SINC;
#endif

#if defined(HAVE_FFMPEG)
static enum record_driver_enum RECORD_DEFAULT_DRIVER = RECORD_FFMPEG;
#else
static enum record_driver_enum RECORD_DEFAULT_DRIVER = RECORD_NULL;
#endif

#if defined(XENON)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_XENON360;
#elif defined(_XBOX360) || defined(_XBOX) || defined(HAVE_XINPUT2) || defined(HAVE_XINPUT_XBOX1)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_XINPUT;
#elif defined(ANDROID)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_ANDROID;
#elif defined(EMSCRIPTEN) && defined(HAVE_SDL2)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_SDL2;
#elif defined(EMSCRIPTEN)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_RWEBINPUT;
#elif defined(_WIN32)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_DINPUT;
#elif defined(__CELLOS_LV2__)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_PS3;
#elif defined(PSP) || defined(VITA)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_PSP;
#elif defined(_3DS)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_CTR;
#elif defined(GEKKO)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_WII;
#elif defined(WIIU)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_WIIU;
#elif defined(HAVE_X11)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_X;
#elif defined(HAVE_UDEV)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_UDEV;
#elif defined(__linux__) && !defined(ANDROID)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_LINUXRAW;
#elif defined(HAVE_WAYLAND)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_WAYLAND;
#elif defined(HAVE_COCOA) || defined(HAVE_COCOATOUCH)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_COCOA;
#elif defined(__QNX__)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_QNX;
#elif defined(HAVE_SDL)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_SDL;
#elif defined(HAVE_SDL2)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_SDL2;
#elif defined(DJGPP)
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_DOS;
#else
static enum input_driver_enum INPUT_DEFAULT_DRIVER = INPUT_NULL;
#endif

#if defined(__CELLOS_LV2__)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_PS3;
#elif defined(HAVE_XINPUT)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_XINPUT;
#elif defined(GEKKO)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_GX;
#elif defined(WIIU)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_WIIU;
#elif defined(_XBOX)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_XDK;
#elif defined(PSP) || defined(VITA)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_PSP;
#elif defined(_3DS)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_CTR;
#elif defined(HAVE_DINPUT)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_DINPUT;
#elif defined(HAVE_UDEV)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_UDEV;
#elif defined(__linux) && !defined(ANDROID)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_LINUXRAW;
#elif defined(ANDROID)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_ANDROID;
#elif defined(HAVE_SDL) || defined(HAVE_SDL2)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_SDL;
#elif defined(DJGPP)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_DOS;
#elif defined(HAVE_HID)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_HID;
#elif defined(__QNX__)
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_QNX;
#else
static enum joypad_driver_enum JOYPAD_DEFAULT_DRIVER = JOYPAD_NULL;
#endif

#if defined(HAVE_V4L2)
static enum camera_driver_enum CAMERA_DEFAULT_DRIVER = CAMERA_V4L2;
#elif defined(EMSCRIPTEN)
static enum camera_driver_enum CAMERA_DEFAULT_DRIVER = CAMERA_RWEBCAM;
#elif defined(ANDROID)
static enum camera_driver_enum CAMERA_DEFAULT_DRIVER = CAMERA_ANDROID;
#elif defined(HAVE_AVFOUNDATION) && (defined(HAVE_COCOA) || defined(HAVE_COCOATOUCH))
static enum camera_driver_enum CAMERA_DEFAULT_DRIVER = CAMERA_AVFOUNDATION;
#else
static enum camera_driver_enum CAMERA_DEFAULT_DRIVER = CAMERA_NULL;
#endif

#if defined(HAVE_LAKKA)
static enum wifi_driver_enum WIFI_DEFAULT_DRIVER = WIFI_CONNMANCTL;
#else
static enum wifi_driver_enum WIFI_DEFAULT_DRIVER = WIFI_NULL;
#endif

#if defined(ANDROID)
static enum location_driver_enum LOCATION_DEFAULT_DRIVER = LOCATION_ANDROID;
#elif defined(HAVE_CORELOCATION) && (defined(HAVE_COCOA) || defined(HAVE_COCOATOUCH))
static enum location_driver_enum LOCATION_DEFAULT_DRIVER = LOCATION_CORELOCATION;
#else
static enum location_driver_enum LOCATION_DEFAULT_DRIVER = LOCATION_NULL;
#endif

#if defined(HAVE_XUI)
static enum menu_driver_enum MENU_DEFAULT_DRIVER = MENU_XUI;
#elif defined(HAVE_MATERIALUI) && defined(RARCH_MOBILE)
static enum menu_driver_enum MENU_DEFAULT_DRIVER = MENU_MATERIALUI;
#elif defined(HAVE_XMB)
static enum menu_driver_enum MENU_DEFAULT_DRIVER = MENU_XMB;
#elif defined(HAVE_RGUI)
static enum menu_driver_enum MENU_DEFAULT_DRIVER = MENU_RGUI;
#else
static enum menu_driver_enum MENU_DEFAULT_DRIVER = MENU_NULL;
#endif


#define GENERAL_SETTING(key, configval, default_enable, default_setting, type, handle_setting) \
{ \
   tmp[count].ident      = key; \
   tmp[count].ptr        = configval; \
   tmp[count].def_enable = default_enable; \
   if (default_enable) \
      tmp[count].def     = default_setting; \
   tmp[count].handle   = handle_setting; \
   count++; \
} 

#define SETTING_BOOL(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_bool_setting, handle_setting)

#define SETTING_FLOAT(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_float_setting, handle_setting)

#define SETTING_INT(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_int_setting, handle_setting)

#define SETTING_UINT(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_uint_setting, handle_setting)

#define SETTING_PATH(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_path_setting, handle_setting)

#define SETTING_ARRAY(key, configval, default_enable, default_setting, handle_setting) \
   GENERAL_SETTING(key, configval, default_enable, default_setting, struct config_array_setting, handle_setting)

struct defaults g_defaults;
static settings_t *configuration_settings = NULL;

settings_t *config_get_ptr(void)
{
   return configuration_settings;
}

void config_free(void)
{
   free(configuration_settings);
   configuration_settings = NULL;
}

bool config_init(void)
{
   configuration_settings = (settings_t*)calloc(1, sizeof(settings_t));

   if (!configuration_settings)
      return false;
   return true;
}

/**
 * config_get_default_audio:
 *
 * Gets default audio driver.
 *
 * Returns: Default audio driver.
 **/
const char *config_get_default_audio(void)
{
   enum audio_driver_enum default_driver = AUDIO_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case AUDIO_RSOUND:
         return "rsound";
      case AUDIO_OSS:
         return "oss";
      case AUDIO_ALSA:
         return "alsa";
      case AUDIO_ALSATHREAD:
         return "alsathread";
      case AUDIO_ROAR:
         return "roar";
      case AUDIO_COREAUDIO:
         return "coreaudio";
      case AUDIO_AL:
         return "openal";
      case AUDIO_SL:
         return "opensl";
      case AUDIO_SDL:
         return "sdl";
      case AUDIO_SDL2:
         return "sdl2";
      case AUDIO_DSOUND:
         return "dsound";
      case AUDIO_WASAPI:
         return "wasapi";
      case AUDIO_XAUDIO:
         return "xaudio";
      case AUDIO_PULSE:
         return "pulse";
      case AUDIO_EXT:
         return "ext";
      case AUDIO_XENON360:
         return "xenon360";
      case AUDIO_PS3:
         return "ps3";
      case AUDIO_WII:
         return "gx";
      case AUDIO_WIIU:
         return "AX";
      case AUDIO_PSP:
#ifdef VITA
         return "vita";
#else
         return "psp";
#endif
      case AUDIO_CTR:
         return "csnd";
      case AUDIO_RWEBAUDIO:
         return "rwebaudio";
      case AUDIO_JACK:
         return "jack";
      case AUDIO_NULL:
         break;
   }

   return "null";
}

const char *config_get_default_record(void)
{
   enum record_driver_enum default_driver = RECORD_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case RECORD_FFMPEG:
         return "ffmpeg";
      case RECORD_NULL:
         break;
   }

   return "null";
}

/**
 * config_get_default_audio_resampler:
 *
 * Gets default audio resampler driver.
 *
 * Returns: Default audio resampler driver.
 **/
const char *config_get_default_audio_resampler(void)
{
   enum audio_resampler_driver_enum default_driver = AUDIO_DEFAULT_RESAMPLER_DRIVER;

   switch (default_driver)
   {
      case AUDIO_RESAMPLER_CC:
         return "cc";
      case AUDIO_RESAMPLER_SINC:
         return "sinc";
      case AUDIO_RESAMPLER_NEAREST:
         return "nearest";
      case AUDIO_RESAMPLER_NULL:
         break;
   }

   return "null";
}

/**
 * config_get_default_video:
 *
 * Gets default video driver.
 *
 * Returns: Default video driver.
 **/
const char *config_get_default_video(void)
{
   enum video_driver_enum default_driver = VIDEO_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case VIDEO_GL:
         return "gl";
      case VIDEO_VULKAN:
         return "vulkan";
      case VIDEO_DRM:
         return "drm";
      case VIDEO_WII:
         return "gx";
      case VIDEO_WIIU:
         return "gx2";
      case VIDEO_XENON360:
         return "xenon360";
      case VIDEO_XDK_D3D:
      case VIDEO_D3D9:
         return "d3d";
      case VIDEO_PSP1:
         return "psp1";
      case VIDEO_VITA2D:
         return "vita2d";
      case VIDEO_CTR:
         return "ctr";
      case VIDEO_XVIDEO:
         return "xvideo";
      case VIDEO_SDL:
         return "sdl";
      case VIDEO_SDL2:
         return "sdl2";
      case VIDEO_EXT:
         return "ext";
      case VIDEO_VG:
         return "vg";
      case VIDEO_OMAP:
         return "omap";
      case VIDEO_EXYNOS:
         return "exynos";
      case VIDEO_DISPMANX:
         return "dispmanx";
      case VIDEO_SUNXI:
         return "sunxi";
      case VIDEO_CACA:
         return "caca";
      case VIDEO_GDI:
         return "gdi";
      case VIDEO_VGA:
         return "vga";
      case VIDEO_NULL:
         break;
   }

   return "null";
}

/**
 * config_get_default_input:
 *
 * Gets default input driver.
 *
 * Returns: Default input driver.
 **/
const char *config_get_default_input(void)
{
   enum input_driver_enum default_driver = INPUT_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case INPUT_ANDROID:
         return "android";
      case INPUT_PS3:
         return "ps3";
      case INPUT_PSP:
#ifdef VITA
         return "vita";
#else
         return "psp";
#endif
      case INPUT_CTR:
         return "ctr";
      case INPUT_SDL:
         return "sdl";
      case INPUT_SDL2:
         return "sdl2";
      case INPUT_DINPUT:
         return "dinput";
      case INPUT_X:
         return "x";
      case INPUT_WAYLAND:
         return "wayland";
      case INPUT_XENON360:
         return "xenon360";
      case INPUT_XINPUT:
         return "xinput";
      case INPUT_WII:
         return "gx";
      case INPUT_WIIU:
         return "wiiu";
      case INPUT_LINUXRAW:
         return "linuxraw";
      case INPUT_UDEV:
         return "udev";
      case INPUT_COCOA:
         return "cocoa";
      case INPUT_QNX:
      	 return "qnx_input";
      case INPUT_RWEBINPUT:
      	 return "rwebinput";
      case INPUT_DOS:
         return "dos";
      case INPUT_NULL:
          break;
   }

   return "null";
}

/**
 * config_get_default_joypad:
 *
 * Gets default input joypad driver.
 *
 * Returns: Default input joypad driver.
 **/
const char *config_get_default_joypad(void)
{
   enum joypad_driver_enum default_driver = JOYPAD_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case JOYPAD_PS3:
         return "ps3";
      case JOYPAD_XINPUT:
         return "xinput";
      case JOYPAD_GX:
         return "gx";
      case JOYPAD_WIIU:
         return "wiiu";
      case JOYPAD_XDK:
         return "xdk";
      case JOYPAD_PSP:
#ifdef VITA
         return "vita";
#else
         return "psp";
#endif
      case JOYPAD_CTR:
         return "ctr";
      case JOYPAD_DINPUT:
         return "dinput";
      case JOYPAD_UDEV:
         return "udev";
      case JOYPAD_LINUXRAW:
         return "linuxraw";
      case JOYPAD_ANDROID:
         return "android";
      case JOYPAD_SDL:
#ifdef HAVE_SDL2
         return "sdl2";
#else
         return "sdl";
#endif
      case JOYPAD_HID:
         return "hid";
      case JOYPAD_QNX:
         return "qnx";
      case JOYPAD_DOS:
         return "dos";
      case JOYPAD_NULL:
         break;
   }

   return "null";
}


/**
 * config_get_default_camera:
 *
 * Gets default camera driver.
 *
 * Returns: Default camera driver.
 **/
const char *config_get_default_camera(void)
{
   enum camera_driver_enum default_driver = CAMERA_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case CAMERA_V4L2:
         return "video4linux2";
      case CAMERA_RWEBCAM:
         return "rwebcam";
      case CAMERA_ANDROID:
         return "android";
      case CAMERA_AVFOUNDATION:
         return "avfoundation";
      case CAMERA_NULL:
         break;
   }

   return "null";
}

/**
 * config_get_default_wifi:
 *
 * Gets default wifi driver.
 *
 * Returns: Default wifi driver.
 **/
const char *config_get_default_wifi(void)
{
   enum wifi_driver_enum default_driver = WIFI_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case WIFI_CONNMANCTL:
         return "connmanctl";
      case WIFI_NULL:
         break;
   }

   return "null";
}

/**
 * config_get_default_location:
 *
 * Gets default location driver.
 *
 * Returns: Default location driver.
 **/
const char *config_get_default_location(void)
{
   enum location_driver_enum default_driver = LOCATION_DEFAULT_DRIVER;

   switch (default_driver)
   {
      case LOCATION_ANDROID:
         return "android";
      case LOCATION_CORELOCATION:
         return "corelocation";
      case LOCATION_NULL:
         break;
   }

   return "null";
}

#ifdef HAVE_MENU
/**
 * config_get_default_menu:
 *
 * Gets default menu driver.
 *
 * Returns: Default menu driver.
 **/
const char *config_get_default_menu(void)
{
   enum menu_driver_enum default_driver = MENU_DEFAULT_DRIVER;

   if (!string_is_empty(g_defaults.settings.menu))
      return g_defaults.settings.menu;

   switch (default_driver)
   {
      case MENU_RGUI:
         return "rgui";
      case MENU_XUI:
         return "xui";
      case MENU_MATERIALUI:
         return "glui";
      case MENU_XMB:
         return "xmb";
      case MENU_NUKLEAR:
         return "nuklear";
      case MENU_NULL:
         break;
   }

   return "null";
}

#endif

bool config_overlay_enable_default(void)
{
   if (g_defaults.overlay.set)
      return g_defaults.overlay.enable;
   return true;
}

static struct config_array_setting *populate_settings_array(settings_t *settings, int *size)
{
   unsigned count                        = 0;
   struct config_array_setting  *tmp    = (struct config_array_setting*)malloc((*size + 1) * sizeof(struct config_array_setting));

   /* Arrays */
   SETTING_ARRAY("playlist_names",           settings->arrays.playlist_names, false, NULL, true);
   SETTING_ARRAY("playlist_cores",           settings->arrays.playlist_cores, false, NULL, true);
   SETTING_ARRAY("video_driver",             settings->arrays.video_driver,   false, NULL, true);
   SETTING_ARRAY("record_driver",            settings->arrays.record_driver,  false, NULL, true);
   SETTING_ARRAY("camera_driver",            settings->arrays.camera_driver,  false, NULL, true);
   SETTING_ARRAY("wifi_driver",              settings->arrays.wifi_driver,    false, NULL, true);
   SETTING_ARRAY("location_driver",          settings->arrays.location_driver,false, NULL, true);
#ifdef HAVE_MENU
   SETTING_ARRAY("menu_driver",              settings->arrays.menu_driver,    false, NULL, true);
#endif
   SETTING_ARRAY("audio_device",             settings->arrays.audio_device,   false, NULL, true);
   SETTING_ARRAY("camera_device",            settings->arrays.camera_device,  false, NULL, true);
#ifdef HAVE_CHEEVOS
   SETTING_ARRAY("cheevos_username",         settings->arrays.cheevos_username, false, NULL, true);
   SETTING_ARRAY("cheevos_password",         settings->arrays.cheevos_password, false, NULL, true);
#endif
   SETTING_ARRAY("video_context_driver",     settings->arrays.video_context_driver,   false, NULL, true);
   SETTING_ARRAY("audio_driver",             settings->arrays.audio_driver,           false, NULL, true);
   SETTING_ARRAY("audio_resampler",          settings->arrays.audio_resampler,        false, NULL, true);
   SETTING_ARRAY("input_driver",             settings->arrays.input_driver,           false, NULL, true);
   SETTING_ARRAY("input_joypad_driver",      settings->arrays.input_joypad_driver,    false, NULL, true);
   SETTING_ARRAY("input_keyboard_layout",    settings->arrays.input_keyboard_layout,  false, NULL, true);
   SETTING_ARRAY("bundle_assets_src_path",   settings->arrays.bundle_assets_src, false, NULL, true);
   SETTING_ARRAY("bundle_assets_dst_path",   settings->arrays.bundle_assets_dst, false, NULL, true);
   SETTING_ARRAY("bundle_assets_dst_path_subdir", settings->arrays.bundle_assets_dst_subdir, false, NULL, true);

   *size = count;

   return tmp;
}

static struct config_path_setting *populate_settings_path(settings_t *settings, int *size)
{
   unsigned count = 0;
   global_t   *global                  = global_get_ptr();
   struct config_path_setting  *tmp    = (struct config_path_setting*)malloc((*size + 1) * sizeof(struct config_path_setting));

   /* Paths */
#ifdef HAVE_XMB
   SETTING_PATH("xmb_font",                   settings->paths.path_menu_xmb_font, false, NULL, true);
#endif
   SETTING_PATH("netplay_nickname",           settings->paths.username, false, NULL, true);
   SETTING_PATH("video_filter",               settings->paths.path_softfilter_plugin, false, NULL, true);
   SETTING_PATH("audio_dsp_plugin",           settings->paths.path_audio_dsp_plugin, false, NULL, true);
   SETTING_PATH("core_updater_buildbot_url", settings->paths.network_buildbot_url, false, NULL, true);
   SETTING_PATH("core_updater_buildbot_assets_url", settings->paths.network_buildbot_assets_url, false, NULL, true);
#ifdef HAVE_NETWORKING
   SETTING_PATH("netplay_ip_address",       settings->paths.netplay_server, false, NULL, true);
   SETTING_PATH("netplay_password",           settings->paths.netplay_password, false, NULL, true);
   SETTING_PATH("netplay_spectate_password",  settings->paths.netplay_spectate_password, false, NULL, true);
#endif
   SETTING_PATH("libretro_directory",
         settings->paths.directory_libretro, false, NULL, false);
   SETTING_PATH("core_options_path",
         settings->paths.path_core_options, false, NULL, true);
   SETTING_PATH("libretro_info_path",
         settings->paths.path_libretro_info, false, NULL, true);
   SETTING_PATH("video_shader",
         settings->paths.path_shader, false, NULL, true);
   SETTING_PATH("content_database_path",
         settings->paths.path_content_database, false, NULL, true);
   SETTING_PATH("cheat_database_path",
         settings->paths.path_cheat_database, false, NULL, true);
#ifdef HAVE_MENU
   SETTING_PATH("menu_wallpaper", 
         settings->paths.path_menu_wallpaper, false, NULL, true);
#endif
   SETTING_PATH("content_history_path",
         settings->paths.path_content_history, false, NULL, true);
   SETTING_PATH("content_music_history_path",
         settings->paths.path_content_music_history, false, NULL, true);
   SETTING_PATH("content_video_history_path",
         settings->paths.path_content_video_history, false, NULL, true);
   SETTING_PATH("content_image_history_path",
         settings->paths.path_content_image_history, false, NULL, true);
#ifdef HAVE_OVERLAY
   SETTING_PATH("input_overlay",
         settings->paths.path_overlay, false, NULL, true);
#endif
   SETTING_PATH("video_font_path",
         settings->paths.path_font, false, NULL, true);
   SETTING_PATH("cursor_directory",
         settings->paths.directory_cursor, false, NULL, true);
   SETTING_PATH("content_history_dir", 
         settings->paths.directory_content_history, false, NULL, true);
   SETTING_PATH("screenshot_directory",
         settings->paths.directory_screenshot, true, NULL, true);
   SETTING_PATH("system_directory",
         settings->paths.directory_system, true, NULL, true);
   SETTING_PATH("cache_directory",
         settings->paths.directory_cache, false, NULL, true);
   SETTING_PATH("input_remapping_directory",
         settings->paths.directory_input_remapping, false, NULL, true);
   SETTING_PATH("resampler_directory",
         settings->paths.directory_resampler, false, NULL, true);
   SETTING_PATH("video_shader_dir",
         settings->paths.directory_video_shader, true, NULL, true);
   SETTING_PATH("video_filter_dir",
         settings->paths.directory_video_filter, true, NULL, true);
   SETTING_PATH("core_assets_directory",
         settings->paths.directory_core_assets, true, NULL, true);
   SETTING_PATH("assets_directory",
         settings->paths.directory_assets, true, NULL, true);
   SETTING_PATH("dynamic_wallpapers_directory",
         settings->paths.directory_dynamic_wallpapers, true, NULL, true);
   SETTING_PATH("thumbnails_directory",
         settings->paths.directory_thumbnails, true, NULL, true);
   SETTING_PATH("playlist_directory",
         settings->paths.directory_playlist, true, NULL, true);
   SETTING_PATH("joypad_autoconfig_dir",
         settings->paths.directory_autoconfig, false, NULL, true);
   SETTING_PATH("audio_filter_dir",
         settings->paths.directory_audio_filter, true, NULL, true);
   SETTING_PATH("savefile_directory", 
         dir_get_ptr(RARCH_DIR_SAVEFILE), true, NULL, false);
   SETTING_PATH("savestate_directory",
         dir_get_ptr(RARCH_DIR_SAVESTATE), true, NULL, false);
#ifdef HAVE_MENU
   SETTING_PATH("rgui_browser_directory",
         settings->paths.directory_menu_content, true, NULL, true);
   SETTING_PATH("rgui_config_directory",
         settings->paths.directory_menu_config, true, NULL, true);
#endif
#ifdef HAVE_OVERLAY
   SETTING_PATH("overlay_directory",
         settings->paths.directory_overlay, true, NULL, true);
#endif
#ifndef HAVE_DYNAMIC
   SETTING_PATH("libretro_path", 
         path_get_ptr(RARCH_PATH_CORE), false, NULL, false);
#endif
   SETTING_PATH(
         "screenshot_directory", 
         settings->paths.directory_screenshot, true, NULL, false);

   if (global)
   {
      SETTING_PATH("recording_output_directory",
            global->record.output_dir, false, NULL, true);
      SETTING_PATH("recording_config_directory",
            global->record.config_dir, false, NULL, true);
   }

   *size = count;

   return tmp;
}

static struct config_bool_setting *populate_settings_bool(settings_t *settings, int *size)
{
   struct config_bool_setting  *tmp    = (struct config_bool_setting*)malloc((*size + 1) * sizeof(struct config_bool_setting));
   unsigned count                      = 0;
   global_t   *global                  = global_get_ptr();

   SETTING_BOOL("ui_companion_start_on_boot",    &settings->bools.ui_companion_start_on_boot, true, ui_companion_start_on_boot, false);
   SETTING_BOOL("ui_companion_enable",           &settings->bools.ui_companion_enable, true, ui_companion_enable, false);
   SETTING_BOOL("video_gpu_record",              &settings->bools.video_gpu_record, true, gpu_record, false);
   SETTING_BOOL("input_remap_binds_enable",      &settings->bools.input_remap_binds_enable, true, true, false);
   SETTING_BOOL("all_users_control_menu",        &settings->bools.input_all_users_control_menu, true, all_users_control_menu, false);
   SETTING_BOOL("menu_swap_ok_cancel_buttons",   &settings->bools.input_menu_swap_ok_cancel_buttons, true, menu_swap_ok_cancel_buttons, false);
#ifdef HAVE_NETWORKING
   SETTING_BOOL("netplay_public_announce",       &settings->bools.netplay_public_announce, true, netplay_public_announce, false);
   SETTING_BOOL("netplay_start_as_spectator",    &settings->bools.netplay_start_as_spectator, false, netplay_start_as_spectator, false);
   SETTING_BOOL("netplay_allow_slaves",          &settings->bools.netplay_allow_slaves, true, netplay_allow_slaves, false);
   SETTING_BOOL("netplay_require_slaves",        &settings->bools.netplay_require_slaves, true, netplay_require_slaves, false);
   SETTING_BOOL("netplay_stateless_mode",        &settings->bools.netplay_stateless_mode, true, netplay_stateless_mode, false);
   SETTING_BOOL("netplay_client_swap_input",     &settings->bools.netplay_swap_input, true, netplay_client_swap_input, false);
   SETTING_BOOL("netplay_use_mitm_server",       &settings->bools.netplay_use_mitm_server, true, netplay_use_mitm_server, false);
#endif
   SETTING_BOOL("input_descriptor_label_show",   &settings->bools.input_descriptor_label_show, true, input_descriptor_label_show, false);
   SETTING_BOOL("input_descriptor_hide_unbound", &settings->bools.input_descriptor_hide_unbound, true, input_descriptor_hide_unbound, false);
   SETTING_BOOL("load_dummy_on_core_shutdown",   &settings->bools.load_dummy_on_core_shutdown, true, load_dummy_on_core_shutdown, false);
   SETTING_BOOL("check_firmware_before_loading", &settings->bools.check_firmware_before_loading, true, check_firmware_before_loading, false);
   SETTING_BOOL("builtin_mediaplayer_enable",    &settings->bools.multimedia_builtin_mediaplayer_enable, false, false /* TODO */, false);
   SETTING_BOOL("builtin_imageviewer_enable",    &settings->bools.multimedia_builtin_imageviewer_enable, true, true, false);
   SETTING_BOOL("fps_show",                      &settings->bools.video_fps_show, true, false, false);
   SETTING_BOOL("ui_menubar_enable",             &settings->bools.ui_menubar_enable, true, true, false);
   SETTING_BOOL("suspend_screensaver_enable",    &settings->bools.ui_suspend_screensaver_enable, true, true, false);
   SETTING_BOOL("rewind_enable",                 &settings->bools.rewind_enable, true, rewind_enable, false);
   SETTING_BOOL("audio_sync",                    &settings->bools.audio_sync, true, audio_sync, false);
   SETTING_BOOL("video_shader_enable",           &settings->bools.video_shader_enable, true, shader_enable, false);

   /* Let implementation decide if automatic, or 1:1 PAR. */
   SETTING_BOOL("video_aspect_ratio_auto",       &settings->bools.video_aspect_ratio_auto, true, aspect_ratio_auto, false);

   SETTING_BOOL("video_allow_rotate",            &settings->bools.video_allow_rotate, true, allow_rotate, false);
   SETTING_BOOL("video_windowed_fullscreen",     &settings->bools.video_windowed_fullscreen, true, windowed_fullscreen, false);
   SETTING_BOOL("video_crop_overscan",           &settings->bools.video_crop_overscan, true, crop_overscan, false);
   SETTING_BOOL("video_scale_integer",           &settings->bools.video_scale_integer, true, scale_integer, false);
   SETTING_BOOL("video_smooth",                  &settings->bools.video_smooth, true, video_smooth, false);
   SETTING_BOOL("video_force_aspect",            &settings->bools.video_force_aspect, true, force_aspect, false);
   SETTING_BOOL("video_threaded",                video_driver_get_threaded(), true, video_threaded, false);
   SETTING_BOOL("video_shared_context",          &settings->bools.video_shared_context, true, video_shared_context, false);
   SETTING_BOOL("auto_screenshot_filename",      &settings->bools.auto_screenshot_filename, true, auto_screenshot_filename, false);
   SETTING_BOOL("video_force_srgb_disable",      &settings->bools.video_force_srgb_disable, true, false, false);
   SETTING_BOOL("video_fullscreen",              &settings->bools.video_fullscreen, true, fullscreen, false);
   SETTING_BOOL("bundle_assets_extract_enable",  &settings->bools.bundle_assets_extract_enable, true, bundle_assets_extract_enable, false);
   SETTING_BOOL("video_vsync",                   &settings->bools.video_vsync, true, vsync, false);
   SETTING_BOOL("video_hard_sync",               &settings->bools.video_hard_sync, true, hard_sync, false);
   SETTING_BOOL("video_black_frame_insertion",   &settings->bools.video_black_frame_insertion, true, black_frame_insertion, false);
   SETTING_BOOL("video_disable_composition",     &settings->bools.video_disable_composition, true, disable_composition, false);
   SETTING_BOOL("pause_nonactive",               &settings->bools.pause_nonactive, true, pause_nonactive, false);
   SETTING_BOOL("video_gpu_screenshot",          &settings->bools.video_gpu_screenshot, true, gpu_screenshot, false);
   SETTING_BOOL("video_post_filter_record",      &settings->bools.video_post_filter_record, true, post_filter_record, false);
   SETTING_BOOL("keyboard_gamepad_enable",       &settings->bools.input_keyboard_gamepad_enable, true, true, false);
   SETTING_BOOL("core_set_supports_no_game_enable", &settings->bools.set_supports_no_game_enable, true, true, false);
   SETTING_BOOL("audio_enable",                  &settings->bools.audio_enable, true, audio_enable, false);
   SETTING_BOOL("audio_mute_enable",             audio_get_bool_ptr(AUDIO_ACTION_MUTE_ENABLE), true, false, false);
   SETTING_BOOL("location_allow",                &settings->bools.location_allow, true, false, false);
   SETTING_BOOL("video_font_enable",             &settings->bools.video_font_enable, true, font_enable, false);
   SETTING_BOOL("core_updater_auto_extract_archive", &settings->bools.network_buildbot_auto_extract_archive, true, true, false);
   SETTING_BOOL("camera_allow",                  &settings->bools.camera_allow, true, false, false);
#if defined(VITA)
   SETTING_BOOL("input_backtouch_enable",         &settings->bools.input_backtouch_enable, false, false, false);
   SETTING_BOOL("input_backtouch_toggle",         &settings->bools.input_backtouch_toggle, false, false, false);
#endif
#if TARGET_OS_IPHONE
   SETTING_BOOL("small_keyboard_enable",         &settings->bools.input_small_keyboard_enable, true, false, false);
#endif
#ifdef GEKKO
   SETTING_BOOL("video_vfilter",                 &settings->bools.video_vfilter, true, video_vfilter, false);
#endif
#ifdef HAVE_MENU
   SETTING_BOOL("menu_unified_controls",         &settings->bools.menu_unified_controls, true, false, false);
#ifdef HAVE_THREADS
   SETTING_BOOL("threaded_data_runloop_enable",  &settings->bools.threaded_data_runloop_enable, true, threaded_data_runloop_enable, false);
#endif
   SETTING_BOOL("menu_throttle_framerate",       &settings->bools.menu_throttle_framerate, true, true, false);
   SETTING_BOOL("menu_linear_filter",            &settings->bools.menu_linear_filter, true, true, false);
   SETTING_BOOL("dpi_override_enable",           &settings->bools.menu_dpi_override_enable, true, menu_dpi_override_enable, false);
   SETTING_BOOL("menu_pause_libretro",           &settings->bools.menu_pause_libretro, true, true, false);
   SETTING_BOOL("menu_mouse_enable",             &settings->bools.menu_mouse_enable, true, def_mouse_enable, false);
   SETTING_BOOL("menu_pointer_enable",           &settings->bools.menu_pointer_enable, true, pointer_enable, false);
   SETTING_BOOL("menu_timedate_enable",          &settings->bools.menu_timedate_enable, true, true, false);
   SETTING_BOOL("menu_battery_level_enable",     &settings->bools.menu_battery_level_enable, true, true, false);
   SETTING_BOOL("menu_core_enable",              &settings->bools.menu_core_enable, true, true, false);
   SETTING_BOOL("menu_dynamic_wallpaper_enable", &settings->bools.menu_dynamic_wallpaper_enable, true, false, false);
#ifdef HAVE_XMB
   SETTING_BOOL("xmb_shadows_enable",            &settings->bools.menu_xmb_shadows_enable, true, xmb_shadows_enable, false);
   SETTING_BOOL("xmb_show_settings",             &settings->bools.menu_xmb_show_settings, true, xmb_show_settings, false);
#ifdef HAVE_IMAGEVIEWER
   SETTING_BOOL("xmb_show_images",               &settings->bools.menu_xmb_show_images, true, xmb_show_images, false);
#endif
   SETTING_BOOL("xmb_show_music",                &settings->bools.menu_xmb_show_music, true, xmb_show_music, false);
#ifdef HAVE_FFMPEG
   SETTING_BOOL("xmb_show_video",                &settings->bools.menu_xmb_show_video, true, xmb_show_video, false);
#endif
#ifdef HAVE_NETWORKING
   SETTING_BOOL("xmb_show_netplay",              &settings->bools.menu_xmb_show_netplay, true, xmb_show_netplay, false);
#endif
   SETTING_BOOL("xmb_show_history",              &settings->bools.menu_xmb_show_history, true, xmb_show_history, false);
#ifdef HAVE_LIBRETRODB 
   SETTING_BOOL("xmb_show_add",                  &settings->bools.menu_xmb_show_add, true, xmb_show_add, false);
#endif
#endif
   SETTING_BOOL("filter_by_current_core",        &settings->bools.filter_by_current_core, false, false /* TODO */, false);
   SETTING_BOOL("rgui_show_start_screen",        &settings->bools.menu_show_start_screen, false, false /* TODO */, false);
   SETTING_BOOL("menu_navigation_wraparound_enable", &settings->bools.menu_navigation_wraparound_enable, true, true, false);
   SETTING_BOOL("menu_navigation_browser_filter_supported_extensions_enable", 
         &settings->bools.menu_navigation_browser_filter_supported_extensions_enable, true, true, false);
   SETTING_BOOL("menu_show_advanced_settings",  &settings->bools.menu_show_advanced_settings, true, show_advanced_settings, false);
#endif
#ifdef HAVE_CHEEVOS
   SETTING_BOOL("cheevos_enable",               &settings->bools.cheevos_enable, true, cheevos_enable, false);
   SETTING_BOOL("cheevos_test_unofficial",      &settings->bools.cheevos_test_unofficial, true, false, false);
   SETTING_BOOL("cheevos_hardcore_mode_enable", &settings->bools.cheevos_hardcore_mode_enable, true, false, false);
#endif
#ifdef HAVE_OVERLAY
   SETTING_BOOL("input_overlay_enable",         &settings->bools.input_overlay_enable, true, config_overlay_enable_default(), false);
   SETTING_BOOL("input_overlay_enable_autopreferred", &settings->bools.input_overlay_enable_autopreferred, true, true, false);
   SETTING_BOOL("input_overlay_hide_in_menu",   &settings->bools.input_overlay_hide_in_menu, true, overlay_hide_in_menu, false);
#endif
#ifdef HAVE_COMMAND
   SETTING_BOOL("network_cmd_enable",           &settings->bools.network_cmd_enable, true, network_cmd_enable, false);
   SETTING_BOOL("stdin_cmd_enable",             &settings->bools.stdin_cmd_enable, true, stdin_cmd_enable, false);
#endif
#ifdef HAVE_NETWORKGAMEPAD
   SETTING_BOOL("network_remote_enable",        &settings->bools.network_remote_enable, false, false /* TODO */, false);
#endif
#ifdef HAVE_NETWORKING
   SETTING_BOOL("netplay_nat_traversal",        &settings->bools.netplay_nat_traversal, true, true, false);
#endif
   SETTING_BOOL("block_sram_overwrite",         &settings->bools.block_sram_overwrite, true, block_sram_overwrite, false);
   SETTING_BOOL("savestate_auto_index",         &settings->bools.savestate_auto_index, true, savestate_auto_index, false);
   SETTING_BOOL("savestate_auto_save",          &settings->bools.savestate_auto_save, true, savestate_auto_save, false);
   SETTING_BOOL("savestate_auto_load",          &settings->bools.savestate_auto_load, true, savestate_auto_load, false);
   SETTING_BOOL("savestate_thumbnail_enable",   &settings->bools.savestate_thumbnail_enable, true, savestate_thumbnail_enable, false);
   SETTING_BOOL("history_list_enable",          &settings->bools.history_list_enable, true, def_history_list_enable, false);
   SETTING_BOOL("playlist_entry_remove",        &settings->bools.playlist_entry_remove, true, def_playlist_entry_remove, false);
   SETTING_BOOL("game_specific_options",        &settings->bools.game_specific_options, true, default_game_specific_options, false);
   SETTING_BOOL("auto_overrides_enable",        &settings->bools.auto_overrides_enable, true, default_auto_overrides_enable, false);
   SETTING_BOOL("auto_remaps_enable",           &settings->bools.auto_remaps_enable, true, default_auto_remaps_enable, false);
   SETTING_BOOL("auto_shaders_enable",          &settings->bools.auto_shaders_enable, true, default_auto_shaders_enable, false);
   SETTING_BOOL("sort_savefiles_enable",        &settings->bools.sort_savefiles_enable, true, default_sort_savefiles_enable, false);
   SETTING_BOOL("sort_savestates_enable",       &settings->bools.sort_savestates_enable, true, default_sort_savestates_enable, false);
   SETTING_BOOL("config_save_on_exit",          &settings->bools.config_save_on_exit, true, config_save_on_exit, false);
   SETTING_BOOL("show_hidden_files",            &settings->bools.show_hidden_files, true, show_hidden_files, false);
   SETTING_BOOL("input_autodetect_enable",      &settings->bools.input_autodetect_enable, true, input_autodetect_enable, false);
   SETTING_BOOL("audio_rate_control",           &settings->bools.audio_rate_control, true, rate_control, false);
#ifdef HAVE_WASAPI
   SETTING_BOOL("audio_wasapi_exclusive_mode",  &settings->bools.audio_wasapi_exclusive_mode, true, wasapi_exclusive_mode, false);
   SETTING_BOOL("audio_wasapi_float_format",    &settings->bools.audio_wasapi_float_format, true, wasapi_float_format, false);
#endif

   if (global)
   {
      SETTING_BOOL("custom_bgm_enable",         &global->console.sound.system_bgm_enable, true, false, false);
   }

   *size = count;

   return tmp;
}

static struct config_float_setting *populate_settings_float(settings_t *settings, int *size)
{
   unsigned count = 0;
   struct config_float_setting  *tmp      = (struct config_float_setting*)malloc((*size + 1) * sizeof(struct config_float_setting));

   SETTING_FLOAT("video_aspect_ratio",       &settings->floats.video_aspect_ratio, true, aspect_ratio, false);
   SETTING_FLOAT("video_scale",              &settings->floats.video_scale, false, 0.0f, false);
   SETTING_FLOAT("video_refresh_rate",       &settings->floats.video_refresh_rate, true, refresh_rate, false);
   SETTING_FLOAT("audio_rate_control_delta", audio_get_float_ptr(AUDIO_ACTION_RATE_CONTROL_DELTA), true, rate_control_delta, false);
   SETTING_FLOAT("audio_max_timing_skew",    &settings->floats.audio_max_timing_skew, true, max_timing_skew, false);
   SETTING_FLOAT("audio_volume",             &settings->floats.audio_volume, true, audio_volume, false);
#ifdef HAVE_OVERLAY
   SETTING_FLOAT("input_overlay_opacity",    &settings->floats.input_overlay_opacity, true, 0.7f, false);
   SETTING_FLOAT("input_overlay_scale",      &settings->floats.input_overlay_scale, true, 1.0f, false);
#endif
#ifdef HAVE_MENU
   SETTING_FLOAT("menu_wallpaper_opacity",   &settings->floats.menu_wallpaper_opacity, true, menu_wallpaper_opacity, false);
   SETTING_FLOAT("menu_footer_opacity",      &settings->floats.menu_footer_opacity,    true, menu_footer_opacity, false);
   SETTING_FLOAT("menu_header_opacity",      &settings->floats.menu_header_opacity,    true, menu_header_opacity, false);
#endif
   SETTING_FLOAT("video_message_pos_x",      &settings->floats.video_msg_pos_x,      true, message_pos_offset_x, false);
   SETTING_FLOAT("video_message_pos_y",      &settings->floats.video_msg_pos_y,      true, message_pos_offset_y, false);
   SETTING_FLOAT("video_font_size",          &settings->floats.video_font_size,      true, font_size, false);
   SETTING_FLOAT("fastforward_ratio",        &settings->floats.fastforward_ratio,    true, fastforward_ratio, false);
   SETTING_FLOAT("slowmotion_ratio",         &settings->floats.slowmotion_ratio,     true, slowmotion_ratio, false);
   SETTING_FLOAT("input_axis_threshold",     input_driver_get_float(INPUT_ACTION_AXIS_THRESHOLD), true, axis_threshold, false);

   *size = count;

   return tmp;
}

static struct config_uint_setting *populate_settings_uint(settings_t *settings, int *size)
{
   unsigned count                     = 0;
   struct video_viewport *custom_vp   = video_viewport_get_custom();
   struct config_uint_setting  *tmp   = (struct config_uint_setting*)malloc((*size + 1) * sizeof(struct config_uint_setting));

   SETTING_UINT("input_bind_timeout",           &settings->uints.input_bind_timeout,     true, input_bind_timeout, false);
   SETTING_UINT("input_turbo_period",           &settings->uints.input_turbo_period,     true, turbo_period, false);
   SETTING_UINT("input_duty_cycle",             &settings->uints.input_turbo_duty_cycle, true, turbo_duty_cycle, false);
   SETTING_UINT("input_max_users",              input_driver_get_uint(INPUT_ACTION_MAX_USERS),        true, input_max_users, false);
   SETTING_UINT("input_menu_toggle_gamepad_combo", &settings->uints.input_menu_toggle_gamepad_combo, true, menu_toggle_gamepad_combo, false);
   SETTING_UINT("audio_latency",                &settings->uints.audio_latency, false, 0 /* TODO */, false);
   SETTING_UINT("audio_block_frames",           &settings->uints.audio_block_frames, true, 0, false);
   SETTING_UINT("rewind_granularity",           &settings->uints.rewind_granularity, true, rewind_granularity, false);
   SETTING_UINT("autosave_interval",            &settings->uints.autosave_interval,  true, autosave_interval, false);
   SETTING_UINT("libretro_log_level",           &settings->uints.libretro_log_level, true, libretro_log_level, false);
   SETTING_UINT("keyboard_gamepad_mapping_type",&settings->uints.input_keyboard_gamepad_mapping_type, true, 1, false);
   SETTING_UINT("input_poll_type_behavior",     &settings->uints.input_poll_type_behavior, true, 2, false);
   SETTING_UINT("video_monitor_index",          &settings->uints.video_monitor_index, true, monitor_index, false);
   SETTING_UINT("video_fullscreen_x",           &settings->uints.video_fullscreen_x,  true, fullscreen_x, false);
   SETTING_UINT("video_fullscreen_y",           &settings->uints.video_fullscreen_y,  true, fullscreen_y, false);
   SETTING_UINT("video_window_x",               &settings->uints.video_window_x,  true, fullscreen_x, false);
   SETTING_UINT("video_window_y",               &settings->uints.video_window_y,  true, fullscreen_y, false);
#ifdef HAVE_COMMAND
   SETTING_UINT("network_cmd_port",             &settings->uints.network_cmd_port,    true, network_cmd_port, false);
#endif
#ifdef HAVE_NETWORKGAMEPAD
   SETTING_UINT("network_remote_base_port",     &settings->uints.network_remote_base_port, true, network_remote_base_port, false);
#endif
#ifdef GEKKO
   SETTING_UINT("video_viwidth",                &settings->uints.video_viwidth, true, video_viwidth, false);
#endif
#ifdef HAVE_MENU
   SETTING_UINT("dpi_override_value",           &settings->uints.menu_dpi_override_value, true, menu_dpi_override_value, false);
   SETTING_UINT("menu_thumbnails",              &settings->uints.menu_thumbnails, true, menu_thumbnails_default, false);
#ifdef HAVE_XMB
   SETTING_UINT("xmb_alpha_factor",             &settings->uints.menu_xmb_alpha_factor, true, xmb_alpha_factor, false);
   SETTING_UINT("xmb_scale_factor",             &settings->uints.menu_xmb_scale_factor, true, xmb_scale_factor, false);
   SETTING_UINT("xmb_theme",                    &settings->uints.menu_xmb_theme, true, xmb_icon_theme, false);
   SETTING_UINT("xmb_menu_color_theme",         &settings->uints.menu_xmb_color_theme, true, xmb_theme, false);
#endif
   SETTING_UINT("materialui_menu_color_theme",  &settings->uints.menu_materialui_color_theme, true, MATERIALUI_THEME_BLUE, false);
   SETTING_UINT("menu_shader_pipeline",         &settings->uints.menu_xmb_shader_pipeline, true, menu_shader_pipeline, false);
#endif
   SETTING_UINT("audio_out_rate",               &settings->uints.audio_out_rate, true, out_rate, false);
   SETTING_UINT("custom_viewport_width",        &custom_vp->width, false, 0 /* TODO */, false);
   SETTING_UINT("custom_viewport_height",       &custom_vp->height, false, 0 /* TODO */, false);
   SETTING_UINT("custom_viewport_x",            (unsigned*)&custom_vp->x, false, 0 /* TODO */, false);
   SETTING_UINT("custom_viewport_y",            (unsigned*)&custom_vp->y, false, 0 /* TODO */, false);
   SETTING_UINT("content_history_size",         &settings->uints.content_history_size,   true, default_content_history_size, false);
   SETTING_UINT("video_hard_sync_frames",       &settings->uints.video_hard_sync_frames, true, hard_sync_frames, false);
   SETTING_UINT("video_frame_delay",            &settings->uints.video_frame_delay,      true, frame_delay, false);
   SETTING_UINT("video_max_swapchain_images",   &settings->uints.video_max_swapchain_images, true, max_swapchain_images, false);
   SETTING_UINT("video_swap_interval",          &settings->uints.video_swap_interval, true, swap_interval, false);
   SETTING_UINT("video_rotation",               &settings->uints.video_rotation, true, ORIENTATION_NORMAL, false);
   SETTING_UINT("aspect_ratio_index",           &settings->uints.video_aspect_ratio_idx, true, aspect_ratio_idx, false);
#ifdef HAVE_NETWORKING
   SETTING_UINT("netplay_ip_port",              &settings->uints.netplay_port,         true, RARCH_DEFAULT_PORT, false);
   SETTING_UINT("netplay_input_latency_frames_min",&settings->uints.netplay_input_latency_frames_min, true, 0, false);
   SETTING_UINT("netplay_input_latency_frames_range",&settings->uints.netplay_input_latency_frames_range, true, 0, false);
#endif
#ifdef HAVE_LANGEXTRA
   SETTING_UINT("user_language",                msg_hash_get_uint(MSG_HASH_USER_LANGUAGE), true, RETRO_LANGUAGE_ENGLISH, false);
#endif
   SETTING_UINT("bundle_assets_extract_version_current", &settings->uints.bundle_assets_extract_version_current, true, 0, false);
   SETTING_UINT("bundle_assets_extract_last_version",    &settings->uints.bundle_assets_extract_last_version, true, 0, false);

   *size = count;

   return tmp;
}

static struct config_int_setting *populate_settings_int(settings_t *settings, int *size)
{
   unsigned count                     = 0;
   struct config_int_setting  *tmp    = (struct config_int_setting*)malloc((*size + 1) * sizeof(struct config_int_setting));

   SETTING_INT("state_slot",                   &settings->ints.state_slot, false, 0 /* TODO */, false);
#ifdef HAVE_NETWORKING
   SETTING_INT("netplay_check_frames",         &settings->ints.netplay_check_frames, true, netplay_check_frames, false);
#endif
#ifdef HAVE_WASAPI
   SETTING_INT("audio_wasapi_sh_buffer_length", &settings->ints.audio_wasapi_sh_buffer_length, true, wasapi_sh_buffer_length, false);
#endif

   *size = count;

   return tmp;
}

/**
 * config_set_defaults:
 *
 * Set 'default' configuration values.
 **/
static void config_set_defaults(void)
{
   unsigned i, j;
#ifdef HAVE_MENU
   static bool first_initialized   = true;
#endif
   settings_t *settings            = config_get_ptr();
   int bool_settings_size          = sizeof(settings->bools)   / sizeof(settings->bools.placeholder);
   int float_settings_size         = sizeof(settings->floats)  / sizeof(settings->floats.placeholder);
   int int_settings_size           = sizeof(settings->ints)    / sizeof(settings->ints.placeholder);
   int uint_settings_size          = sizeof(settings->uints)   / sizeof(settings->uints.placeholder);
   const char *def_video           = config_get_default_video();
   const char *def_audio           = config_get_default_audio();
   const char *def_audio_resampler = config_get_default_audio_resampler();
   const char *def_input           = config_get_default_input();
   const char *def_joypad          = config_get_default_joypad();
#ifdef HAVE_MENU
   const char *def_menu            = config_get_default_menu();
#endif
   const char *def_camera          = config_get_default_camera();
   const char *def_wifi            = config_get_default_wifi();
   const char *def_location        = config_get_default_location();
   const char *def_record          = config_get_default_record();
   struct config_float_setting      *float_settings = populate_settings_float  (settings, &float_settings_size);
   struct config_bool_setting       *bool_settings  = populate_settings_bool  (settings, &bool_settings_size);
   struct config_int_setting        *int_settings   = populate_settings_int   (settings, &int_settings_size);
   struct config_uint_setting       *uint_settings  = populate_settings_uint   (settings, &uint_settings_size);

   if (bool_settings && (bool_settings_size > 0))
   {
      for (i = 0; i < (unsigned)bool_settings_size; i++)
      {
         if (bool_settings[i].def_enable)
            *bool_settings[i].ptr = bool_settings[i].def;
      }

      free(bool_settings);
   }

   if (int_settings && (int_settings_size > 0))
   {
      for (i = 0; i < (unsigned)int_settings_size; i++)
      {
         if (int_settings[i].def_enable)
            *int_settings[i].ptr = int_settings[i].def;
      }

      free(int_settings);
   }

   if (uint_settings && (uint_settings_size > 0))
   {
      for (i = 0; i < (unsigned)uint_settings_size; i++)
      {
         if (uint_settings[i].def_enable)
            *uint_settings[i].ptr = uint_settings[i].def;
      }

      free(uint_settings);
   }

   if (float_settings && (float_settings_size > 0))
   {
      for (i = 0; i < (unsigned)float_settings_size; i++)
      {
         if (float_settings[i].def_enable)
            *float_settings[i].ptr = float_settings[i].def;
      }

      free(float_settings);
   }

   if (def_camera)
      strlcpy(settings->arrays.camera_driver,
            def_camera, sizeof(settings->arrays.camera_driver));
   if (def_wifi)
      strlcpy(settings->arrays.wifi_driver,
            def_wifi, sizeof(settings->arrays.wifi_driver));
   if (def_location)
      strlcpy(settings->arrays.location_driver,
            def_location, sizeof(settings->arrays.location_driver));
   if (def_video)
      strlcpy(settings->arrays.video_driver,
            def_video, sizeof(settings->arrays.video_driver));
   if (def_audio)
      strlcpy(settings->arrays.audio_driver,
            def_audio, sizeof(settings->arrays.audio_driver));
   if (def_audio_resampler)
      strlcpy(settings->arrays.audio_resampler,
            def_audio_resampler, sizeof(settings->arrays.audio_resampler));
   if (def_input)
      strlcpy(settings->arrays.input_driver,
            def_input, sizeof(settings->arrays.input_driver));
   if (def_joypad)
      strlcpy(settings->arrays.input_joypad_driver,
            def_joypad, sizeof(settings->arrays.input_joypad_driver));
   if (def_record)
      strlcpy(settings->arrays.record_driver,
            def_record, sizeof(settings->arrays.record_driver));
#ifdef HAVE_MENU
   if (def_menu)
      strlcpy(settings->arrays.menu_driver,
            def_menu,  sizeof(settings->arrays.menu_driver));
#ifdef HAVE_XMB
   *settings->paths.path_menu_xmb_font            = '\0';
#endif

#ifdef HAVE_MATERIALUI
   if (g_defaults.menu.materialui.menu_color_theme_enable)
      settings->uints.menu_materialui_color_theme = g_defaults.menu.materialui.menu_color_theme;
#endif
#endif

#ifdef HAVE_FFMPEG
   configuration_set_bool(settings, settings->bools.multimedia_builtin_mediaplayer_enable, true);
#else
   configuration_set_bool(settings, settings->bools.multimedia_builtin_mediaplayer_enable, false);
#endif
   settings->floats.video_scale                = scale;

   if (retroarch_is_forced_fullscreen())
   {
      configuration_set_bool(settings, settings->bools.video_fullscreen, true);
   }

   if (g_defaults.settings.video_threaded_enable != video_threaded)
      video_driver_set_threaded(g_defaults.settings.video_threaded_enable);

   settings->floats.video_msg_color_r          = ((message_color >> 16) & 0xff) / 255.0f;
   settings->floats.video_msg_color_g          = ((message_color >>  8) & 0xff) / 255.0f;
   settings->floats.video_msg_color_b          = ((message_color >>  0) & 0xff) / 255.0f;

   if (g_defaults.settings.video_refresh_rate > 0.0 &&
         g_defaults.settings.video_refresh_rate != refresh_rate)
      settings->floats.video_refresh_rate      = g_defaults.settings.video_refresh_rate;

   if (audio_device)
      strlcpy(settings->arrays.audio_device,
            audio_device, sizeof(settings->arrays.audio_device));

   if (!g_defaults.settings.out_latency)
      g_defaults.settings.out_latency          = out_latency;

   settings->uints.audio_latency               = g_defaults.settings.out_latency;

   audio_driver_set_volume_gain(db_to_gain(settings->floats.audio_volume));

   settings->rewind_buffer_size                = rewind_buffer_size;

#ifdef HAVE_LAKKA
   settings->bools.ssh_enable                  = path_file_exists(LAKKA_SSH_PATH);
   settings->bools.samba_enable                = path_file_exists(LAKKA_SAMBA_PATH);
   settings->bools.bluetooth_enable            = path_file_exists(LAKKA_BLUETOOTH_PATH);
#endif

#ifdef HAVE_MENU
   if (first_initialized)
      settings->bools.menu_show_start_screen   = default_menu_show_start_screen;
   settings->uints.menu_entry_normal_color     = menu_entry_normal_color;
   settings->uints.menu_entry_hover_color      = menu_entry_hover_color;
   settings->uints.menu_title_color            = menu_title_color;
#endif

#ifdef HAVE_CHEEVOS
   *settings->arrays.cheevos_username                 = '\0';
   *settings->arrays.cheevos_password                 = '\0';
#endif

   input_config_reset();
   input_remapping_set_defaults();
   input_autoconfigure_reset();

   /* Verify that binds are in proper order. */
   for (i = 0; i < MAX_USERS; i++)
   {
      for (j = 0; j < RARCH_BIND_LIST_END; j++)
      {
         const struct retro_keybind *keyval = &input_config_binds[i][j];
         if (keyval->valid)
            retro_assert(j == keyval->id);
      }
   }

   strlcpy(settings->paths.network_buildbot_url, buildbot_server_url,
         sizeof(settings->paths.network_buildbot_url));
   strlcpy(settings->paths.network_buildbot_assets_url, buildbot_assets_server_url,
         sizeof(settings->paths.network_buildbot_assets_url));

   *settings->arrays.input_keyboard_layout                = '\0';

   for (i = 0; i < MAX_USERS; i++)
   {
      settings->uints.input_joypad_map[i] = i;
      settings->uints.input_analog_dpad_mode[i] = ANALOG_DPAD_NONE;
      if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_LIBRETRO_DEVICE, &i))
         input_config_set_device(i, RETRO_DEVICE_JOYPAD);
   }

   video_driver_reset_custom_viewport();

   /* Make sure settings from other configs carry over into defaults
    * for another config. */
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_SAVE_PATH, NULL))
      dir_clear(RARCH_DIR_SAVEFILE);
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL))
      dir_clear(RARCH_DIR_SAVESTATE);

   *settings->paths.path_libretro_info = '\0';
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_LIBRETRO_DIRECTORY, NULL))
      *settings->paths.directory_libretro = '\0';
   *settings->paths.directory_cursor = '\0';
   *settings->paths.directory_resampler = '\0';
   *settings->paths.directory_screenshot = '\0';
   *settings->paths.directory_system = '\0';
   *settings->paths.directory_cache = '\0';
   *settings->paths.directory_input_remapping = '\0';
   *settings->paths.directory_core_assets = '\0';
   *settings->paths.directory_assets = '\0';
   *settings->paths.directory_dynamic_wallpapers = '\0';
   *settings->paths.directory_thumbnails = '\0';
   *settings->paths.directory_playlist = '\0';
   *settings->paths.directory_autoconfig = '\0';
#ifdef HAVE_MENU
   *settings->paths.directory_menu_content = '\0';
   *settings->paths.directory_menu_config = '\0';
#endif
   *settings->paths.directory_video_shader = '\0';
   *settings->paths.directory_video_filter = '\0';
   *settings->paths.directory_audio_filter = '\0';

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_UPS_PREF, NULL))
      rarch_ctl(RARCH_CTL_UNSET_UPS_PREF, NULL);
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_BPS_PREF, NULL))
      rarch_ctl(RARCH_CTL_UNSET_BPS_PREF, NULL);
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_IPS_PREF, NULL))
      rarch_ctl(RARCH_CTL_UNSET_IPS_PREF, NULL);

   {
      global_t *global =  global_get_ptr();

      if (global)
      {
         *global->record.output_dir = '\0';
         *global->record.config_dir = '\0';
      }
   }

   *settings->paths.path_core_options      = '\0';
   *settings->paths.path_content_history   = '\0';
   *settings->paths.path_content_music_history   = '\0';
   *settings->paths.path_content_image_history   = '\0';
   *settings->paths.path_content_video_history   = '\0';
   *settings->paths.path_cheat_settings    = '\0';
   *settings->paths.path_shader            = '\0';
#ifndef IOS
   *settings->arrays.bundle_assets_src = '\0';
   *settings->arrays.bundle_assets_dst = '\0';
   *settings->arrays.bundle_assets_dst_subdir = '\0';
#endif
   *settings->paths.path_cheat_database    = '\0';
   *settings->paths.path_menu_wallpaper    = '\0';
   *settings->paths.path_content_database  = '\0';
   *settings->paths.path_overlay           = '\0';
   *settings->paths.path_softfilter_plugin = '\0';

   *settings->arrays.playlist_names = '\0';
   *settings->arrays.playlist_cores = '\0';
   *settings->paths.directory_content_history = '\0';
   *settings->paths.path_audio_dsp_plugin = '\0';

   video_driver_default_settings();

   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_WALLPAPERS]))
      strlcpy(settings->paths.directory_dynamic_wallpapers,
            g_defaults.dirs[DEFAULT_DIR_WALLPAPERS],
            sizeof(settings->paths.directory_dynamic_wallpapers));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS]))
      strlcpy(settings->paths.directory_thumbnails,
            g_defaults.dirs[DEFAULT_DIR_THUMBNAILS],
            sizeof(settings->paths.directory_thumbnails));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_REMAP]))
      strlcpy(settings->paths.directory_input_remapping,
            g_defaults.dirs[DEFAULT_DIR_REMAP],
            sizeof(settings->paths.directory_input_remapping));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CACHE]))
      strlcpy(settings->paths.directory_cache,
            g_defaults.dirs[DEFAULT_DIR_CACHE],
            sizeof(settings->paths.directory_cache));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_ASSETS]))
      strlcpy(settings->paths.directory_assets,
            g_defaults.dirs[DEFAULT_DIR_ASSETS],
            sizeof(settings->paths.directory_assets));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS]))
      strlcpy(settings->paths.directory_core_assets,
            g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS],
            sizeof(settings->paths.directory_core_assets));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_PLAYLIST]))
      strlcpy(settings->paths.directory_playlist,
            g_defaults.dirs[DEFAULT_DIR_PLAYLIST],
            sizeof(settings->paths.directory_playlist));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CORE]))
      fill_pathname_expand_special(settings->paths.directory_libretro,
            g_defaults.dirs[DEFAULT_DIR_CORE],
            sizeof(settings->paths.directory_libretro));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER]))
      strlcpy(settings->paths.directory_audio_filter,
            g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER],
            sizeof(settings->paths.directory_audio_filter));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER]))
      strlcpy(settings->paths.directory_video_filter,
            g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER],
            sizeof(settings->paths.directory_video_filter));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_SHADER]))
      fill_pathname_expand_special(settings->paths.directory_video_shader,
            g_defaults.dirs[DEFAULT_DIR_SHADER],
            sizeof(settings->paths.directory_video_shader));

   if (!string_is_empty(g_defaults.path.buildbot_server_url))
      strlcpy(settings->paths.network_buildbot_url,
            g_defaults.path.buildbot_server_url,
            sizeof(settings->paths.network_buildbot_url));
   if (!string_is_empty(g_defaults.path.core))
      rarch_ctl(RARCH_CTL_SET_LIBRETRO_PATH, g_defaults.path.core);
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_DATABASE]))
      strlcpy(settings->paths.path_content_database,
            g_defaults.dirs[DEFAULT_DIR_DATABASE],
            sizeof(settings->paths.path_content_database));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CURSOR]))
      strlcpy(settings->paths.directory_cursor,
            g_defaults.dirs[DEFAULT_DIR_CURSOR],
            sizeof(settings->paths.directory_cursor));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CHEATS]))
      strlcpy(settings->paths.path_cheat_database,
            g_defaults.dirs[DEFAULT_DIR_CHEATS],
            sizeof(settings->paths.path_cheat_database));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CORE_INFO]))
      fill_pathname_expand_special(settings->paths.path_libretro_info,
            g_defaults.dirs[DEFAULT_DIR_CORE_INFO],
            sizeof(settings->paths.path_libretro_info));
#ifdef HAVE_OVERLAY
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_OVERLAY]))
   {
      fill_pathname_expand_special(settings->paths.directory_overlay,
            g_defaults.dirs[DEFAULT_DIR_OVERLAY],
            sizeof(settings->paths.directory_overlay));
#ifdef RARCH_MOBILE
      if (string_is_empty(settings->paths.path_overlay))
            fill_pathname_join(settings->paths.path_overlay,
                  settings->paths.directory_overlay,
                  "gamepads/flat/retropad.cfg",
                  sizeof(settings->paths.path_overlay));
#endif
   }
#endif

#ifdef HAVE_MENU
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG]))
   {
      strlcpy(settings->paths.directory_menu_config,
            g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG],
            sizeof(settings->paths.directory_menu_config));
#if TARGET_OS_IPHONE
      path_set(RARCH_PATH_CONFIG,
            settings->paths.directory_menu_config);
#endif
   }

   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_MENU_CONTENT]))
      strlcpy(settings->paths.directory_menu_content,
            g_defaults.dirs[DEFAULT_DIR_MENU_CONTENT],
            sizeof(settings->paths.directory_menu_content));
#endif
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG]))
      strlcpy(settings->paths.directory_autoconfig,
            g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG],
            sizeof(settings->paths.directory_autoconfig));

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL) &&
         !string_is_empty(g_defaults.dirs[DEFAULT_DIR_SAVESTATE]))
      dir_set(RARCH_DIR_SAVESTATE, g_defaults.dirs[DEFAULT_DIR_SAVESTATE]);

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_SAVE_PATH, NULL) &&
         !string_is_empty(g_defaults.dirs[DEFAULT_DIR_SRAM]))
      dir_set(RARCH_DIR_SAVEFILE, g_defaults.dirs[DEFAULT_DIR_SRAM]);

   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_SYSTEM]))
      strlcpy(settings->paths.directory_system,
            g_defaults.dirs[DEFAULT_DIR_SYSTEM],
            sizeof(settings->paths.directory_system));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]))
      strlcpy(settings->paths.directory_screenshot,
            g_defaults.dirs[DEFAULT_DIR_SCREENSHOT],
            sizeof(settings->paths.directory_screenshot));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_RESAMPLER]))
      strlcpy(settings->paths.directory_resampler,
            g_defaults.dirs[DEFAULT_DIR_RESAMPLER],
            sizeof(settings->paths.directory_resampler));
   if (!string_is_empty(g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY]))
      strlcpy(settings->paths.directory_content_history,
            g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY],
            sizeof(settings->paths.directory_content_history));

   if (!string_is_empty(g_defaults.path.config))
   {
      char temp_str[PATH_MAX_LENGTH];

      temp_str[0] = '\0';

      fill_pathname_expand_special(temp_str,
            g_defaults.path.config, sizeof(temp_str));
      path_set(RARCH_PATH_CONFIG, temp_str);
   }

   /* Avoid reloading config on every content load */
   if (default_block_config_read)
      rarch_ctl(RARCH_CTL_SET_BLOCK_CONFIG_READ, NULL);
   else
      rarch_ctl(RARCH_CTL_UNSET_BLOCK_CONFIG_READ, NULL);

#ifdef HAVE_MENU
   first_initialized = false;
#endif
}

/**
 * open_default_config_file
 *
 * Open a default config file. Platform-specific.
 *
 * Returns: handle to config file if found, otherwise NULL.
 **/
static config_file_t *open_default_config_file(void)
{
   char application_data[PATH_MAX_LENGTH];
   char conf_path[PATH_MAX_LENGTH];
   char app_path[PATH_MAX_LENGTH];
   config_file_t *conf                    = NULL;

   application_data[0] = conf_path[0] = app_path[0] = '\0';

#if defined(_WIN32) && !defined(_XBOX)
   fill_pathname_application_path(app_path, sizeof(app_path));
   fill_pathname_resolve_relative(conf_path, app_path,
         file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));

   conf = config_file_new(conf_path);

   if (!conf)
   {
      if (fill_pathname_application_data(application_data,
            sizeof(application_data)))
      {
         fill_pathname_join(conf_path, application_data,
               file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));
         conf = config_file_new(conf_path);
      }
   }

   if (!conf)
   {
      bool saved = false;

      /* Try to create a new config file. */
      conf = config_file_new(NULL);


      if (conf)
      {
         /* Since this is a clean config file, we can
          * safely use config_save_on_exit. */
         fill_pathname_resolve_relative(conf_path, app_path,
               file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));
         config_set_bool(conf, "config_save_on_exit", true);
         saved = config_file_write(conf, conf_path);
      }

      if (!saved)
      {
         /* WARN here to make sure user has a good chance of seeing it. */
         RARCH_ERR("Failed to create new config file in: \"%s\".\n",
               conf_path);
         config_file_free(conf);
         return NULL;
      }

      RARCH_WARN("Created new config file in: \"%s\".\n", conf_path);
   }
#elif defined(OSX)
   if (!fill_pathname_application_data(application_data,
            sizeof(application_data)))
      return NULL;

   /* Group config file with menu configs, remaps, etc: */
   strlcat(application_data, "/config", sizeof(application_data));

   path_mkdir(application_data);

   fill_pathname_join(conf_path, application_data,
         file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));
   conf = config_file_new(conf_path);

   if (!conf)
   {
      bool saved = false;

      conf = config_file_new(NULL);

      if (conf)
      {
         config_set_bool(conf, "config_save_on_exit", true);
         saved = config_file_write(conf, conf_path);
      }

      if (!saved)
      {
         /* WARN here to make sure user has a good chance of seeing it. */
         RARCH_ERR("Failed to create new config file in: \"%s\".\n",
               conf_path);
         config_file_free(conf);

         return NULL;
      }

      RARCH_WARN("Created new config file in: \"%s\".\n", conf_path);
   }
#elif !defined(RARCH_CONSOLE)
   bool has_application_data = 
      fill_pathname_application_data(application_data,
            sizeof(application_data));

   if (has_application_data)
   {
      fill_pathname_join(conf_path, application_data,
            file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));
      RARCH_LOG("Looking for config in: \"%s\".\n", conf_path);
      conf = config_file_new(conf_path);
   }

   /* Fallback to $HOME/.retroarch.cfg. */
   if (!conf && getenv("HOME"))
   {
      fill_pathname_join(conf_path, getenv("HOME"),
            ".retroarch.cfg", sizeof(conf_path));
      RARCH_LOG("Looking for config in: \"%s\".\n", conf_path);
      conf = config_file_new(conf_path);
   }

   if (!conf && has_application_data)
   {
      char basedir[PATH_MAX_LENGTH];

      basedir[0] = '\0';

      /* Try to create a new config file. */

      strlcpy(conf_path, application_data, sizeof(conf_path));

      fill_pathname_basedir(basedir, conf_path, sizeof(basedir));

      fill_pathname_join(conf_path, conf_path,
            file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(conf_path));

      if (path_mkdir(basedir))
      {
         char skeleton_conf[PATH_MAX_LENGTH];
         bool saved                          = false;

         skeleton_conf[0] = '\0';

#if defined(__HAIKU__)
         fill_pathname_join(skeleton_conf, "/system/settings",
               file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(skeleton_conf));
#else
         fill_pathname_join(skeleton_conf, "/etc",
               file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(skeleton_conf));
#endif
         conf = config_file_new(skeleton_conf);
         if (conf)
            RARCH_WARN("Config: using skeleton config \"%s\" as base for a new config file.\n", skeleton_conf);
         else
            conf = config_file_new(NULL);

         if (conf)
         {
            /* Since this is a clean config file, we can safely use config_save_on_exit. */
            config_set_bool(conf, "config_save_on_exit", true);
            saved = config_file_write(conf, conf_path);
         }

         if (!saved)
         {
            /* WARN here to make sure user has a good chance of seeing it. */
            RARCH_ERR("Failed to create new config file in: \"%s\".\n", conf_path);
            config_file_free(conf);

            return NULL;
         }

         RARCH_WARN("Config: Created new config file in: \"%s\".\n", conf_path);
      }
   }
#endif

   (void)application_data;
   (void)conf_path;
   (void)app_path;

   if (!conf)
      return NULL;

   path_set(RARCH_PATH_CONFIG, conf_path);

   return conf;
}

static void read_keybinds_keyboard(config_file_t *conf, unsigned user,
      unsigned idx, struct retro_keybind *bind)
{
   const char *prefix = NULL;

   if (!input_config_bind_map_get_valid(idx))
      return;

   if (!input_config_bind_map_get_base(idx))
      return;

   prefix = input_config_get_prefix(user, input_config_bind_map_get_meta(idx));

   if (prefix)
      input_config_parse_key(conf, prefix,
            input_config_bind_map_get_base(idx), bind);
}

static void read_keybinds_button(config_file_t *conf, unsigned user,
      unsigned idx, struct retro_keybind *bind)
{
   const char *prefix = NULL;

   if (!input_config_bind_map_get_valid(idx))
      return;
   if (!input_config_bind_map_get_base(idx))
      return;

   prefix = input_config_get_prefix(user,
         input_config_bind_map_get_meta(idx));

   if (prefix)
      input_config_parse_joy_button(conf, prefix,
            input_config_bind_map_get_base(idx), bind);
}

static void read_keybinds_axis(config_file_t *conf, unsigned user,
      unsigned idx, struct retro_keybind *bind)
{
   const char *prefix = NULL;

   if (!input_config_bind_map_get_valid(idx))
      return;
   if (!input_config_bind_map_get_base(idx))
      return;

   prefix = input_config_get_prefix(user,
         input_config_bind_map_get_meta(idx));

   if (prefix)
      input_config_parse_joy_axis(conf, prefix,
            input_config_bind_map_get_base(idx), bind);
}

static void read_keybinds_user(config_file_t *conf, unsigned user)
{
   unsigned i;

   for (i = 0; input_config_bind_map_get_valid(i); i++)
   {
      struct retro_keybind *bind = &input_config_binds[user][i];

      if (!bind->valid)
         continue;

      read_keybinds_keyboard(conf, user, i, bind);
      read_keybinds_button(conf, user, i, bind);
      read_keybinds_axis(conf, user, i, bind);
   }
}

static void config_read_keybinds_conf(config_file_t *conf)
{
   unsigned i;

   for (i = 0; i < MAX_USERS; i++)
      read_keybinds_user(conf, i);
}

static bool check_shader_compatibility(enum file_path_enum enum_idx)
{
   settings_t *settings = config_get_ptr();

   if (string_is_equal_fast(settings->arrays.video_driver, "vulkan", 6))
   {
      if (enum_idx != FILE_PATH_SLANGP_EXTENSION)
         return false;
      return true;
   }

   if (string_is_equal_fast(settings->arrays.video_driver, "gl", 2) || 
       string_is_equal_fast(settings->arrays.video_driver, "d3d", 3)
      )
   {
      if (enum_idx == FILE_PATH_SLANGP_EXTENSION)
         return false;
      return true;
   }

   return false;
}

#if 0
static bool config_read_keybinds(const char *path)
{
   config_file_t *conf = (config_file_t*)config_file_new(path);

   if (!conf)
      return false;

   config_read_keybinds_conf(conf);
   config_file_free(conf);

   return true;
}

/* Also dumps inherited values, useful for logging. */
static void config_file_dump_all(config_file_t *conf)
{
   struct config_entry_list *list = NULL;
   struct config_include_list *includes = conf->includes;

   while (includes)
   {
      RARCH_LOG("#include \"%s\"\n", includes->path);
      includes = includes->next;
   }

   list = conf->entries;

   while (list)
   {
      RARCH_LOG("%s = \"%s\"%s\n", list->key,
            list->value, list->readonly ? " (included)" : "");
      list = list->next;
   }
}
#endif

#ifdef HAVE_MENU
static void config_get_hex_base(config_file_t *conf,
      const char *key, unsigned *base)
{
   unsigned tmp = 0;
   if (!base)
      return;
   if (config_get_hex(conf, key, &tmp))
      *base = tmp;
}
#endif


/**
 * config_load:
 * @path                : path to be read from.
 * @set_defaults        : set default values first before
 *                        reading the values from the config file
 *
 * Loads a config file and reads all the values into memory.
 *
 */
static bool config_load_file(const char *path, bool set_defaults, 
   settings_t *settings)
{
   unsigned i;
   char tmp_str[PATH_MAX_LENGTH];
   bool ret                                        = false;
   bool tmp_bool                                   = false;
   char *save                                      = NULL;
   unsigned msg_color                              = 0;
   config_file_t *conf                             = NULL;
   char *override_username                         = NULL;
#ifdef HAVE_NETWORKING
   char *override_netplay_ip_address               = NULL;
#endif
   int bool_settings_size                          = sizeof(settings->bools)  / sizeof(settings->bools.placeholder);
   int float_settings_size                         = sizeof(settings->floats) / sizeof(settings->floats.placeholder);
   int int_settings_size                           = sizeof(settings->ints)   / sizeof(settings->ints.placeholder);
   int uint_settings_size                          = sizeof(settings->uints)  / sizeof(settings->uints.placeholder);
   int array_settings_size                         = sizeof(settings->arrays) / sizeof(settings->arrays.placeholder);
   int path_settings_size                          = sizeof(settings->paths)  / sizeof(settings->paths.placeholder);
   struct config_bool_setting *bool_settings       = populate_settings_bool  (settings, &bool_settings_size);
   struct config_float_setting *float_settings     = populate_settings_float (settings, &float_settings_size);
   struct config_int_setting *int_settings         = populate_settings_int   (settings, &int_settings_size);
   struct config_uint_setting *uint_settings       = populate_settings_uint  (settings, &uint_settings_size);
   struct config_array_setting *array_settings     = populate_settings_array (settings, &array_settings_size);
   struct config_path_setting *path_settings       = populate_settings_path  (settings, &path_settings_size);

   tmp_str[0] = '\0';

   if (path)
   {
      conf = config_file_new(path);
      if (!conf)
         goto end;
   }
   else
      conf = open_default_config_file();

   if (!conf)
   {
      ret = true;
      goto end;
   }

   if (set_defaults)
      config_set_defaults();

   if (!path_is_empty(RARCH_PATH_CONFIG_APPEND))
   {
      /* Don't destroy append_config_path, store in temporary
       * variable. */
      char tmp_append_path[PATH_MAX_LENGTH];
      const char *extra_path                = NULL;

      tmp_append_path[0] = '\0';

      strlcpy(tmp_append_path, path_get(RARCH_PATH_CONFIG_APPEND),
            sizeof(tmp_append_path));
      extra_path = strtok_r(tmp_append_path, "|", &save);

      while (extra_path)
      {
         bool ret = config_append_file(conf, extra_path);

         RARCH_LOG("Config: appending config \"%s\"\n", extra_path);

         if (!ret)
            RARCH_ERR("Config: failed to append config \"%s\"\n", extra_path);
         extra_path = strtok_r(NULL, "|", &save);
      }
   }

#if 0
   if (verbosity_is_enabled())
   {
      RARCH_LOG_OUTPUT("=== Config ===\n");
      config_file_dump_all(conf);
      RARCH_LOG_OUTPUT("=== Config end ===\n");
   }
#endif

   /* Overrides */

   if (rarch_ctl(RARCH_CTL_HAS_SET_USERNAME, NULL))
      override_username = strdup(settings->paths.username);

#ifdef HAVE_NETWORKING
   if (retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_NETPLAY_IP_ADDRESS, NULL))
      override_netplay_ip_address = strdup(settings->paths.netplay_server);
#endif

   /* Boolean settings */

   for (i = 0; i < (unsigned)bool_settings_size; i++)
   {
      bool tmp = false;
      if (config_get_bool(conf, bool_settings[i].ident, &tmp))
         *bool_settings[i].ptr = tmp;
   }

   if (!retroarch_is_forced_fullscreen())
      CONFIG_GET_BOOL_BASE(conf, settings, bools.video_fullscreen, "video_fullscreen");

#ifdef HAVE_NETWORKGAMEPAD
   for (i = 0; i < MAX_USERS; i++)
   {
      char tmp[64];

      tmp[0] = '\0';

      snprintf(tmp, sizeof(tmp), "network_remote_enable_user_p%u", i + 1);

      if (config_get_bool(conf, tmp, &tmp_bool))
         settings->bools.network_remote_enable_user[i] = tmp_bool;
   }
#endif
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_VERBOSITY, NULL))
   {
      if (config_get_bool(conf, "log_verbosity", &tmp_bool))
      {
         if (tmp_bool)
            verbosity_enable();
         else
            verbosity_disable();
      }
   }
   {
      char tmp[64];

      tmp[0] = '\0';

      strlcpy(tmp, "perfcnt_enable", sizeof(tmp));
      if (config_get_bool(conf, tmp, &tmp_bool))
      {
         if (tmp_bool)
            rarch_ctl(RARCH_CTL_SET_PERFCNT_ENABLE, NULL);
         else
            rarch_ctl(RARCH_CTL_UNSET_PERFCNT_ENABLE, NULL);
      }
   }

   /* Integer settings */

   for (i = 0; i < (unsigned)int_settings_size; i++)
   {
      int tmp = 0;
      if (config_get_int(conf, int_settings[i].ident, &tmp))
         *int_settings[i].ptr = tmp;
   }

   for (i = 0; i < (unsigned)uint_settings_size; i++)
   {
      int tmp = 0;
      if (config_get_int(conf, uint_settings[i].ident, &tmp))
         *uint_settings[i].ptr = tmp;
   }

#ifdef HAVE_NETWORKING
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_NETPLAY_STATELESS_MODE, NULL))
      CONFIG_GET_BOOL_BASE(conf, settings, bools.netplay_stateless_mode, "netplay_stateless_mode");
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_NETPLAY_CHECK_FRAMES, NULL))
      CONFIG_GET_INT_BASE(conf, settings, ints.netplay_check_frames, "netplay_check_frames");
   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_NETPLAY_IP_PORT, NULL))
      CONFIG_GET_INT_BASE(conf, settings, uints.netplay_port, "netplay_ip_port");
#endif
   for (i = 0; i < MAX_USERS; i++)
   {
      char buf[64];

      buf[0] = '\0';

      snprintf(buf, sizeof(buf), "input_player%u_joypad_index", i + 1);
      CONFIG_GET_INT_BASE(conf, settings, uints.input_joypad_map[i], buf);

      snprintf(buf, sizeof(buf), "input_player%u_analog_dpad_mode", i + 1);
      CONFIG_GET_INT_BASE(conf, settings, uints.input_analog_dpad_mode[i], buf);

      if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_LIBRETRO_DEVICE, &i))
      {
         snprintf(buf, sizeof(buf), "input_libretro_device_p%u", i + 1);
         CONFIG_GET_INT_BASE(conf, settings, uints.input_libretro_device[i], buf);
      }
   }
   {
      /* ugly hack around C89 not allowing mixing declarations and code */
      int buffer_size = 0;
      if (config_get_int(conf, "rewind_buffer_size", &buffer_size))
         settings->rewind_buffer_size = buffer_size * UINT64_C(1000000);
   }


   /* Hexadecimal settings  */

   if (config_get_hex(conf, "video_message_color", &msg_color))
   {
      settings->floats.video_msg_color_r = ((msg_color >> 16) & 0xff) / 255.0f;
      settings->floats.video_msg_color_g = ((msg_color >>  8) & 0xff) / 255.0f;
      settings->floats.video_msg_color_b = ((msg_color >>  0) & 0xff) / 255.0f;
   }
#ifdef HAVE_MENU
   config_get_hex_base(conf, "menu_entry_normal_color",
         &settings->uints.menu_entry_normal_color);
   config_get_hex_base(conf, "menu_entry_hover_color",
         &settings->uints.menu_entry_hover_color);
   config_get_hex_base(conf, "menu_title_color",
         &settings->uints.menu_title_color);
#endif

   /* Float settings */
   for (i = 0; i < (unsigned)float_settings_size; i++)
   {
      float tmp = 0.0f;
      if (config_get_float(conf, float_settings[i].ident, &tmp))
         *float_settings[i].ptr = tmp;
   }

   /* Array settings  */
   for (i = 0; i < (unsigned)array_settings_size; i++)
   {
      if (!array_settings[i].handle)
         continue;
      config_get_array(conf, array_settings[i].ident,
            array_settings[i].ptr, PATH_MAX_LENGTH);
   }

   /* Path settings  */
   for (i = 0; i < (unsigned)path_settings_size; i++)
   {
      if (!path_settings[i].handle)
         continue;
      if (config_get_path(conf, path_settings[i].ident, tmp_str, sizeof(tmp_str)))
         strlcpy(path_settings[i].ptr, tmp_str, PATH_MAX_LENGTH);
   }

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_LIBRETRO_DIRECTORY, NULL))
   {
      if (config_get_path(conf, "libretro_directory", tmp_str, sizeof(tmp_str)))
            strlcpy(settings->paths.directory_libretro, tmp_str, sizeof(settings->paths.directory_libretro));
   }

#ifndef HAVE_DYNAMIC
   if (config_get_path(conf, "libretro_path", tmp_str, sizeof(tmp_str)))
      path_set(RARCH_PATH_CORE, tmp_str);
#endif

#ifdef RARCH_CONSOLE
   video_driver_load_settings(conf);
#endif

   /* Post-settings load */

   if (rarch_ctl(RARCH_CTL_HAS_SET_USERNAME, NULL) && override_username)
   {
      strlcpy(settings->paths.username,
            override_username,
            sizeof(settings->paths.username));
      free(override_username);
   }

#ifdef HAVE_NETWORKING
   if (retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_NETPLAY_IP_ADDRESS, NULL))
   {
      strlcpy(settings->paths.netplay_server,
            override_netplay_ip_address,
            sizeof(settings->paths.netplay_server));
      free(override_netplay_ip_address);
   }
#endif

   if (settings->uints.video_hard_sync_frames > 3)
      settings->uints.video_hard_sync_frames = 3;

   if (settings->uints.video_frame_delay > 15)
      settings->uints.video_frame_delay = 15;

   settings->uints.video_swap_interval = MAX(settings->uints.video_swap_interval, 1);
   settings->uints.video_swap_interval = MIN(settings->uints.video_swap_interval, 4);

   audio_driver_set_volume_gain(db_to_gain(settings->floats.audio_volume));

   if (string_is_empty(settings->paths.path_content_history))
   {
      if (string_is_empty(settings->paths.directory_content_history))
      {
         fill_pathname_resolve_relative(
               settings->paths.path_content_history,
               path_get(RARCH_PATH_CONFIG),
               file_path_str(FILE_PATH_CONTENT_HISTORY),
               sizeof(settings->paths.path_content_history));
      }
      else
      {
         fill_pathname_join(settings->paths.path_content_history,
               settings->paths.directory_content_history,
               file_path_str(FILE_PATH_CONTENT_HISTORY),
               sizeof(settings->paths.path_content_history));
      }
   }

   if (string_is_empty(settings->paths.path_content_music_history))
   {
      if (string_is_empty(settings->paths.directory_content_history))
      {
         fill_pathname_resolve_relative(
               settings->paths.path_content_music_history,
               path_get(RARCH_PATH_CONFIG),
               file_path_str(FILE_PATH_CONTENT_MUSIC_HISTORY),
               sizeof(settings->paths.path_content_music_history));
      }
      else
      {
         fill_pathname_join(settings->paths.path_content_music_history,
               settings->paths.directory_content_history,
               file_path_str(FILE_PATH_CONTENT_MUSIC_HISTORY),
               sizeof(settings->paths.path_content_music_history));
      }
   }

   if (string_is_empty(settings->paths.path_content_video_history))
   {
      if (string_is_empty(settings->paths.directory_content_history))
      {
         fill_pathname_resolve_relative(
               settings->paths.path_content_video_history,
               path_get(RARCH_PATH_CONFIG),
               file_path_str(FILE_PATH_CONTENT_VIDEO_HISTORY),
               sizeof(settings->paths.path_content_video_history));
      }
      else
      {
         fill_pathname_join(settings->paths.path_content_video_history,
               settings->paths.directory_content_history,
               file_path_str(FILE_PATH_CONTENT_VIDEO_HISTORY),
               sizeof(settings->paths.path_content_video_history));
      }
   }

   if (string_is_empty(settings->paths.path_content_image_history))
   {
      if (string_is_empty(settings->paths.directory_content_history))
      {
         fill_pathname_resolve_relative(
               settings->paths.path_content_image_history,
               path_get(RARCH_PATH_CONFIG),
               file_path_str(FILE_PATH_CONTENT_IMAGE_HISTORY),
               sizeof(settings->paths.path_content_image_history));
      }
      else
      {
         fill_pathname_join(settings->paths.path_content_image_history,
               settings->paths.directory_content_history,
               file_path_str(FILE_PATH_CONTENT_IMAGE_HISTORY),
               sizeof(settings->paths.path_content_image_history));
      }
   }


   if (!string_is_empty(settings->paths.directory_screenshot))
   {
      if (string_is_equal_fast(settings->paths.directory_screenshot, "default", 7))
         *settings->paths.directory_screenshot = '\0';
      else if (!path_is_directory(settings->paths.directory_screenshot))
      {
         RARCH_WARN("screenshot_directory is not an existing directory, ignoring ...\n");
         *settings->paths.directory_screenshot = '\0';
      }
   }

   /* Safe-guard against older behavior. */
   if (path_is_directory(path_get(RARCH_PATH_CORE)))
   {
      RARCH_WARN("\"libretro_path\" is a directory, using this for \"libretro_directory\" instead.\n");
      strlcpy(settings->paths.directory_libretro, path_get(RARCH_PATH_CORE),
            sizeof(settings->paths.directory_libretro));
      path_clear(RARCH_PATH_CORE);
   }

   if (string_is_equal_fast(settings->paths.path_menu_wallpaper, "default", 7))
      *settings->paths.path_menu_wallpaper = '\0';
   if (string_is_equal_fast(settings->paths.directory_video_shader, "default", 7))
      *settings->paths.directory_video_shader = '\0';
   if (string_is_equal_fast(settings->paths.directory_video_filter, "default", 7))
      *settings->paths.directory_video_filter = '\0';
   if (string_is_equal_fast(settings->paths.directory_audio_filter, "default", 7))
      *settings->paths.directory_audio_filter = '\0';
   if (string_is_equal_fast(settings->paths.directory_core_assets, "default", 7))
      *settings->paths.directory_core_assets = '\0';
   if (string_is_equal_fast(settings->paths.directory_assets, "default", 7))
      *settings->paths.directory_assets = '\0';
   if (string_is_equal_fast(settings->paths.directory_dynamic_wallpapers, "default", 7))
      *settings->paths.directory_dynamic_wallpapers = '\0';
   if (string_is_equal_fast(settings->paths.directory_thumbnails, "default", 7))
      *settings->paths.directory_thumbnails = '\0';
   if (string_is_equal_fast(settings->paths.directory_playlist, "default", 7))
      *settings->paths.directory_playlist = '\0';
#ifdef HAVE_MENU

   if (string_is_equal_fast(settings->paths.directory_menu_content, "default", 7))
      *settings->paths.directory_menu_content = '\0';
   if (string_is_equal_fast(settings->paths.directory_menu_config, "default", 7))
      *settings->paths.directory_menu_config = '\0';
#endif
#ifdef HAVE_OVERLAY
   if (string_is_equal_fast(settings->paths.directory_overlay, "default", 7))
      *settings->paths.directory_overlay = '\0';
#endif
   if (string_is_equal_fast(settings->paths.directory_system, "default", 7))
      *settings->paths.directory_system = '\0';

   if (settings->floats.slowmotion_ratio < 1.0f)
   {
      configuration_set_float(settings, settings->floats.slowmotion_ratio, 1.0f);
   }

   /* Sanitize fastforward_ratio value - previously range was -1
    * and up (with 0 being skipped) */
   if (settings->floats.fastforward_ratio < 0.0f)
   {
      configuration_set_float(settings, settings->floats.fastforward_ratio, 0.0f);
   }

#ifdef HAVE_LAKKA
   settings->bools.ssh_enable       = path_file_exists(LAKKA_SSH_PATH);
   settings->bools.samba_enable     = path_file_exists(LAKKA_SAMBA_PATH);
   settings->bools.bluetooth_enable = path_file_exists(LAKKA_BLUETOOTH_PATH);
#endif

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_SAVE_PATH, NULL) &&
         config_get_path(conf, "savefile_directory", tmp_str, sizeof(tmp_str)))
   {
      if (string_is_equal_fast(tmp_str, "default", 7))
         dir_set(RARCH_DIR_SAVEFILE, g_defaults.dirs[DEFAULT_DIR_SRAM]);

      else if (path_is_directory(tmp_str))
      {
         global_t   *global = global_get_ptr();

         dir_set(RARCH_DIR_SAVEFILE, tmp_str);

         if (global)
         {
            strlcpy(global->name.savefile, tmp_str,
                  sizeof(global->name.savefile));
            fill_pathname_dir(global->name.savefile,
                  path_get(RARCH_PATH_BASENAME),
                  file_path_str(FILE_PATH_SRM_EXTENSION),
                  sizeof(global->name.savefile));
         }
      }
      else
         RARCH_WARN("savefile_directory is not a directory, ignoring ...\n");
   }

   if (!retroarch_override_setting_is_set(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL) &&
         config_get_path(conf, "savestate_directory", tmp_str, sizeof(tmp_str)))
   {
      if (string_is_equal_fast(tmp_str, "default", 7))
         dir_set(RARCH_DIR_SAVESTATE, g_defaults.dirs[DEFAULT_DIR_SAVESTATE]);
      else if (path_is_directory(tmp_str))
      {
         global_t   *global = global_get_ptr();

         dir_set(RARCH_DIR_SAVESTATE, tmp_str);

         if (global)
         {
            strlcpy(global->name.savestate, tmp_str,
                  sizeof(global->name.savestate));
            fill_pathname_dir(global->name.savestate,
                  path_get(RARCH_PATH_BASENAME),
                  file_path_str(FILE_PATH_STATE_EXTENSION),
                  sizeof(global->name.savestate));
         }
      }
      else
         RARCH_WARN("savestate_directory is not a directory, ignoring ...\n");
   }

   config_read_keybinds_conf(conf);

   ret = true;

   {
      const char *shader_ext = path_get_extension(settings->paths.path_shader);

      if (!string_is_empty(shader_ext))
      {
         for(i = FILE_PATH_CGP_EXTENSION; i <= FILE_PATH_SLANGP_EXTENSION; i++)
         {
            enum file_path_enum ext = (enum file_path_enum)(i);
            if(!strstr(file_path_str(ext), shader_ext))
               continue;

            if (check_shader_compatibility(ext))
               continue;

            RARCH_LOG("Incompatible shader for backend %s, clearing...\n",
                  settings->arrays.video_driver);
            settings->paths.path_shader[0] = '\0';
            break;
         }
      }
   }

end:
   if (conf)
      config_file_free(conf);
   if (bool_settings)
      free(bool_settings);
   if (int_settings)
      free(int_settings);
   if (uint_settings)
      free(uint_settings);
   if (float_settings)
      free(float_settings);
   if (array_settings)
      free(array_settings);
   if (path_settings)
      free(path_settings);
   return ret;
}

/**
 * config_load_override:
 *
 * Tries to append game-specific and core-specific configuration.
 * These settings will always have precedence, thus this feature
 * can be used to enforce overrides.
 *
 * This function only has an effect if a game-specific or core-specific
 * configuration file exists at respective locations.
 *
 * core-specific: $CONFIG_DIR/$CORE_NAME/$CORE_NAME.cfg 
 * fallback:      $CURRENT_CFG_LOCATION/$CORE_NAME/$CORE_NAME.cfg
 *
 * game-specific: $CONFIG_DIR/$CORE_NAME/$ROM_NAME.cfg 
 * fallback:      $CURRENT_CFG_LOCATION/$CORE_NAME/$GAME_NAME.cfg
 *
 * Returns: false if there was an error or no action was performed.
 *
 */
bool config_load_override(void)
{
   char buf[PATH_MAX_LENGTH];
   char config_directory[PATH_MAX_LENGTH];
   char core_path[PATH_MAX_LENGTH];
   char game_path[PATH_MAX_LENGTH];
   config_file_t *new_conf                = NULL;
   const char *core_name                  = NULL;
   const char *game_name                  = NULL;
   bool should_append                     = false;
   rarch_system_info_t *system            = runloop_get_system_info();

   if (system)
      core_name = system->info.library_name;

   game_name = path_basename(path_get(RARCH_PATH_BASENAME));

   if (string_is_empty(core_name) || string_is_empty(game_name))
      return false;

   config_directory[0] = core_path[0] = game_path[0] = '\0';

   fill_pathname_application_special(config_directory, sizeof(config_directory),
         APPLICATION_SPECIAL_DIRECTORY_CONFIG);

   /* Concatenate strings into full paths for core_path, game_path */
   fill_pathname_join_special_ext(game_path,
         config_directory, core_name,
         game_name,
         file_path_str(FILE_PATH_CONFIG_EXTENSION),
         sizeof(game_path));

   fill_pathname_join_special_ext(core_path,
         config_directory, core_name,
         core_name,
         file_path_str(FILE_PATH_CONFIG_EXTENSION),
         sizeof(core_path));

   /* Create a new config file from core_path */
   new_conf = config_file_new(core_path);

   /* If a core override exists, add its location to append_config_path */
   if (new_conf)
   {
      RARCH_LOG("[overrides] core-specific overrides found at %s.\n", core_path);

      config_file_free(new_conf);
      path_set(RARCH_PATH_CONFIG_APPEND, core_path);

      should_append = true;
   }
   else
      RARCH_LOG("[overrides] no core-specific overrides found at %s.\n", core_path);

   /* Create a new config file from game_path */
   new_conf = config_file_new(game_path);

   /* If a game override exists, add it's location to append_config_path */
   if (new_conf)
   {
      char temp_path[PATH_MAX_LENGTH];

      temp_path[0] = '\0';

      config_file_free(new_conf);

      RARCH_LOG("[overrides] game-specific overrides found at %s.\n", game_path);

      if (should_append)
      {
         strlcpy(temp_path, path_get(RARCH_PATH_CONFIG_APPEND), sizeof(temp_path));
         strlcat(temp_path, "|", sizeof(temp_path));
         strlcat(temp_path, game_path, sizeof(temp_path));
      }
      else
         strlcpy(temp_path, game_path, sizeof(temp_path));

      path_set(RARCH_PATH_CONFIG_APPEND, temp_path);

      should_append = true;
   }
   else
      RARCH_LOG("[overrides] no game-specific overrides found at %s.\n", game_path);

   if (!should_append)
      return false;

   /* Re-load the configuration with any overrides that might have been found */
   buf[0] = '\0';

   /* Store the libretro_path we're using since it will be 
    * overwritten by the override when reloading. */
   strlcpy(buf, path_get(RARCH_PATH_CORE), sizeof(buf));

   /* Toggle has_save_path to false so it resets */
   retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL);
   retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_SAVE_PATH,  NULL);

   if (!config_load_file(path_get(RARCH_PATH_CONFIG), false, config_get_ptr()))
      return false;

   /* Restore the libretro_path we're using
    * since it will be overwritten by the override when reloading. */
   path_set(RARCH_PATH_CORE, buf);
   runloop_msg_queue_push("Configuration override loaded.", 1, 100, true);

   /* Reset save paths. */
   retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL);
   retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_SAVE_PATH, NULL);

   path_clear(RARCH_PATH_CONFIG_APPEND);

   return true;
}

/**
 * config_unload_override:
 *
 * Unloads configuration overrides if overrides are active.
 *
 *
 * Returns: false if there was an error.
 */
bool config_unload_override(void)
{
   path_clear(RARCH_PATH_CONFIG_APPEND);

   /* Toggle has_save_path to false so it resets */
   retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL);
   retroarch_override_setting_unset(RARCH_OVERRIDE_SETTING_SAVE_PATH,  NULL);

   if (!config_load_file(path_get(RARCH_PATH_CONFIG), false, config_get_ptr()))
      return false;

   RARCH_LOG("[overrides] configuration overrides unloaded, original configuration restored.\n");

   /* Reset save paths */
   retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_STATE_PATH, NULL);
   retroarch_override_setting_set(RARCH_OVERRIDE_SETTING_SAVE_PATH, NULL);

   return true;
}

/**
 * config_load_remap:
 *
 * Tries to append game-specific and core-specific remap files.
 *
 * This function only has an effect if a game-specific or core-specific
 * configuration file exists at respective locations.
 *
 * core-specific: $REMAP_DIR/$CORE_NAME/$CORE_NAME.cfg
 * game-specific: $REMAP_DIR/$CORE_NAME/$GAME_NAME.cfg
 *
 * Returns: false if there was an error or no action was performed.
 */
bool config_load_remap(void)
{
   char remap_directory[PATH_MAX_LENGTH];    /* path to the directory containing retroarch.cfg (prefix)    */
   char core_path[PATH_MAX_LENGTH];          /* final path for core-specific configuration (prefix+suffix) */
   char game_path[PATH_MAX_LENGTH];          /* final path for game-specific configuration (prefix+suffix) */
   config_file_t *new_conf                = NULL;
   const char *core_name                  = NULL;
   const char *game_name                  = NULL;
   settings_t *settings                   = config_get_ptr();
   rarch_system_info_t *system            = runloop_get_system_info();

   if (system)
      core_name = system->info.library_name;

   game_name = path_basename(path_get(RARCH_PATH_BASENAME));

   if (string_is_empty(core_name) || string_is_empty(game_name))
      return false;

   /* Remap directory: remap_directory.
    * Try remap directory setting, no fallbacks defined */
   if (string_is_empty(settings->paths.directory_input_remapping))
      return false;

   remap_directory[0] = core_path[0] = game_path[0] = '\0';

   strlcpy(remap_directory,
         settings->paths.directory_input_remapping,
         sizeof(remap_directory));
   RARCH_LOG("Remaps: remap directory: %s\n", remap_directory);

   /* Concatenate strings into full paths for core_path, game_path */
   fill_pathname_join_special_ext(core_path,
         remap_directory, core_name,
         core_name,
         file_path_str(FILE_PATH_REMAP_EXTENSION),
         sizeof(core_path));

   fill_pathname_join_special_ext(game_path,
         remap_directory, core_name,
         game_name,
         file_path_str(FILE_PATH_REMAP_EXTENSION),
         sizeof(game_path));

   /* Create a new config file from game_path */
   new_conf = config_file_new(game_path);

   /* If a game remap file exists, load it. */
   if (new_conf)
   {
      RARCH_LOG("Remaps: game-specific remap found at %s.\n", game_path);
      if (input_remapping_load_file(new_conf, game_path))
      {
         runloop_msg_queue_push("Game remap file loaded.", 1, 100, true);
         return true;
      }
   }
   else
   {
      RARCH_LOG("Remaps: no game-specific remap found at %s.\n", game_path);
      input_remapping_set_defaults();
   }

   /* Create a new config file from core_path */
   new_conf = config_file_new(core_path);

   /* If a core remap file exists, load it. */
   if (new_conf)
   {
      RARCH_LOG("Remaps: core-specific remap found at %s.\n", core_path);
      if (input_remapping_load_file(new_conf, core_path))
      {
         runloop_msg_queue_push("Core remap file loaded.", 1, 100, true);
         return true;
      }
   }
   else
   {
      RARCH_LOG("Remaps: no core-specific remap found at %s.\n", core_path);
      input_remapping_set_defaults();
   }

   new_conf = NULL;

   return false;
}

/**
 * config_load_shader_preset:
 *
 * Tries to append game-specific and core-specific shader presets.
 *
 * This function only has an effect if a game-specific or core-specific
 * configuration file exists at respective locations.
 *
 * core-specific: $SHADER_DIR/presets/$CORE_NAME/$CORE_NAME.cfg
 * game-specific: $SHADER_DIR/presets/$CORE_NAME/$GAME_NAME.cfg
 *
 * Returns: false if there was an error or no action was performed.
 */
bool config_load_shader_preset(void)
{
   unsigned idx;
   char shader_directory[PATH_MAX_LENGTH];    /* path to the directory containing retroarch.cfg (prefix)    */
   char core_path[PATH_MAX_LENGTH];          /* final path for core-specific configuration (prefix+suffix) */
   char game_path[PATH_MAX_LENGTH];          /* final path for game-specific configuration (prefix+suffix) */
   const char *core_name                  = NULL;
   const char *game_name                  = NULL;
   settings_t *settings                   = config_get_ptr();
   rarch_system_info_t *system            = runloop_get_system_info();

   if (system)
      core_name = system->info.library_name;

   game_name = path_basename(path_get(RARCH_PATH_BASENAME));

   if (string_is_empty(core_name) || string_is_empty(game_name))
      return false;

   /* Shader directory: shader_directory.
    * Try shader directory setting, no fallbacks defined */
   if (string_is_empty(settings->paths.directory_video_shader))
      return false;

   shader_directory[0] = core_path[0] = game_path[0] = '\0';

   fill_pathname_join (shader_directory, settings->paths.directory_video_shader,
       "presets", sizeof(shader_directory));

   RARCH_LOG("Shaders: preset directory: %s\n", shader_directory);

   for(idx = FILE_PATH_CGP_EXTENSION; idx <= FILE_PATH_SLANGP_EXTENSION; idx++)
   {
      config_file_t *new_conf = NULL;

      if (!check_shader_compatibility((enum file_path_enum)(idx)))
         continue;
      /* Concatenate strings into full paths for core_path, game_path */
      fill_pathname_join_special_ext(core_path,
            shader_directory, core_name,
            core_name,
            file_path_str((enum file_path_enum)(idx)),
            sizeof(core_path));

      fill_pathname_join_special_ext(game_path,
            shader_directory, core_name,
            game_name,
            file_path_str((enum file_path_enum)(idx)),
            sizeof(game_path));

      /* Create a new config file from game_path */
      new_conf = config_file_new(game_path);

      if (!new_conf)
      {
         RARCH_LOG("Shaders: no game-specific preset found at %s.\n", game_path);
         continue;
      }

      /* Game shader preset exists, load it. */
      RARCH_LOG("Shaders: game-specific shader preset found at %s.\n", game_path);

      path_set(RARCH_PATH_DEFAULT_SHADER_PRESET, settings->paths.path_shader);
      strlcpy(settings->paths.path_shader, game_path, sizeof(settings->paths.path_shader));
      config_file_free(new_conf);
      return true;
   }

   for(idx = FILE_PATH_CGP_EXTENSION; idx <= FILE_PATH_SLANGP_EXTENSION; idx++)
   {
      config_file_t *new_conf = NULL;

      if (!check_shader_compatibility((enum file_path_enum)(idx)))
         continue;
      /* Concatenate strings into full paths for core_path, game_path */
      fill_pathname_join_special_ext(core_path,
            shader_directory, core_name,
            core_name,
            file_path_str((enum file_path_enum)(idx)),
            sizeof(core_path));

      /* Create a new config file from core_path */
      new_conf = config_file_new(core_path);

      if (!new_conf)
      {
         RARCH_LOG("Shaders: no core-specific preset found at %s.\n", core_path);
         continue;
      }

      /* Core shader preset exists, load it. */
      RARCH_LOG("Shaders: core-specific shader preset found at %s.\n", core_path);
      path_set(RARCH_PATH_DEFAULT_SHADER_PRESET, settings->paths.path_shader);
      strlcpy(settings->paths.path_shader, core_path, sizeof(settings->paths.path_shader));
      config_file_free(new_conf);
      return true;
   }
   return false;
}

static void parse_config_file(void)
{
   if (path_is_empty(RARCH_PATH_CONFIG))
   {
      RARCH_LOG("[Config]: Loading default config.\n");
      if (!path_is_empty(RARCH_PATH_CONFIG))
         RARCH_LOG("[Config]: found default config: %s.\n", path_get(RARCH_PATH_CONFIG));
   }

   RARCH_LOG("[Config]: loading config from: %s.\n", path_get(RARCH_PATH_CONFIG));
   if (config_load_file(path_get(RARCH_PATH_CONFIG), false, config_get_ptr()))
      return;

   RARCH_ERR("[Config]: couldn't find config at path: \"%s\"\n", path_get(RARCH_PATH_CONFIG));
}



static void save_keybind_key(config_file_t *conf, const char *prefix,
      const char *base, const struct retro_keybind *bind)
{
   char key[64];
   char btn[64];

   key[0] = btn[0] = '\0';

   fill_pathname_join_delim(key, prefix, base, '_', sizeof(key));

   input_keymaps_translate_rk_to_str(bind->key, btn, sizeof(btn));
   config_set_string(conf, key, btn);
}

static void save_keybind_hat(config_file_t *conf, const char *key,
      const struct retro_keybind *bind)
{
   char config[16];
   unsigned hat     = (unsigned)GET_HAT(bind->joykey);
   const char *dir  = NULL;

   config[0]        = '\0';

   switch (GET_HAT_DIR(bind->joykey))
   {
      case HAT_UP_MASK:
         dir = "up";
         break;

      case HAT_DOWN_MASK:
         dir = "down";
         break;

      case HAT_LEFT_MASK:
         dir = "left";
         break;

      case HAT_RIGHT_MASK:
         dir = "right";
         break;

      default:
         retro_assert(0);
         break;
   }

   snprintf(config, sizeof(config), "h%u%s", hat, dir);
   config_set_string(conf, key, config);
}

static void save_keybind_joykey(config_file_t *conf, const char *prefix,
      const char *base, const struct retro_keybind *bind, bool save_empty)
{
   char key[64];

   key[0] = '\0';

   fill_pathname_join_delim_concat(key, prefix,
         base, '_', "_btn", sizeof(key));

   if (bind->joykey == NO_BTN)
   {
       if (save_empty)
         config_set_string(conf, key, file_path_str(FILE_PATH_NUL));
   }
   else if (GET_HAT_DIR(bind->joykey))
      save_keybind_hat(conf, key, bind);
   else
      config_set_uint64(conf, key, bind->joykey);
}

static void save_keybind_axis(config_file_t *conf, const char *prefix,
      const char *base, const struct retro_keybind *bind, bool save_empty)
{
   char key[64];
   unsigned axis   = 0;
   char dir        = '\0';

   key[0] = '\0';

   fill_pathname_join_delim_concat(key,
         prefix, base, '_',
         "_axis",
         sizeof(key));

   if (bind->joyaxis == AXIS_NONE)
   {
      if (save_empty)
         config_set_string(conf, key, file_path_str(FILE_PATH_NUL));
   }
   else if (AXIS_NEG_GET(bind->joyaxis) != AXIS_DIR_NONE)
   {
      dir = '-';
      axis = AXIS_NEG_GET(bind->joyaxis);
   }
   else if (AXIS_POS_GET(bind->joyaxis) != AXIS_DIR_NONE)
   {
      dir = '+';
      axis = AXIS_POS_GET(bind->joyaxis);
   }

   if (dir)
   {
      char config[16];

      config[0] = '\0';

      snprintf(config, sizeof(config), "%c%u", dir, axis);
      config_set_string(conf, key, config);
   }
}

/**
 * save_keybind:
 * @conf               : pointer to config file object
 * @prefix             : prefix name of keybind
 * @base               : base name   of keybind
 * @bind               : pointer to key binding object
 * @kb                 : save keyboard binds
 *
 * Save a key binding to the config file.
 */
static void save_keybind(config_file_t *conf, const char *prefix,
      const char *base, const struct retro_keybind *bind,
      bool save_kb, bool save_empty)
{
   if (!bind->valid)
      return;
   if (save_kb)
      save_keybind_key(conf, prefix, base, bind);
   save_keybind_joykey(conf, prefix, base, bind, save_empty);
   save_keybind_axis(conf, prefix, base, bind, save_empty);
}

/**
 * save_keybinds_user:
 * @conf               : pointer to config file object
 * @user               : user number
 *
 * Save the current keybinds of a user (@user) to the config file (@conf).
 */
static void save_keybinds_user(config_file_t *conf, unsigned user)
{
   unsigned i = 0;

   for (i = 0; input_config_bind_map_get_valid(i); i++)
   {
      const char *prefix = input_config_get_prefix(user,
            input_config_bind_map_get_meta(i));

      if (prefix)
         save_keybind(conf, prefix, input_config_bind_map_get_base(i),
               &input_config_binds[user][i], true, true);
   }
}

/**
 * config_load:
 *
 * Loads a config file and reads all the values into memory.
 *
 */
void config_load(void)
{
   /* Flush out some states that could have been 
    * set by core environment variables */
   core_unset_input_descriptors();

   if (rarch_ctl(RARCH_CTL_IS_BLOCK_CONFIG_READ, NULL))
      return;

   config_set_defaults();
   parse_config_file();
}

#if 0
/**
 * config_save_keybinds_file:
 * @path            : Path that shall be written to.
 *
 * Writes a keybinds config file to disk.
 *
 * Returns: true (1) on success, otherwise returns false (0).
 **/
static bool config_save_keybinds_file(const char *path)
{
   unsigned          i = 0;
   bool            ret = false;
   config_file_t *conf = config_file_new(path);

   if (!conf)
      conf = config_file_new(NULL);

   if (!conf)
      return false;

   RARCH_LOG("Saving keybinds config at path: \"%s\"\n", path);

   for (i = 0; i < MAX_USERS; i++)
      save_keybinds_user(conf, i);

   ret = config_file_write(conf, path);
   config_file_free(conf);
   return ret;
}
#endif


/**
 * config_save_autoconf_profile:
 * @path            : Path that shall be written to.
 * @user              : Controller number to save
 * Writes a controller autoconf file to disk.
 **/
bool config_save_autoconf_profile(const char *path, unsigned user)
{
   unsigned i;
   char buf[PATH_MAX_LENGTH];
   char autoconf_file[PATH_MAX_LENGTH];
   int32_t pid_user                     = 0;
   int32_t vid_user                     = 0;
   bool ret                             = false;
   config_file_t *conf                  = NULL;
   settings_t *settings                 = config_get_ptr();
   const char *autoconf_dir             = settings->paths.directory_autoconfig;
   const char *joypad_ident             = settings->arrays.input_joypad_driver;

   buf[0] = autoconf_file[0]            = '\0';

   fill_pathname_join(buf, autoconf_dir,
         joypad_ident, sizeof(buf));

   if(path_is_directory(buf))
   {
      char buf_new[PATH_MAX_LENGTH];

      buf_new[0] = '\0';

      fill_pathname_join(buf_new, buf,
            path, sizeof(buf_new));
      fill_pathname_noext(autoconf_file, buf_new,
            file_path_str(FILE_PATH_CONFIG_EXTENSION),
            sizeof(autoconf_file));
   }
   else
   {
      fill_pathname_join(buf, autoconf_dir,
            path, sizeof(buf));
      fill_pathname_noext(autoconf_file, buf,
            file_path_str(FILE_PATH_CONFIG_EXTENSION),
            sizeof(autoconf_file));
   }

   conf  = config_file_new(autoconf_file);

   if (!conf)
   {
      conf = config_file_new(NULL);
      if (!conf)
         return false;
   }

   config_set_string(conf, "input_driver", joypad_ident);
   config_set_string(conf, "input_device",
         input_config_get_device_name(user));

   pid_user = input_config_get_pid(user);
   vid_user = input_config_get_vid(user);

   if(pid_user && vid_user)
   {
      config_set_int(conf, "input_vendor_id",
            vid_user);
      config_set_int(conf, "input_product_id",
            pid_user);
   }

   for (i = 0; i < RARCH_FIRST_META_KEY; i++)
   {
      save_keybind(conf, "input", input_config_bind_map_get_base(i),
            &input_config_binds[user][i], false, false);
   }

   ret = config_file_write(conf, autoconf_file);

   config_file_free(conf);

   return ret;
}


/**
 * config_save_file:
 * @path            : Path that shall be written to.
 *
 * Writes a config file to disk.
 *
 * Returns: true (1) on success, otherwise returns false (0).
 **/
bool config_save_file(const char *path)
{
   float msg_color;
   unsigned i                                        = 0;
   bool ret                                          = false;
   struct config_bool_setting     *bool_settings     = NULL;
   struct config_int_setting     *int_settings       = NULL;
   struct config_uint_setting     *uint_settings     = NULL;
   struct config_float_setting     *float_settings   = NULL;
   struct config_array_setting     *array_settings   = NULL;
   struct config_path_setting     *path_settings     = NULL;
   config_file_t                              *conf  = config_file_new(path);
   settings_t                              *settings = config_get_ptr();
   int bool_settings_size                            = sizeof(settings->bools) / sizeof(settings->bools.placeholder);
   int float_settings_size                           = sizeof(settings->floats)/ sizeof(settings->floats.placeholder);
   int int_settings_size                             = sizeof(settings->ints)  / sizeof(settings->ints.placeholder);
   int uint_settings_size                            = sizeof(settings->uints) / sizeof(settings->uints.placeholder);
   int array_settings_size                           = sizeof(settings->arrays)/ sizeof(settings->arrays.placeholder);
   int path_settings_size                            = sizeof(settings->paths) / sizeof(settings->paths.placeholder);

   if (!conf)
      conf = config_file_new(NULL);

   if (!conf || rarch_ctl(RARCH_CTL_IS_OVERRIDES_ACTIVE, NULL))
   {
      if (conf)
         config_file_free(conf);
      return false;
   }

   bool_settings   = populate_settings_bool  (settings, &bool_settings_size);
   int_settings    = populate_settings_int   (settings, &int_settings_size);
   uint_settings   = populate_settings_uint  (settings, &uint_settings_size);
   float_settings  = populate_settings_float (settings, &float_settings_size);
   array_settings  = populate_settings_array (settings, &array_settings_size);
   path_settings   = populate_settings_path  (settings, &path_settings_size);

   /* Path settings */
   if (path_settings && (path_settings_size > 0))
   {
      for (i = 0; i < (unsigned)path_settings_size; i++)
      {
         const char *value = path_settings[i].ptr;

         if (path_settings[i].def_enable && string_is_empty(path_settings[i].ptr))
            value = "default";

         config_set_path(conf, path_settings[i].ident, value);
      }

      free(path_settings);
   }

#ifdef HAVE_MENU
   config_set_path(conf, "xmb_font",
         !string_is_empty(settings->paths.path_menu_xmb_font) 
         ? settings->paths.path_menu_xmb_font : "");
#endif

   /* String settings  */
   if (array_settings && (array_settings_size > 0))
   {
      for (i = 0; i < (unsigned)array_settings_size; i++)
         config_set_string(conf,
               array_settings[i].ident,
               array_settings[i].ptr);

      free(array_settings);
   }

   /* Float settings  */
   if (float_settings && (float_settings_size > 0))
   {
      for (i = 0; i < (unsigned)float_settings_size; i++)
         config_set_float(conf,
               float_settings[i].ident,
               *float_settings[i].ptr);

      free(float_settings);
   }

   /* Integer settings */
   if (int_settings && (int_settings_size > 0))
   {
      for (i = 0; i < (unsigned)int_settings_size; i++)
         config_set_int(conf,
               int_settings[i].ident,
               *int_settings[i].ptr);

      free(int_settings);
   }

   if (uint_settings && (uint_settings_size > 0))
   {
      for (i = 0; i < (unsigned)uint_settings_size; i++)
         config_set_int(conf,
               uint_settings[i].ident,
               *uint_settings[i].ptr);

      free(uint_settings);
   }

   for (i = 0; i < MAX_USERS; i++)
   {
      char cfg[64];

      cfg[0] = '\0';

      snprintf(cfg, sizeof(cfg), "input_device_p%u", i + 1);
      config_set_int(conf, cfg, settings->uints.input_device[i]);
      snprintf(cfg, sizeof(cfg), "input_player%u_joypad_index", i + 1);
      config_set_int(conf, cfg, settings->uints.input_joypad_map[i]);
      snprintf(cfg, sizeof(cfg), "input_libretro_device_p%u", i + 1);
      config_set_int(conf, cfg, input_config_get_device(i));
      snprintf(cfg, sizeof(cfg), "input_player%u_analog_dpad_mode", i + 1);
      config_set_int(conf, cfg, settings->uints.input_analog_dpad_mode[i]);
   }

   /* Boolean settings */
   if (bool_settings && (bool_settings_size > 0))
   {
      for (i = 0; i < (unsigned)bool_settings_size; i++)
         config_set_bool(conf, bool_settings[i].ident,
               *bool_settings[i].ptr);

      free(bool_settings);
   }

#ifdef HAVE_NETWORKGAMEPAD
   for (i = 0; i < MAX_USERS; i++)
   {
      char tmp[64];

      tmp[0] = '\0';

      snprintf(tmp, sizeof(tmp), "network_remote_enable_user_p%u", i + 1);
      config_set_bool(conf, tmp, settings->bools.network_remote_enable_user[i]);
   }
#endif

   config_set_bool(conf, "log_verbosity",
         verbosity_is_enabled());
   config_set_bool(conf, "perfcnt_enable",
         rarch_ctl(RARCH_CTL_IS_PERFCNT_ENABLE, NULL));

   msg_color = (((int)(settings->floats.video_msg_color_r * 255.0f) & 0xff) << 16) +
               (((int)(settings->floats.video_msg_color_g * 255.0f) & 0xff) <<  8) +
               (((int)(settings->floats.video_msg_color_b * 255.0f) & 0xff));

   /* Hexadecimal settings */
   config_set_hex(conf, "video_message_color", msg_color);
#ifdef HAVE_MENU
   config_set_hex(conf, "menu_entry_normal_color",
         settings->uints.menu_entry_normal_color);
   config_set_hex(conf, "menu_entry_hover_color",
         settings->uints.menu_entry_hover_color);
   config_set_hex(conf, "menu_title_color",
         settings->uints.menu_title_color);
#endif


   video_driver_save_settings(conf);

#ifdef HAVE_LAKKA
   if (settings->bools.ssh_enable)
      fclose(fopen(LAKKA_SSH_PATH, "w"));
   else
      remove(LAKKA_SSH_PATH);
   if (settings->bools.samba_enable)
      fclose(fopen(LAKKA_SAMBA_PATH, "w"));
   else
      remove(LAKKA_SAMBA_PATH);
   if (settings->bools.bluetooth_enable)
      fclose(fopen(LAKKA_BLUETOOTH_PATH, "w"));
   else
      remove(LAKKA_BLUETOOTH_PATH);
#endif

   for (i = 0; i < MAX_USERS; i++)
      save_keybinds_user(conf, i);

   ret = config_file_write(conf, path);
   config_file_free(conf);

   return ret;
}

/**
 * config_save_overrides:
 * @path            : Path that shall be written to.
 *
 * Writes a config file override to disk.
 *
 * Returns: true (1) on success, otherwise returns false (0).
 **/
bool config_save_overrides(int override_type)
{
   char config_directory[PATH_MAX_LENGTH];
   char override_directory[PATH_MAX_LENGTH];
   char core_path[PATH_MAX_LENGTH];
   char game_path[PATH_MAX_LENGTH];
   int tmp_i                                   = 0;
   unsigned i                                  = 0;
   bool ret                                    = false;
   const char *core_name                       = NULL;
   const char *game_name                       = NULL;
   config_file_t *conf                         = NULL;
   settings_t *settings                        = NULL;
   struct config_bool_setting *bool_settings   = NULL;
   struct config_bool_setting *bool_overrides  = NULL;
   struct config_int_setting *int_settings     = NULL;
   struct config_uint_setting *uint_settings   = NULL;
   struct config_int_setting *int_overrides    = NULL;
   struct config_uint_setting *uint_overrides  = NULL;
   struct config_float_setting *float_settings = NULL;
   struct config_float_setting *float_overrides= NULL;
   struct config_array_setting *array_settings = NULL;
   struct config_array_setting *array_overrides= NULL;
   struct config_path_setting *path_settings   = NULL;
   struct config_path_setting *path_overrides  = NULL;
   settings_t *overrides                       = config_get_ptr();
   int bool_settings_size                      = sizeof(settings->bools)  / sizeof(settings->bools.placeholder);
   int float_settings_size                     = sizeof(settings->floats) / sizeof(settings->floats.placeholder);
   int int_settings_size                       = sizeof(settings->ints)   / sizeof(settings->ints.placeholder);
   int uint_settings_size                      = sizeof(settings->uints)  / sizeof(settings->uints.placeholder);
   int array_settings_size                     = sizeof(settings->arrays) / sizeof(settings->arrays.placeholder);
   int path_settings_size                      = sizeof(settings->paths)  / sizeof(settings->paths.placeholder);
   rarch_system_info_t *system                 = runloop_get_system_info();

   if (system)
      core_name = system->info.library_name;

   game_name = path_basename(path_get(RARCH_PATH_BASENAME));

   if (string_is_empty(core_name) || string_is_empty(game_name))
      return false;

   settings  = (settings_t*)calloc(1, sizeof(settings_t));

   config_directory[0] = override_directory[0] = core_path[0] = game_path[0] = '\0';

   fill_pathname_application_special(config_directory, sizeof(config_directory),
         APPLICATION_SPECIAL_DIRECTORY_CONFIG);

   fill_pathname_join(override_directory, config_directory, core_name, 
      sizeof(override_directory));

   if(!path_file_exists(override_directory))
       path_mkdir(override_directory);

   /* Concatenate strings into full paths for core_path, game_path */
   fill_pathname_join_special_ext(game_path,
         config_directory, core_name,
         game_name,
         file_path_str(FILE_PATH_CONFIG_EXTENSION),
         sizeof(game_path));

   fill_pathname_join_special_ext(core_path,
         config_directory, core_name,
         core_name,
         file_path_str(FILE_PATH_CONFIG_EXTENSION),
         sizeof(core_path));

   if (!conf)
      conf = config_file_new(NULL);

   /* Load the original config file in memory */
   config_load_file(path_get(RARCH_PATH_CONFIG), false, settings);

   bool_settings       = populate_settings_bool(settings,   &bool_settings_size);
   tmp_i               = sizeof(settings->bools) / sizeof(settings->bools.placeholder);
   bool_overrides      = populate_settings_bool(overrides,  &tmp_i);

   int_settings        = populate_settings_int(settings,    &int_settings_size);
   tmp_i               = sizeof(settings->ints) / sizeof(settings->ints.placeholder);
   int_overrides       = populate_settings_int (overrides,  &tmp_i);

   uint_settings       = populate_settings_uint(settings,    &uint_settings_size);
   tmp_i               = sizeof(settings->uints) / sizeof(settings->uints.placeholder);
   uint_overrides      = populate_settings_uint (overrides,  &tmp_i);

   float_settings      = populate_settings_float(settings,  &float_settings_size);
   tmp_i               = sizeof(settings->floats) / sizeof(settings->floats.placeholder);
   float_overrides     = populate_settings_float(overrides, &tmp_i);

   array_settings      = populate_settings_array(settings,  &array_settings_size);
   tmp_i               = sizeof(settings->arrays)   / sizeof(settings->arrays.placeholder);
   array_overrides     = populate_settings_array (overrides, &tmp_i);

   path_settings       = populate_settings_path(settings, &path_settings_size);
   tmp_i               = sizeof(settings->paths)   / sizeof(settings->paths.placeholder);
   path_overrides      = populate_settings_path (overrides, &tmp_i);

   RARCH_LOG("[overrides] looking for changed settings... \n");

   if (conf)
   {
      for (i = 0; i < (unsigned)bool_settings_size; i++)
      {
         if ((*bool_settings[i].ptr) != (*bool_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%d\n", 
                  bool_settings[i].ident, (*bool_settings[i].ptr));
            RARCH_LOG("   override: %s=%d\n", 
                  bool_overrides[i].ident, (*bool_overrides[i].ptr));
            config_set_bool(conf, bool_overrides[i].ident,
                  (*bool_overrides[i].ptr));
         }
      }
      for (i = 0; i < (unsigned)int_settings_size; i++)
      {
         if ((*int_settings[i].ptr) != (*int_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%d\n", 
                  int_settings[i].ident, (*int_settings[i].ptr));
            RARCH_LOG("   override: %s=%d\n", 
                  int_overrides[i].ident, (*int_overrides[i].ptr));
            config_set_int(conf, int_overrides[i].ident,
                  (*int_overrides[i].ptr));
         }
      }
      for (i = 0; i < (unsigned)uint_settings_size; i++)
      {
         if ((*uint_settings[i].ptr) != (*uint_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%d\n", 
                  uint_settings[i].ident, (*uint_settings[i].ptr));
            RARCH_LOG("   override: %s=%d\n", 
                  uint_overrides[i].ident, (*uint_overrides[i].ptr));
            config_set_int(conf, uint_overrides[i].ident,
                  (*uint_overrides[i].ptr));
         }
      }
      for (i = 0; i < (unsigned)float_settings_size; i++)
      {
         if ((*float_settings[i].ptr) != (*float_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%f\n", 
                  float_settings[i].ident, *float_settings[i].ptr);
            RARCH_LOG("   override: %s=%f\n", 
                  float_overrides[i].ident, *float_overrides[i].ptr);
            config_set_float(conf, float_overrides[i].ident,
                  *float_overrides[i].ptr);
         }
      }

      for (i = 0; i < (unsigned)array_settings_size; i++)
      {
         if (!string_is_equal(array_settings[i].ptr, array_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%s\n", 
                  array_settings[i].ident, array_settings[i].ptr);
            RARCH_LOG("   override: %s=%s\n", 
                  array_overrides[i].ident, array_overrides[i].ptr);
            config_set_string(conf, array_overrides[i].ident,
                  array_overrides[i].ptr);
         }
      }

      for (i = 0; i < (unsigned)path_settings_size; i++)
      {
         if (!string_is_equal(path_settings[i].ptr, path_overrides[i].ptr))
         {
            RARCH_LOG("   original: %s=%s\n", 
                  path_settings[i].ident, path_settings[i].ptr);
            RARCH_LOG("   override: %s=%s\n", 
                  path_overrides[i].ident, path_overrides[i].ptr);
            config_set_path(conf, path_overrides[i].ident,
                  path_overrides[i].ptr);
         }
      }

      for (i = 0; i < MAX_USERS; i++)
      {
         char cfg[64];

         cfg[0] = '\0';
         if (settings->uints.input_device[i] != overrides->uints.input_device[i])
         {
            snprintf(cfg, sizeof(cfg), "input_device_p%u", i + 1);
            config_set_int(conf, cfg, overrides->uints.input_device[i]);
         }
         if (settings->uints.input_joypad_map[i] != overrides->uints.input_joypad_map[i])
         {
            snprintf(cfg, sizeof(cfg), "input_player%u_joypad_index", i + 1);
            config_set_int(conf, cfg, overrides->uints.input_joypad_map[i]);
         }
         if (input_config_get_device(i) != overrides->uints.input_libretro_device[i])
         {
            snprintf(cfg, sizeof(cfg), "input_libretro_device_p%u", i + 1);
            config_set_int(conf, cfg, overrides->uints.input_libretro_device[i]);
         }
         if (settings->uints.input_analog_dpad_mode[i] != overrides->uints.input_analog_dpad_mode[i])
         {
            snprintf(cfg, sizeof(cfg), "input_player%u_analog_dpad_mode", i + 1);
            config_set_int(conf, cfg, overrides->uints.input_analog_dpad_mode[i]);
         }
      }

      ret = false;

      switch (override_type)
      {
         case OVERRIDE_CORE:
            /* Create a new config file from core_path */
            RARCH_LOG ("[overrides] path %s\n", core_path);
            ret = config_file_write(conf, core_path);
            break;
         case OVERRIDE_GAME:
            /* Create a new config file from core_path */
            RARCH_LOG ("[overrides] path %s\n", game_path);
            ret = config_file_write(conf, game_path);
            break;
         default:
            break;
      }

      config_file_free(conf);
   }

   if (bool_settings)
      free(bool_settings);
   if (bool_overrides)
      free(bool_overrides);
   if (int_settings)
      free(int_settings);
   if (uint_settings)
      free(uint_settings);
   if (int_overrides)
      free(int_overrides);
   if (uint_overrides)
      free(uint_overrides);
   if (float_settings)
      free(float_settings);
   if (float_overrides)
      free(float_overrides);
   if (array_settings)
      free(array_settings);
   if (array_overrides)
      free(array_overrides);
   if (path_settings)
      free(path_settings);
   if (path_overrides)
      free(path_overrides);
   free(settings);

   return ret;
}

/* Replaces currently loaded configuration file with
 * another one. Will load a dummy core to flush state
 * properly. */
bool config_replace(bool config_save_on_exit, char *path)
{
   content_ctx_info_t content_info = {0};

   if (!path)
      return false;

   /* If config file to be replaced is the same as the
    * current config file, exit. */
   if (string_is_equal(path, path_get(RARCH_PATH_CONFIG)))
      return false;

   if (config_save_on_exit && !path_is_empty(RARCH_PATH_CONFIG))
      config_save_file(path_get(RARCH_PATH_CONFIG));

   path_set(RARCH_PATH_CONFIG, path);

   rarch_ctl(RARCH_CTL_UNSET_BLOCK_CONFIG_READ, NULL);

   /* Load core in new config. */
   path_clear(RARCH_PATH_CORE);

   if (!task_push_start_dummy_core(&content_info))
      return false;

   return true;
}
