//
// Created by pkriv on 23.11.2023.
//

#ifndef LAB6_CUBE_H
#define LAB6_CUBE_H

#include <math.h>
#include <string>
#include <android/log.h>
#include "GLES3/gl3.h"

#define toRadians(x) (float)(((x) * M_PI) / 180.0)

struct Vector3{
    float x, y, z;
};

void setIdentity(float matrix[]);


class Cube {
    static const int verticesCount = 24;
    static const int indicesCount = 36;
    static const int stride = 5;

    Vector3 position;
    float scale;
    float angle;

    GLuint vao, vbo, ibo;
    GLuint shaderProgram;
    GLuint location_trans;
    GLuint location_proj;
    GLuint location_rot;
    GLuint location_scale;
    GLuint location_boxTexture;

    GLuint texture;

    float vertices[verticesCount * stride] = {
            -1, -1, -1, 0, 0,// front face
            1, -1, -1, 1, 0,
            1, 1, -1, 1, 1,
            -1, 1, -1, 0, 1,

            1, -1, -1, 0, 0, // right face
            1, -1, 1, 1, 0,
            1, 1, 1, 1, 1,
            1, 1, -1, 0, 1,

            1, -1, 1, 0, 0,// back face
            -1, -1, 1, 1, 0,
            -1, 1, 1, 1, 1,
            1, 1, 1, 0, 1,

            -1, -1, 1, 0, 0,// left face
            -1, -1, -1, 1, 0,
            -1, 1, -1, 1, 1,
            -1, 1, 1, 0, 1,

            -1, 1, -1, 0, 0,// top face
            1, 1, -1, 1, 0,
            1, 1, 1, 1, 1,
            -1, 1, 1, 0, 1,

            -1, -1, -1, 0, 0,// bottom face
            -1, -1, 1, 1, 0,
            1, -1, 1, 1, 1,
            1, -1, -1, 0, 1
    };

    unsigned int indices[indicesCount] = {
            0, 1, 2,  // front face
            0, 2, 3,
            4, 5, 6,  // right face
            4, 6, 7,
            12, 13, 14, // left face
            12, 14, 15,
            8, 9, 10, // back face
            8, 10, 11,
            20, 21, 22, // bottom face
            20, 22, 23,
            16, 17, 18, // top face
            16, 18, 19
    };

    float translationMatrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, -2,
            0, 0, 1, 10,
            0, 0, 0, 1
    };

    float scaleMatrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    void getLocations();

public:
    Cube();
    void loadShaders(std::string vertexShaderSrc, std::string fragmentShaderSrc);
    void loadTexture(const void* pixels, int width, int height);
    float* getVertices();
    int getVerticesCount();
    unsigned int* getIndices();
    int getIndicesCount();
    void setPosition(float x, float y, float z);
    void getTranslation(float translationMatrix[]);
    void setScale(float scale);
    void getScale(float scaleMatrix[]);
    void setRotation(float angle);
    void getRotation(float rotationMatrix[]);
    void setProjectionMatrix(float projectionMatrix[]);
    void render();
};


#endif //LAB6_CUBE_H
