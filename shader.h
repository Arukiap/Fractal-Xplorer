#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include "transform.h"
#include "camera.h"

class Shader{
    public:
        Shader(const std::string& fileName);
        void Bind();
        void Update(const Transform& transform, const Camera& camera);
        virtual ~Shader();
    private:
        static const unsigned int NUM_SHADERS = 2; //Vertex and Fragment shaders
        static const unsigned int NUM_UNIFORMS = 2;

        GLuint program;
        GLuint shaders[NUM_SHADERS];
        GLuint uniforms[NUM_UNIFORMS];
};


#endif // SHADER_H