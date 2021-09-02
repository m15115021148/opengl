#include <jni.h>
#include <android/log.h>
 
#include <GLES/gl.h>
#include <GLES/glext.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
 
static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}
 
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
 
bool init() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
 
 
    glShadeModel(GL_SMOOTH);						// 启用阴影平滑
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// 黑色背景
    glClearDepthf(1.0f);							// 设置深度缓存
    glEnable(GL_DEPTH_TEST);						// 启用深度测试
    glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 告诉系统对透视进行修正
 
    return true;
}
 
const GLfloat gVertices[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
 
		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, -1.0f,
 
		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, -1.0f,
		-1.0f,-1.0f, -1.0f,
 
		0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f
};
 
const GLfloat gColors[] = {
		1.0f,0.0f,0.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
 
		1.0f,0.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,
 
		1.0f,0.0f,0.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
 
		1.0f,0.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f
};
 
static GLfloat rtri;  //三角形的旋转变量
static GLfloat rquad;  //四边形的旋转变量
 
const GLfloat PI = 3.1415f;
 
static void _gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat top = zNear * ((GLfloat) tan(fovy * PI / 360.0));
	GLfloat bottom = -top;
	GLfloat left = bottom * aspect;
	GLfloat right = top * aspect;
	glFrustumf(left, right, bottom, top, zNear, zFar);
}
 
void resize(int width, int height)
{
    if (height==0)								// 防止被零除
      {
              height=1;							// 将Height设为1
      }
 
      glViewport(0, 0, width, height);					// 重置当前的视口
      glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
      glLoadIdentity();							// 重置投影矩阵
 
      GLfloat ratio = (GLfloat)width/(GLfloat)height;
      // 设置视口的大小
      _gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  //    glOrthof(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
 
      glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵
      glLoadIdentity();							// 重置模型观察矩阵
}
 
void renderFrame() {
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕及深度缓存
    glLoadIdentity();					// 重置模型观察矩阵
    glTranslatef(0.0f,0.0f, -6.0f);				// 移入屏幕 6.0
 
    glRotatef(rtri,0.0f,1.0f,0.0f);				// 绕Y轴旋转金字塔
 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, gColors);
 
    glVertexPointer(3, GL_FLOAT, 0, gVertices);
    glDrawArrays(GL_TRIANGLES, 0, 12);
 
 
	rtri += 0.2f;						// 增加三角形的旋转变量
	//LOGI("xxxxx");
 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
 
	glFlush();
}
 
extern "C" {
    JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_resize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_init(JNIEnv * env, jobject obj);
};
 
JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_resize(JNIEnv * env, jobject obj,  jint width, jint height)
{
    resize(width, height);
}
 
JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}
 
JNIEXPORT void JNICALL Java_com_romantic_gl_GL10Native_init(JNIEnv * env, jobject obj)
{
	init();
}