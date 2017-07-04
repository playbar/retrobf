#include <frontend/frontend_driver.h>
#include <src/content.h>
#include <frontend/drivers/platform_linux.h>
#include <tasks/tasks_internal.h>
#include <ui/ui_companion_driver.h>
#include <src/retroarch.h>
#include <pthread.h>
#include <src/log.h>
#include "retro_render.h"


void RetroInit()
{
    if (frontend_driver_is_inited())
    {
        content_ctx_info_t info;
        char arguments[]  = "retroarch";
        char *argv[] = {arguments,   NULL};
        int argc = 1;
        info.argc            = argc;
        info.argv            = argv;
        info.args            = (void*)g_android;
        info.environ_get     = frontend_driver_environment_get_ptr();

        if (!task_push_start_content_from_cli(NULL, NULL, &info, CORE_TYPE_PLAIN, NULL, NULL))
            return;
    }

    ui_companion_driver_init_first();
}

void RetroSurfaceChange(int width, int height)
{

}

void RetroDrawFrame()
{
    pthread_t pid = pthread_self();
    LOGE("threadid:RetroDrawFrame, pid=%ld", pid);
    unsigned sleep_ms = 0;
    int ret = runloop_iterate(&sleep_ms);
    if (ret == 1 && sleep_ms > 0)
        retro_sleep(sleep_ms);
    task_queue_check();
}
