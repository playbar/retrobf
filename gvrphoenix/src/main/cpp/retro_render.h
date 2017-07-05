#ifndef __RETRO_RENDER_H__
#define __RETRO_RENDER_H__

#ifdef   __cplusplus
extern "C"{
void RetroInit(const char *core_path, const char *game_path);
void RetroSurfaceChange(int width, int height);
void RetroDrawFrame();
}
#endif

#endif

