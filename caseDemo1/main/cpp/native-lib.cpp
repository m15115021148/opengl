
#include "native-lib.h"


extern "C" {
    JNIEXPORT void JNICALL
    Java_com_romantic_gl_OpenGLNative_init(JNIEnv *env, jclass type) {
        renderS=new GLrenderS;
        renderS->cal_pixel();
    }

    JNIEXPORT void JNICALL
    Java_com_romantic_gl_OpenGLNative_startRenders(JNIEnv *env, jclass type) {
        renderS->StartRenderThread();
    }

    JNIEXPORT void JNICALL
    Java_com_romantic_gl_OpenGLNative_stopRenders(JNIEnv *env, jclass type) {
        renderS->StopRenderThread();
    }

    JNIEXPORT void JNICALL
    Java_com_romantic_gl_OpenGLNative_setSurfaces(JNIEnv *env, jclass type, jobject surface) {
        renderS->SetWindow(env,surface);
    }
}

