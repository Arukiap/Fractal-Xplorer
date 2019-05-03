#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "mouse.h"


#define WIDTH 1000
#define HEIGHT 1000

#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

int main(int argc, char* argv[]){

    Display display(WIDTH,HEIGHT,"Fractal Explorer");

    Shader shader("." SEPARATOR "shaders" SEPARATOR "basicShader",HEIGHT,WIDTH);

    Vertex vertices[] = { Vertex(glm::vec3(-1.0,1.0,0),glm::vec2(0.0,0.0)),
                          Vertex(glm::vec3(1.0,1.0,0),glm::vec2(1.0,0.0)),
                          Vertex(glm::vec3(-1.0,-1.0,0),glm::vec2(0.0,1.0)),
                          Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,1.0))};

    Mesh mesh(vertices,sizeof(vertices)/sizeof(vertices[0]));

    Camera camera(0.0,0.0,-4.0,0.01);
    Mouse mouse(0.0,0.0);

    while(!display.IsClosed()){
        display.Clear(0.0f,0.15f,0.3f,1.0f);
        shader.Bind(); 
        shader.UpdateTime(SDL_GetTicks());
        shader.UpdateCamera(camera);
        shader.UpdateMouse(mouse);
        mesh.Draw();
        display.ListenInput(&camera,&mouse);
        display.Update();
    }

    return 0;
}