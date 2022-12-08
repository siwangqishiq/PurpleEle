#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>

#include "application.hpp"

#define  LOG_TAG    "textrender"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

AAssetManager *AndroidAssetManagerInstance = nullptr;

class AndroidApplication : public Application{
};

std::shared_ptr<AndroidApplication> app = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGI("jni onload called");
    JNIEnv* env = nullptr; //注册时在JNIEnv中实现的，所以必须首先获取它
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) { //从JavaVM获取JNIEnv，一般使用1.4的版本
        return -1;
    }
//    app = std::make_shared<AndroidApplication>();
    LOGI("jni onload called end...");
    return JNI_VERSION_1_4; //
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_textrender_NativeBridge_init(JNIEnv *env, jclass clazz) {
    app = std::make_shared<AndroidApplication>();
    LOGI("app init");
    app->init();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_textrender_NativeBridge_trick(JNIEnv *env, jclass clazz) {
    //LOGI("app trick");
    app->update();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_textrender_NativeBridge_free(JNIEnv *env, jclass clazz) {
    LOGI("app free");
    app->free();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_textrender_NativeBridge_resize(JNIEnv *env, jclass clazz, jint width, jint height) {
    LOGI("app resize %d , %d" , width , height);
    app->viewWidth_ = width;
    app->viewHeight_ = height;
    app->onResize(width , height);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_textrender_NativeBridge_setAndroidAssetManager(JNIEnv *env, jclass clazz,
                                                              jobject mgr) {
    AndroidAssetManagerInstance = AAssetManager_fromJava(env , mgr);
}