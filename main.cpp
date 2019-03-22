#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif
int main(int argc, char* argv[]){

    Display display(800,800,"Hello world!");

    Shader shader("." SEPARATOR "shaders" SEPARATOR "basicShader");
    Texture texture("." SEPARATOR "res" SEPARATOR "mosaic.jpg");

    Vertex vertices[] = { Vertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0)),
                          Vertex(glm::vec3(0,0.5,0), glm::vec2(0.5,1)),
                          Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1,0))};

    Mesh mesh(vertices,sizeof(vertices)/sizeof(vertices[0]));

    while(!display .IsClosed()){
        display.Clear(0.0f,0.15f,0.3f,1.0f);
        shader.Bind();
        texture.Bind();
        mesh.Draw();

        display.Update();
    }

    return 0;
}