#ifndef TREASUREHUNT_APP_SRC_MAIN_JNI_TREASUREHUNTRENDERER_H_  // NOLINT
#define TREASUREHUNT_APP_SRC_MAIN_JNI_TREASUREHUNTRENDERER_H_  // NOLINT

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "gvr.h"
#include "gvr_audio.h"
#include "gvr_controller.h"
#include "gvr_types.h"
#include "world_layout_data.h"  // NOLINT
#include "esUtil.h"



class TreasureHuntRenderer {
public:
    TreasureHuntRenderer(gvr_context* gvr_context, std::unique_ptr<gvr::AudioApi> gvr_audio_api);
    ~TreasureHuntRenderer();
    void InitializeGl();
    void SurfaceChange(int width, int height);
    void DrawFrame();
    void OnTriggerEvent();
    void OnPause();
    void OnResume();

private:
    int CreateTexture(int width, int height, int textureFormat, int textureType);
    void PrepareFramebuffer();
    int LoadGLShader(int type, const char** shadercode);

    enum ViewType {
        kLeftView,
        kRightView,
        kMultiview
    };
    void DrawWorld(ViewType view);
    void DrawCardboardReticle();
    void DrawCube(ViewType view);
    void DrawFloor(ViewType view);
    void DrawDaydreamCursor(ViewType view);
    void HideObject();
    bool IsLookingAtObject();
    bool IsPointingAtObject();
    bool ObjectIsFound();
    void LoadAndPlayCubeSound();
    void ProcessControllerInput();
    void ResumeControllerApiAsNeeded();

    std::unique_ptr<gvr::GvrApi> gvr_api_;
    std::unique_ptr<gvr::AudioApi> gvr_audio_api_;
    std::unique_ptr<gvr::BufferViewportList> viewport_list_;
    std::unique_ptr<gvr::SwapChain> swapchain_;
    gvr::BufferViewport viewport_left_;
    gvr::BufferViewport viewport_right_;

    std::vector<float> lightpos_;

    WorldLayoutData world_layout_data_;

    const float* floor_vertices_;
    const float* cube_vertices_;
    const float* cube_colors_;
    const float* cube_found_colors_;
    const float* cube_normals_;
    const float* reticle_vertices_;

    int cube_program_;
    int floor_program_;
    int reticle_program_;

    int cube_position_param_;
    int cube_normal_param_;
    int cube_color_param_;
    int cube_model_param_;
    int cube_modelview_param_;
    int cube_modelview_projection_param_;
    int cube_light_pos_param_;

    int floor_position_param_;
    int floor_normal_param_;
    int floor_color_param_;
    int floor_model_param_;
    int floor_modelview_param_;
    int floor_modelview_projection_param_;
    int floor_light_pos_param_;

    int reticle_position_param_;
    int reticle_modelview_projection_param_;

    const gvr::Sizei reticle_render_size_;

    const std::array<float, 4> light_pos_world_space_;

    gvr::Mat4f head_view_;
    gvr::Mat4f model_cube_;
    gvr::Mat4f camera_;
    gvr::Mat4f view_;
    gvr::Mat4f model_floor_;
    gvr::Mat4f model_reticle_;
    gvr::Mat4f model_cursor_;
    gvr::Sizei render_size_;

    // View-dependent values.  These are stored in length two arrays to allow
    // syncing with uniforms consumed by the multiview vertex shader.  For
    // simplicity, we stash valid values in both elements (left, right) of these
    // arrays even when multiview is disabled.
    std::array<float, 3> light_pos_eye_space_[2];
    gvr::Mat4f modelview_projection_cube_[2];
    gvr::Mat4f modelview_projection_floor_[2];
    gvr::Mat4f modelview_projection_cursor_[2];
    gvr::Mat4f modelview_cube_[2];
    gvr::Mat4f modelview_floor_[2];

    int score_;
    float object_distance_;
    float reticle_distance_;
    bool multiview_enabled_;

    gvr::AudioSourceId audio_source_id_;

    gvr::AudioSourceId success_source_id_;

    std::thread audio_initialization_thread_;

    // Controller API entry point.
    std::unique_ptr<gvr::ControllerApi> gvr_controller_api_;

    // The latest controller state (updated once per frame).
    gvr::ControllerState gvr_controller_state_;

    gvr::ViewerType gvr_viewer_type_;

private:
    ESContext esContext;
    float lastTime;

};

#endif  // TREASUREHUNT_APP_SRC_MAIN_JNI_TREASUREHUNTRENDERER_H_  // NOLINT
