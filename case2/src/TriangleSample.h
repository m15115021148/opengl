
#include <stdint.h>
#include <string.h>
#include "GLUtils.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#ifndef _TRIANGLE_SAMPLE_H_
#define _TRIANGLE_SAMPLE_H_

class TriangleSample {
	public:
		TriangleSample();
		~TriangleSample();
		
		void init();
		void draw();
	private:
		GLuint m_ProgramObj;
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
};


#endif