/* Copyright 2017 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/log.h>
#include <jni.h>

#include <memory>
#include <src/paths.h>

#include "treasure_hunt_renderer.h"  // NOLINT
#include "gvr.h"
#include "gvr_audio.h"
#include "retro_render.h"


#define JNI_METHOD(return_type, method_name) JNIEXPORT return_type JNICALL Java_com_mj_retro_BfRenderer_##method_name

#define JNI_RETRO(return_type, method_name) JNIEXPORT return_type JNICALL Java_com_mj_retro_RetroRender_##method_name

namespace {

inline jlong jptr(TreasureHuntRenderer *native_treasure_hunt)
{
  return reinterpret_cast<intptr_t>(native_treasure_hunt);
}

inline TreasureHuntRenderer *native(jlong ptr)
{
  return reinterpret_cast<TreasureHuntRenderer *>(ptr);
}
}  // anonymous namespace

extern "C" {

extern void android_app_oncreate(jobject clazz );
extern void android_dispatch_motion_event(int source, int id,
                                          float x, float y, float z, float rz, float hatx, float haty,
                                          float ltrig, float rtrig, float brake, float gas);
extern void android_dispatch_key_event(int source, int id, int keycode, int action, int mate);


JavaVM* gvm = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    gvm = vm;
    return JNI_VERSION_1_6;
}


JNIEXPORT void JNICALL Java_com_mj_retro_RetroActivityCommon_nativeOnCreate(JNIEnv *env, jobject obj)
{
    jclass claz = env->GetObjectClass(obj);
    jmethodID getIntent = env->GetMethodID(claz, "getIntent", "()Landroid/content/Intent;");
    jobject gobj = env->NewGlobalRef( obj );
    android_app_oncreate(gobj );
}

JNI_RETRO(void, nativeDispatchMotionEvent)(JNIEnv *env, jobject obj, int source, int id,
                                           float x, float y, float z, float rz, float hatx, float haty,
                                           float ltrig, float rtrig, float brake, float gas)
{
    android_dispatch_motion_event( source, id, x, y, z, rz, hatx, haty, ltrig, rtrig, brake, gas);
}

JNI_RETRO(void, nativeDispatchKeyEvent)(JNIEnv *env, jobject obj, int source, int id, int keycode, int action, int mate)
{
    android_dispatch_key_event(source, id, keycode, action, mate);
}

JNI_RETRO(void, nativeSetPath)(JNIEnv *env, jclass clazz, jstring strPath)
{
    char buf[4096];
    const char *str = env->GetStringUTFChars(strPath, 0);
    path_set(RARCH_PATH_CORE, str);
    env->ReleaseStringUTFChars(strPath, str);
    return;

}


JNI_RETRO(void,  nativeRetroInit)()
{
    RetroInit();
}
JNI_RETRO(void,  nativeRetroSurfaceChange)( int width, int height )
{
    RetroSurfaceChange(width, height);
}

JNI_RETRO(void,  nativeRetroDrawFrame)()
{
    RetroDrawFrame();
}

///////////////////////////////////////////////


JNI_METHOD(jlong, nativeCreateRenderer)(JNIEnv *env, jobject obj, jobject class_loader, jobject android_context, jlong native_gvr_api)
{
  std::unique_ptr<gvr::AudioApi> audio_context(new gvr::AudioApi);
  audio_context->Init(env, android_context, class_loader, GVR_AUDIO_RENDERING_BINAURAL_HIGH_QUALITY);
  return jptr(new TreasureHuntRenderer(reinterpret_cast<gvr_context *>(native_gvr_api), std::move(audio_context)));
}

JNI_METHOD(void, nativeDestroyRenderer)(JNIEnv *env, jclass obj, jlong native_treasure_hunt)
{
  delete native(native_treasure_hunt);
}

JNI_METHOD(void, nativeInitializeGl)(JNIEnv *env, jobject obj, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->InitializeGl();
}

JNI_METHOD(void, nativeSurfaceChange)(JNIEnv *env, jobject obj, jlong native_treasure_hunt, jint width, jint height)
{
  native(native_treasure_hunt)->SurfaceChange(width, height );
}

JNI_METHOD(void, nativeDrawFrame)(JNIEnv *env, jobject obj, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->DrawFrame();
}

JNI_METHOD(void, nativeOnTriggerEvent)(JNIEnv *env, jobject obj, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->OnTriggerEvent();
}

JNI_METHOD(void, nativeOnPause)(JNIEnv *env, jobject obj, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->OnPause();
}

JNI_METHOD(void, nativeOnResume)(JNIEnv *env, jobject obj, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->OnResume();
}

}  // extern "C"
