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

#include "treasure_hunt_renderer.h"  // NOLINT
#include "gvr.h"
#include "gvr_audio.h"


#define JNI_METHOD(return_type, method_name) \
JNIEXPORT return_type JNICALL Java_com_retroarch_browser_retroactivity_RetroActivityFuture_##method_name

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
JavaVM* gvm = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    gvm = vm;
    return JNI_VERSION_1_6;
}

extern void android_app_oncreate(jobject clazz );

JNI_METHOD(void, nativeOnCreate)(JNIEnv *env, jobject obj)
{
    jclass claz = env->GetObjectClass(obj);
    jmethodID getIntent = env->GetMethodID(claz, "getIntent", "()Landroid/content/Intent;");
    jobject gobj = env->NewGlobalRef( obj );
    android_app_oncreate(gobj );
}

JNI_METHOD(void, nativeDispatchKeyEvent)(JNIEnv *env, jobject clazz, jlong native_treasure_hunt)
{
  native(native_treasure_hunt)->DispatchKeyEvent();
}

JNI_METHOD(jlong, nativeCreateRenderer)(JNIEnv *env, jobject clazz, jobject class_loader, jobject android_context, jlong native_gvr_api)
{
  std::unique_ptr<gvr::AudioApi> audio_context(new gvr::AudioApi);
  audio_context->Init(env, android_context, class_loader, GVR_AUDIO_RENDERING_BINAURAL_HIGH_QUALITY);
  return jptr(new TreasureHuntRenderer(reinterpret_cast<gvr_context *>(native_gvr_api), std::move(audio_context)));
}

JNI_METHOD(void, nativeDestroyRenderer)(JNIEnv *env, jclass clazz, jlong native_treasure_hunt)
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
