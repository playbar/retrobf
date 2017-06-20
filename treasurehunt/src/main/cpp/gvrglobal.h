#ifndef __GVRTEST_H__
#define __GVRTEST_H__

#include <string.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <assert.h>


// Android log function wrappers
static const char* kTAG = "gvr";
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

long getimagebase();
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved = nullptr);
JNIEXPORT jstring JNICALL Java_com_mj_vr_MainActivity_stringFromJNI( JNIEnv* env, jobject thiz );

#ifdef __cplusplus
}
#endif

#endif
