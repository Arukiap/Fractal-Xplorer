#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

#define WIDTH 800
#define HEIGHT 800

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif
int main(int argc, char* argv[]){

    Display display(WIDTH,HEIGHT,"Hello world!");

    Shader shader("." SEPARATOR "shaders" SEPARATOR "basicShader");
    Texture texture("." SEPARATOR "res" SEPARATOR "mosaic.jpg");

    Vertex vertices[] = { Vertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0)),
                          Vertex(glm::vec3(0,0.5,0), glm::vec2(0.5,1)),
                          Vertex(glm::vec3(0.5,-0.5,0), glm::vec2(1,0))};

    Mesh mesh(vertices,sizeof(vertices)/sizeof(vertices[0]));

    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f),70.0f,(float)WIDTH/(float)HEIGHT,0.1f,100.0f);

    Transform transform;

    float counter = 0.0f;

    while(!display .IsClosed()){
        display.Clear(0.0f,0.15f,0.3f,1.0f);

        transform.GetPos().z = sinf(counter);
        //transform.GetRot().z = cosf(counter);
        //transform.GetScale().x = sinf(counter);

        shader.Bind();
        texture.Bind();
        shader.Update(transform,camera);
        mesh.Draw();
        display.Update();
        counter += 0.001f;
    }

    return 0;
}