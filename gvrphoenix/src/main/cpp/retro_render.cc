#include <frontend/frontend_driver.h>
#include <src/content.h>
#include <frontend/drivers/platform_linux.h>
#include <tasks/tasks_internal.h>
#include <ui/ui_companion_driver.h>
#include <src/retroarch.h>
#include <pthread.h>
#include <src/log.h>
#include "retro_render.h"


void RetroInit(const char *core_path, const char *game_path)
{
    if (frontend_driver_is_inited())
    {
        content_ctx_info_t info;
        char arguments[]  = "retrobf";
        char *argv[] = {arguments,   NULL};
        int argc = 1;
        info.argc            = argc;
        info.argv            = argv;
        info.environ_get     = frontend_driver_environment_get_ptr();

        if (!task_push_start_content(core_path, game_path, &info, CORE_TYPE_PLAIN, NULL, NULL))
            return;
    }

    ui_companion_driver_init_first();
}

void RetroSurfaceChange(int width, int height)
{

}

void RetroDrawFirst()
{
    pthread_t pid = pthread_self();
    LOGE("threadid:RetroDrawFirst, pid=%ld", pid);
    unsigned sleep_ms = 0;
    int ret = runloop_iterate(&sleep_ms);
    if (ret == 1 && sleep_ms > 0)
        retro_sleep(sleep_ms);
    task_queue_check();
    return;
}

