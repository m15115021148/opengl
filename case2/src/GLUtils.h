
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#ifndef _GL_UTIL_H_
#define _GL_UTIL_H_

class GLUtils {
	public:
		static GLuint LoadShader(GLenum shaderType, const char *pSource);
		static GLuint CreateProgram(const char *pVertexShaderSource, 
						const char *pFragShaderSource, GLuint &vertexShaderHandle, 
						GLuint &fragShaderHandle);
		static void DeleteProgram(GLuint &program);
		static void CheckGLError(const char *pGLOperation);
};

#endif