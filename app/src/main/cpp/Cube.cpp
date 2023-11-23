//
// Created by pkriv on 23.11.2023.
//

#include "Cube.h"

void setIdentity(float matrix[])
{
    for(int i = 0; i < 16; i++){
        matrix[i] = 0;
    }
    for(int i = 0; i < 4; i++){
        matrix[i * 4 + i] = 1;
    }
}

Cube::Cube() {
    position = {0, 0, 0};
    scale = 1;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * stride * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::getTranslation(float *translationMatrix) {
    setIdentity(translationMatrix);
    translationMatrix[0 * 4 + 3] = position.x;
    translationMatrix[1 * 4 + 3] = position.y;
    translationMatrix[2 * 4 + 3] = position.z;
}

void Cube::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Cube::setScale(float scale) {
    this->scale = scale;
}

void Cube::getScale(float *scaleMatrix) {
    setIdentity(scaleMatrix);
    for(int i = 0; i < 3; i++){
        scaleMatrix[i * 4 + i] = scale;
    }
}

float* Cube::getVertices() {
    return vertices;
}

unsigned int *Cube::getIndices() {
    return indices;
}

int Cube::getVerticesCount() {
    return verticesCount * stride;
}

int Cube::getIndicesCount() {
    return indicesCount;
}

void Cube::setRotation(float angle) {
    this->angle = angle;
}

void Cube::getRotation(float *rotationMatrix) {
    setIdentity(rotationMatrix);
    float cosAngle = cos(toRadians(angle));
    float sinAngle = sin(toRadians(angle));
    rotationMatrix[0] = rotationMatrix[2 * 4 + 2] = cosAngle;
    rotationMatrix[2] = sinAngle;
    rotationMatrix[2 * 4 + 0] = -sinAngle;
}

void Cube::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    getTranslation(translationMatrix);
    glUniformMatrix4fv(location_trans, 1, GL_TRUE, translationMatrix);
    float rotationMatrix[16];
    getRotation(rotationMatrix);
    glUniformMatrix4fv(location_rot, 1, GL_TRUE, rotationMatrix);
    getScale(scaleMatrix);
    glUniformMatrix4fv(location_scale, 1, GL_TRUE, scaleMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(location_boxTexture, 0);
    glDrawElements(GL_TRIANGLES, indicesCount * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);
}

int compileShader(GLenum type, const char* src, GLuint len){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE){
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        __android_log_write(ANDROID_LOG_ERROR, "compileShader", log);
    }
    return shader;
}

void Cube::loadShaders(std::string vertexShaderSrc, std::string fragmentShaderSrc){
    shaderProgram = glCreateProgram();
    int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str(), vertexShaderSrc.length());
    __android_log_write(ANDROID_LOG_INFO, "loadShaders", "vertex shader compiled");
    int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str(), fragmentShaderSrc.length());
    __android_log_write(ANDROID_LOG_INFO, "loadShaders", "fragment shader compiled");
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE){
        char log[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
        __android_log_write(ANDROID_LOG_ERROR, "loadShaders", log);
    }

    getLocations();
}

void Cube::getLocations() {
    glUseProgram(shaderProgram);
    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "texCoords");
    location_trans = glGetUniformLocation(shaderProgram, "trans");
    location_proj = glGetUniformLocation(shaderProgram, "proj");
    location_rot = glGetUniformLocation(shaderProgram, "rot");
    location_scale = glGetUniformLocation(shaderProgram, "scale");
    location_boxTexture = glGetUniformLocation(shaderProgram, "boxTexture");
    glUseProgram(0);
}

void Cube::loadTexture(const void *pixels, int width, int height) {
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Cube::setProjectionMatrix(float *projectionMatrix) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(location_proj, 1, GL_TRUE, projectionMatrix);
    glUseProgram(0);
}
