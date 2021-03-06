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
#include <compat/strl.h>
#include <string/stdstring.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "frontend_driver.h"

static frontend_ctx_driver_t *current_frontend_ctx;

bool frontend_driver_get_core_extension(char *s, size_t len)
{
   strlcpy(s, "so", len);
   return true;
}

bool frontend_driver_get_salamander_basename(char *s, size_t len)
{
   return false;
}

#ifndef IS_SALAMANDER
frontend_ctx_driver_t *frontend_get_ptr(void)
{
   return current_frontend_ctx;
}

int frontend_driver_parse_drive_list(void *data, bool load_content)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();

   if (!frontend || !frontend->parse_drive_list)
      return -1;
   return frontend->parse_drive_list(data, load_content);
}

void frontend_driver_content_loaded(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();

   if (!frontend || !frontend->content_loaded)
      return;
   frontend->content_loaded();
}

bool frontend_driver_has_fork(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();

   if (!frontend || !frontend->set_fork)
      return false;
   return true;
}

bool frontend_driver_set_fork(enum frontend_fork fork_mode)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();

   if (!frontend_driver_has_fork())
      return false;
   return frontend->set_fork(fork_mode);
}

void frontend_driver_process_args(int *argc, char *argv[])
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();

   if (!frontend || !frontend->process_args)
      return;
   frontend->process_args(argc, argv);
}

bool frontend_driver_is_inited(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend)
      return false;
   return true;
}

void frontend_driver_init_first()
{
   current_frontend_ctx = &frontend_ctx_linux;

   if (current_frontend_ctx && current_frontend_ctx->init)
      current_frontend_ctx->init();
}

void frontend_driver_free(void)
{
   current_frontend_ctx = NULL;
}

environment_get_t frontend_driver_environment_get_ptr(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend)
      return NULL;
   return frontend->environment_get;
}

bool frontend_driver_has_get_video_driver_func(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_video_driver)
      return false;
   return true;
}

const struct video_driver *frontend_driver_get_video_driver(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_video_driver)
      return NULL;
   return frontend->get_video_driver();
}

void frontend_driver_exitspawn(char *s, size_t len)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->exitspawn)
      return;
   frontend->exitspawn(s, len);
}

void frontend_driver_deinit(void *args)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->deinit)
      return;
   frontend->deinit(args);
}

void frontend_driver_shutdown(bool a)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->shutdown)
      return;
   frontend->shutdown(a);
}

enum frontend_architecture frontend_driver_get_cpu_architecture(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_architecture)
      return FRONTEND_ARCH_NONE;
   return frontend->get_architecture();
}

uint64_t frontend_driver_get_total_memory(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_total_mem)
      return 0;
   return frontend->get_total_mem();
}

uint64_t frontend_driver_get_used_memory(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_used_mem)
      return 0;
   return frontend->get_used_mem();
}

void frontend_driver_install_signal_handler(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->install_signal_handler)
      return;
   frontend->install_signal_handler();
}

int frontend_driver_get_signal_handler_state(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->get_signal_handler_state)
      return -1;
   return frontend->get_signal_handler_state();
}

void frontend_driver_set_signal_handler_state(int value)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->set_signal_handler_state)
      return;
   frontend->set_signal_handler_state(value);
}

void frontend_driver_attach_console(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->attach_console)
      return;
   frontend->attach_console();
}

void frontend_driver_detach_console(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->detach_console)
      return;
   frontend->detach_console();
}

void frontend_driver_destroy_signal_handler_state(void)
{
   frontend_ctx_driver_t *frontend = frontend_get_ptr();
   if (!frontend || !frontend->destroy_signal_handler_state)
      return;
   frontend->destroy_signal_handler_state();
}
#endif
