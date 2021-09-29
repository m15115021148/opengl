#include "log.h"
#include "jni.h"
#include <cstdio>
#include <cstring>

#include <string>
#include <android/bitmap.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "libyuv.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>

#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavfilter/avfilter.h>

#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include <libavutil/imgutils.h>

};

#define NATIVE_RENDER_CLASS_NAME "com/ffmpeg/FFMediaPlayer"

static AVFormatContext *pFormatCtx;

#ifdef _cplusplus
extern "C"{
#endif
JNIEXPORT jstring JNICALL getFFmpegVersion(JNIEnv *env, jobject type){
	char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "libavcodec : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(strBuffer, "\nlibavformat : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(strBuffer, "\nlibavutil : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVUTIL_VERSION));
    strcat(strBuffer, "\nlibavfilter : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFILTER_VERSION));
    strcat(strBuffer, "\nlibswresample : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    strcat(strBuffer, "\nlibswscale : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWSCALE_VERSION));
    strcat(strBuffer, "\navcodec_configure : \n");
    strcat(strBuffer, avcodec_configuration());
    strcat(strBuffer, "\navcodec_license : ");
    strcat(strBuffer, avcodec_license());
    LOGD("getFFmpegVersion\n%s", strBuffer);
    return env->NewStringUTF(strBuffer);
}

JNIEXPORT jint JNICALL playVideo(JNIEnv *env, jobject type, jstring url, jobject surface){
	
	const char *input = env->GetStringUTFChars(url, NULL);

	char input_url[1024]={0};
	sprintf(input_url,"%s", input);
	LOGD("input_url %s",input_url);
	
	av_register_all();
	
	pFormatCtx = avformat_alloc_context();
	
	char buf[1024];
	int err_code = avformat_open_input(&pFormatCtx, input_url, NULL, NULL);

	if (err_code != 0) {
		av_strerror(err_code, buf, 1024);
		LOGE("avformat_open_input fail : %d(%s)", err_code, buf);
		return -1;
	}
	
	if (avformat_find_stream_info(pFormatCtx, NULL) <0 ) {
		LOGE("avformat_find_stream_info fail.");
		return -1;
	}
	
	int videoIndex = -1;
	for (int i=0; i<pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoIndex = i;
			break;
		}
	}
	
	if (videoIndex == -1) {
		LOGE("fail to find stream index.");
		return -1;
	}
	
	AVCodecParameters *codecParameters = pFormatCtx->streams[videoIndex]->codecpar;
	
	//get decoder by codec_id
	AVCodec *pCodec = avcodec_find_decoder(codecParameters->codec_id);
	if (pCodec == nullptr){
		LOGE("avcodec_find_decoder fail.");
		return -1;
	}
	
	AVCodecContext *pAVCodecContent = avcodec_alloc_context3(pCodec);
	if (avcodec_parameters_to_context(pAVCodecContent, codecParameters) != 0) {
		LOGE("avcodec_parameters_to_context fail.");
		return -1;
	}
	
	int result = avcodec_open2(pAVCodecContent, pCodec, NULL);
	if (result < 0) {
		LOGE("avcodec_open2 fail. result = %d", result);
	}
	
	//输出视频信息
    LOGD("视频的文件格式：%s", pFormatCtx->iformat->name);
    LOGD("视频时长：%lld", (pFormatCtx->duration) / (1000 * 1000));
    LOGD("视频的宽高：%d,%d", pAVCodecContent->width, pAVCodecContent->height);
    LOGD("解码器的名称：%s", pCodec->name);
	
	// 获取视频的宽高
    int videoWidth = pAVCodecContent->width;
    int videoHeight = pAVCodecContent->height;

	//read data 
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	
	AVFrame *yuv_frame = av_frame_alloc();
	AVFrame *rgb_frame = av_frame_alloc();
	
	int got_picture, ret;
	int frame_count = 0;

	ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
	if (nativeWindow == NULL) {
		LOGE("init surface nativeWindow is null");
		return -1;
	}
	
	result = ANativeWindow_setBuffersGeometry(
					nativeWindow, 
					pAVCodecContent->width, 
					pAVCodecContent->height,
					WINDOW_FORMAT_RGBA_8888
				);
	if (result < 0) {
		LOGE("Player Error : Can not set native window buffer");
		return -1;
	}
	
	ANativeWindow_Buffer window_buffer;
	
	/*
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
    uint8_t *out_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, out_buffer, AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
	*/
	
	while ( av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == videoIndex) {
			ret = avcodec_decode_video2(pAVCodecContent, yuv_frame, &got_picture, packet);
			if (ret < 0) {
				LOGE("decode failt , ret=%d", ret);
				return -1;
			}
			
			if (got_picture){
				
				
				ANativeWindow_lock(nativeWindow, &window_buffer, NULL);
							   
				av_image_fill_arrays(
								rgb_frame->data,
								rgb_frame->linesize,
								(uint8_t *) window_buffer.bits,
								AV_PIX_FMT_RGBA,
								pAVCodecContent->width,
								pAVCodecContent->height,
								1
				);

                //YUV格式的数据转换成RGBA 8888格式的数据, FFmpeg 也可以转换，但是存在问题，使用libyuv这个库实现
                libyuv::I420ToARGB(yuv_frame->data[0], yuv_frame->linesize[0],
                           yuv_frame->data[2], yuv_frame->linesize[2],
                           yuv_frame->data[1], yuv_frame->linesize[1],
                           rgb_frame->data[0], rgb_frame->linesize[0],
                           pAVCodecContent->width, pAVCodecContent->height);

                //3、unlock window
                ANativeWindow_unlockAndPost(nativeWindow);

                frame_count++;
                LOGI("解码绘制第%d帧", frame_count);
			}
		}
		//释放资源
        av_free_packet(packet);
	}
		
    av_frame_free(&rgb_frame);
    av_frame_free(&yuv_frame);
    av_packet_free(&packet);
    ANativeWindow_release(nativeWindow);
    avcodec_close(pAVCodecContent);
    avformat_close_input(&pFormatCtx);
	
    env->ReleaseStringUTFChars(url, input);
	return 0;
}

#ifdef _cplusplus
}
#endif


static JNINativeMethod g_RenderMethods[] = {
	{"getFFmpegVersion",				"()Ljava/lang/String;",										(void *)(getFFmpegVersion)},
	{"playVideo",						"(Ljava/lang/String;Landroid/view/Surface;)I",				(void *)(playVideo)},
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
