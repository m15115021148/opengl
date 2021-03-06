
#include "TriangleSample.h"
#include "log.h"
#include <stdlib.h>

TriangleSample::TriangleSample(){
	
}

TriangleSample::~TriangleSample(){
	if (m_ProgramObj){
        glDeleteProgram(m_ProgramObj);
    }
}

void TriangleSample::init(){
	char vShaderStr[] =
            "#version 300 es                          			\n"
            "layout(location = 0) in vec4 vPosition;  			\n"
            "void main()                              			\n"
            "{                                        			\n"
            "   gl_Position = vPosition;     					\n"
            "}                                        			\n";

    char fShaderStr[] =
            "#version 300 es                              		\n"
            "precision mediump float;                     		\n"
            "out vec4 fragColor;                          		\n"
            "void main()                                  		\n"
            "{                                           	 	\n"
            "   fragColor = vec4 (0.7f, 0.5f, 1.0f, 1.0f);  	\n"
            "}                                            		\n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
}

void TriangleSample::draw(){
	//LOGD("TriangleSample::Draw");
    GLfloat vVertices[] = {
             0.0f,  0.3f, 0.0f,
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
    };

    if(m_ProgramObj == 0)
        return;

    // Use the program object
    glUseProgram (m_ProgramObj);

    // Load the vertex data
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray (0);

    glDrawArrays (GL_TRIANGLES, 0, 3);

}