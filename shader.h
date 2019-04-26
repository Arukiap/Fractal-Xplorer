#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "camera.h"


class Shader{
    public:
        Shader(const std::string& fileName, unsigned const int displayHeight, unsigned const int displayWidth);
        void Bind();
        void UpdateTime(unsigned const int time);
        void UpdateCamera(Camera camera);
        virtual ~Shader();
    private:
        static const unsigned int NUM_SHADERS = 2; //Vertex and Fragment shaders
        unsigned int height;
        unsigned int width;
        GLuint program;
        GLuint shaders[NUM_SHADERS];
};


#endif // SHADER_H