#include "log.h"
#include "jni.h"
#include "MyGLRenderContext.h"

#define NATIVE_RENDER_CLASS_NAME "com/romantic/gl/Case2NativeRender"

#ifdef _cplusplus
extern "C"{
#endif
JNIEXPORT void JNICALL init(JNIEnv *env, jclass type){
	MyGLRenderContext::GetInstance();
}

JNIEXPORT void JNICALL deInit(JNIEnv *env, jclass type){
	MyGLRenderContext::DestroyInstance();
}

JNIEXPORT void JNICALL setImageData(JNIEnv *env, jclass type, 
							jint format, jint width, jint height, jbyteArray imageData){
	int len = env->GetArrayLength (imageData);
    uint8_t* buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte*>(buf));
    MyGLRenderContext::GetInstance()->setImageData(format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}

JNIEXPORT void JNICALL onSurfaceCreated(JNIEnv *env, jclass type){
	MyGLRenderContext::GetInstance()->onSurfaceCreated();
}

JNIEXPORT void JNICALL onSurfaceChanged(JNIEnv *env, jclass type, jint width, jint height){
	MyGLRenderContext::GetInstance()->onSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL onDrawFrame(JNIEnv *env, jclass type){
	 MyGLRenderContext::GetInstance()->onDrawFrame();
}

#ifdef _cplusplus
}
#endif


static JNINativeMethod g_RenderMethods[] = {
	{"init",						"()V",				(void *)(init)},
	{"deInit",						"()V",				(void *)(deInit)},
	{"setImageData",				"(III[B)V",			(void *)(setImageData)},
	{"onSurfaceCreated",			"()V",				(void *)(onSurfaceCreated)},
	{"onSurfaceChanged",			"(II)V",			(void *)(onSurfaceChanged)},
	{"onDrawFrame",					"()V",				(void *)(onDrawFrame)},
};

static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum){
	LOGD("RegisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGE("RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0)
    {
        LOGE("RegisterNativeMethods fail");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className)
{
    LOGD("UnregisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGE("UnregisterNativeMethods fail. clazz == NULL");
        return;
    }
    if (env != NULL)
    {
        env->UnregisterNatives(clazz);
    }
}

// call this func when loading lib
extern "C" jint JNI_OnLoad(JavaVM *jvm, void *p)
{
    LOGD("===== JNI_OnLoad =====");
    jint jniRet = JNI_ERR;
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return jniRet;
    }

    jint regRet = RegisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME, g_RenderMethods,
                                        sizeof(g_RenderMethods) / sizeof(g_RenderMethods[0]));
    if (regRet != JNI_TRUE)
    {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *p)
{
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return;
    }

    UnregisterNativeMethods(env, NATIVE_RENDER_CLASS_NAME);
}