
#include <stdint.h>
#include <string.h>
#include "TriangleSample.h"
#include "TextureMapSample.h"

#ifndef _My_GL_RENDER_CONTEXT_H_
#define _My_GL_RENDER_CONTEXT_H_

class MyGLRenderContext
{
	public:
		~MyGLRenderContext();
		MyGLRenderContext();
		
		void setImageData(int format, int width, int height, uint8_t *pData);
		void onSurfaceCreated();
		void onSurfaceChanged(int width, int height);
		void onDrawFrame();
		
		static MyGLRenderContext* GetInstance();
		static void DestroyInstance();
	
	private:
		static MyGLRenderContext *m_pContext;
		TriangleSample *p_Sample = new TriangleSample();
		TextureMapSample *p_MapSample = new TextureMapSample();
};

#endif