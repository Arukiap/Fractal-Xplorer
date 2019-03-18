#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include "transform.h"


class Shader{
    public:
        Shader(const std::string& fileName);
        void Bind();
        void Update(const Transform& transform);
        virtual ~Shader();
    private:
        static const unsigned int NUM_SHADERS = 2; //Vertex and Fragment shaders

        enum{
            TRANSFORM_U,

            NUM_UNIFORMS
        };

        GLuint program;
        GLuint shaders[NUM_SHADERS];
        GLuint uniforms[NUM_UNIFORMS];
};


#endif // SHADER_H