#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>


class Shader{
    public:
        Shader(const std::string& fileName);
        void Bind();
        virtual ~Shader();
    private:
        static const unsigned int NUM_SHADERS = 2; //Vertex and Fragment shaders
        GLuint program;
        GLuint shaders[NUM_SHADERS];
};


#endif // SHADER_H