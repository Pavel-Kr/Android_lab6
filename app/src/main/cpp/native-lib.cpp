#include <jni.h>
#include <iostream>
#include <time.h>
#include "Cube.h"

float angle = 0.0;
float zPos = -50;

double prev_time;

Cube *cube;

float projectionMatrix[16] = {0};

std::string vertexShader =
        "attribute vec3 position;\n"
        "attribute vec2 texCoords;\n"
        "uniform mat4 trans;\n"
        "uniform mat4 rot;\n"
        "uniform mat4 scale;\n"
        "uniform mat4 proj;\n"
        "varying vec2 pass_texCoords;\n"
        "void main() {\n"
        "   gl_Position = proj * trans * rot * scale * vec4(position, 1.0);\n"
        "   pass_texCoords = texCoords;"
        "}";

std::string fragmentShader =
        "precision mediump float;"
        "varying vec2 pass_texCoords;\n"
        "uniform sampler2D boxTexture;\n"
        "void main() {\n"
        //"   gl_FragColor = vec4(pass_texCoords, 1.0, 1.0);\n"
        "   gl_FragColor = texture2D(boxTexture, pass_texCoords);\n"
        "}";

void createCube() {
    cube = new Cube();
    cube->setPosition(0, -2, 10);
}

void calcProjMatrix(float fov, float near, float far, float aspectRatio){
    float invTanHalfFOV = 1 / tan(fov / 2);
    float projectionMatrix[] = {
            invTanHalfFOV/aspectRatio, 0, 0, 0,
            0, invTanHalfFOV, 0, 0,
            0, 0, (-far - near)/(near - far), 2*far*near/(near - far),
            0, 0, 1, 0
    };
    cube->setProjectionMatrix(projectionMatrix);
}

static double now_ms(void) {

    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lab6_CRenderer_00024Companion_onSurfaceCreated(JNIEnv *env, jobject thiz) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    createCube();
    cube->loadShaders(vertexShader, fragmentShader);
    __android_log_write(ANDROID_LOG_INFO, "info", "prog initialized");
    prev_time = now_ms();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lab6_CRenderer_00024Companion_onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                jint width, jint height,
                                                                jfloatArray proj_matrix) {
    glViewport(0, 0, width, height);
    float fov = 45.0;
    float aspectRatio = (float)width / height;
    float near = 1.0, far = 100.0;
    calcProjMatrix(fov, near, far, aspectRatio);
    __android_log_write(ANDROID_LOG_INFO, "info", "surface changed");
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lab6_CRenderer_00024Companion_render(JNIEnv *env, jobject thiz) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    double delta = now_ms() - prev_time;
    prev_time = now_ms();
    double speed = 0.05;
    angle += delta * speed;
    while (angle > 360.0){
        angle -= 360.0;
    }
    /*zPos += 1;
    cube->setPosition(0, 0, zPos);*/
    //cube->setRotation(angle);
    cube->render();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_lab6_CRenderer_00024Companion_setTexture(JNIEnv *env, jobject thiz,
                                                          jintArray pixels, jint width,
                                                          jint height) {

    GLuint *data = new GLuint[width*height];
    jint *p_pix = env->GetIntArrayElements(pixels, NULL);
    for(int i=0; i<width*height; ++i) {
        data[i] = (p_pix[i]>>16)&0xff;
        data[i] |= p_pix[i]&0xff00;
        data[i] |= (p_pix[i]<<16)&0xff0000;
    }
    cube->loadTexture(data, width, height);
}