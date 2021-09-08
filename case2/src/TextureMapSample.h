
#include <stdint.h>
#include <string.h>
#include "GLUtils.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "imageDef.h"

#ifndef _TEXTURE_MAP_SAMPLE_H_
#define _TEXTURE_MAP_SAMPLE_H_

class TextureMapSample {
	public:
		TextureMapSample();
		~TextureMapSample();
		
		void loadImage(NativeImage *pImage);
		
		void init();
		void draw();
		void destroy();
	private:
		GLuint m_ProgramObj;
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		
		GLuint m_TextureId;
		GLint m_SamplerLoc;
		NativeImage m_RenderImage;
};


#endif