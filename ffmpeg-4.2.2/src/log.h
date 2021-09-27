#include <android/log.h>

#ifndef _LOG_H_
#define _LOG_H_ 1

#define LOG_TAG "natveJni"

#define CubicLogE(...) \
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define CubicLogD(...)  \
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define CubicLogI(...)  \
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
		
#define CubicLogW(...)  \
        __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#define LOGE CubicLogE
#define LOGD CubicLogD
#define LOGI CubicLogI
#define LOGW CubicLogW


#endif
