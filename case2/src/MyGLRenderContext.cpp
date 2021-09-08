
#include "log.h"
#include <stdint.h>
#include <string.h>
#include "MyGLRenderContext.h"
#include "TriangleSample.h"
#include "GLUtils.h"
#include "imageDef.h"
#include "TextureMapSample.h"

MyGLRenderContext* MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext(){
	
}

MyGLRenderContext::~MyGLRenderContext(){
	
}

void MyGLRenderContext::setImageData(int format, int width, int height, uint8_t *pData){
	LOGD("MyGLRenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format){
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
	p_MapSample->loadImage(&nativeImage);
}

void MyGLRenderContext::onSurfaceCreated(){
	LOGD("MyGLRenderContext::OnSurfaceCreated");
    glClearColor(1.0f,1.0f,1.0f, 1.0f);
    //p_Sample->init();
	p_MapSample->init();
}

void MyGLRenderContext::onSurfaceChanged(int width, int height){
	LOGD("MyGLRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
}

void MyGLRenderContext::onDrawFrame(){
	//LOGD("MyGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //p_Sample->draw();
	p_MapSample->draw();

}

MyGLRenderContext *MyGLRenderContext::GetInstance(){
	if (m_pContext == nullptr) {
		LOGD("MyGLRenderContext::GetInstance");
		m_pContext = new MyGLRenderContext();
	}
	return m_pContext;
}

void MyGLRenderContext::DestroyInstance(){
	LOGE("MyGLRenderContext::DestroyInstance");
	if (m_pContext) {
		delete m_pContext;
		m_pContext = nullptr;
	}
}