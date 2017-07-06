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

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "camera_driver.h"

#include "../src/configuration.h"
#include "../src/driver.h"
#include "../src/retroarch.h"
#include "../src/list_special.h"
#include "../src/verbosity.h"

static const camera_driver_t *camera_drivers[] = {
   &camera_null,
   NULL,
};

static struct retro_camera_callback camera_cb;
static const camera_driver_t *camera_driver   = NULL;
static void *camera_data                      = NULL;
static bool camera_driver_active              = false;
static bool camera_driver_data_own            = false;

/**
 * camera_driver_find_handle:
 * @idx                : index of driver to get handle to.
 *
 * Returns: handle to camera driver at index. Can be NULL
 * if nothing found.
 **/
const void *camera_driver_find_handle(int idx)
{
   const void *drv = camera_drivers[idx];
   if (!drv)
      return NULL;
   return drv;
}

/**
 * camera_driver_find_ident:
 * @idx                : index of driver to get handle to.
 *
 * Returns: Human-readable identifier of camera driver at index. Can be NULL
 * if nothing found.
 **/
const char *camera_driver_find_ident(int idx)
{
   const camera_driver_t *drv = camera_drivers[idx];
   if (!drv)
      return NULL;
   return drv->ident;
}

/**
 * config_get_camera_driver_options:
 *
 * Get an enumerated list of all camera driver names,
 * separated by '|'.
 *
 * Returns: string listing of all camera driver names,
 * separated by '|'.
 **/
const char* config_get_camera_driver_options(void)
{
   return char_list_new_special(STRING_LIST_CAMERA_DRIVERS, NULL);
}

void driver_camera_stop(void)
{
   camera_driver_ctl(RARCH_CAMERA_CTL_START, NULL);
}

bool driver_camera_start(void)
{
   return camera_driver_ctl(RARCH_CAMERA_CTL_START, NULL);
}

void camera_driver_poll(void)
{
   if (!camera_cb.caps)
      return;
   if (!camera_driver || !camera_driver->poll || !camera_data)
      return;
   camera_driver->poll(camera_data,
         camera_cb.frame_raw_framebuffer,
         camera_cb.frame_opengl_texture);
}

bool camera_driver_ctl(enum rarch_camera_ctl_state state, void *data)
{
   settings_t        *settings = config_get_ptr();

   switch (state)
   {
      case RARCH_CAMERA_CTL_DESTROY:
         camera_driver_active   = false;
         camera_driver_data_own = false;
         camera_driver          = NULL;
         camera_data            = NULL;
         break;
      case RARCH_CAMERA_CTL_SET_OWN_DRIVER:
         camera_driver_data_own = true;
         break;
      case RARCH_CAMERA_CTL_UNSET_OWN_DRIVER:
         camera_driver_data_own = false;
         break;
      case RARCH_CAMERA_CTL_OWNS_DRIVER:
         return camera_driver_data_own;
      case RARCH_CAMERA_CTL_SET_ACTIVE:
         camera_driver_active = true;
         break;
      case RARCH_CAMERA_CTL_FIND_DRIVER:
         {
            int i;
            driver_ctx_info_t drv;

            drv.label = "camera_driver";
            drv.s     = settings->arrays.camera_driver;

            driver_ctl(RARCH_DRIVER_CTL_FIND_INDEX, &drv);

            i         = (int)drv.len;

            if (i >= 0)
               camera_driver = (const camera_driver_t*)camera_driver_find_handle(i);
            else
            {
               unsigned d;
               RARCH_ERR("Couldn't find any camera driver named \"%s\"\n",
                     settings->arrays.camera_driver);
               RARCH_LOG_OUTPUT("Available camera drivers are:\n");
               for (d = 0; camera_driver_find_handle(d); d++)
                  RARCH_LOG_OUTPUT("\t%s\n", camera_driver_find_ident(d));

               RARCH_WARN("Going to default to first camera driver...\n");

               camera_driver = (const camera_driver_t*)camera_driver_find_handle(0);

               if (!camera_driver)
                  retroarch_fail(1, "find_camera_driver()");
            }
         }
         break;
      case RARCH_CAMERA_CTL_UNSET_ACTIVE:
         camera_driver_active = false;
         break;
      case RARCH_CAMERA_CTL_IS_ACTIVE:
        return camera_driver_active;
      case RARCH_CAMERA_CTL_DEINIT:
        if (camera_data && camera_driver)
        {
           if (camera_cb.deinitialized)
              camera_cb.deinitialized();

           if (camera_driver->free)
              camera_driver->free(camera_data);
        }

        camera_data = NULL;
        break;
      case RARCH_CAMERA_CTL_STOP:
        if (     camera_driver
              && camera_driver->stop
              && camera_data)
           camera_driver->stop(camera_data);
        break;
      case RARCH_CAMERA_CTL_START:
        if (camera_driver && camera_data && camera_driver->start)
        {
           if (settings->bools.camera_allow)
              return camera_driver->start(camera_data);

           runloop_msg_queue_push(
                 "Camera is explicitly disabled.\n", 1, 180, false);
        }
        break;
      case RARCH_CAMERA_CTL_SET_CB:
        {
           struct retro_camera_callback *cb =
              (struct retro_camera_callback*)data;
           camera_cb          = *cb;
        }
        break;
      case RARCH_CAMERA_CTL_INIT:
        /* Resource leaks will follow if camera is initialized twice. */
        if (camera_data)
           return false;

        camera_driver_ctl(RARCH_CAMERA_CTL_FIND_DRIVER, NULL);

        if (!camera_driver)
           return false;

        camera_data = camera_driver->init(
              *settings->arrays.camera_device ? settings->arrays.camera_device : NULL,
              camera_cb.caps,
              settings->uints.camera_width ?
              settings->uints.camera_width : camera_cb.width,
              settings->uints.camera_height ?
              settings->uints.camera_height : camera_cb.height);

        if (!camera_data)
        {
           RARCH_ERR("Failed to initialize camera driver. Will continue without camera.\n");
           camera_driver_ctl(RARCH_CAMERA_CTL_UNSET_ACTIVE, NULL);
        }

        if (camera_cb.initialized)
           camera_cb.initialized();
        break;
      default:
         break;
   }

   return true;
}
