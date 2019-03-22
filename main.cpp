#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif
int main(int argc, char* argv[]){

    Display display(800,800,"Hello world!");

<<<<<<< HEAD
    Shader shader("." SEPARATOR "shaders" SEPARATOR "basicShader");
    Texture texture("." SEPARATOR "res" SEPARATOR "mosaic.jpg");
=======
    Shader shader(".\\shaders\\basicShader");
>>>>>>> parent of b6a5c0b... Add 2d texture mapping to mesh

    Vertex vertices[] = { Vertex(glm::vec3(-0.5,-0.5,0)),
                          Vertex(glm::vec3(0,0.5,0)),
                          Vertex(glm::vec3(0.5,-0.5,0))};

    Mesh mesh(vertices,sizeof(vertices)/sizeof(vertices[0]));

    while(!display .IsClosed()){
        display.Clear(0.0f,0.15f,0.3f,1.0f);
        shader.Bind();
        mesh.Draw();

        display.Update();
    }

    return 0;
}