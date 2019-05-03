#include "display.h"
#define GLEW_STATIC
#include "GL/glew.h"
#include <iostream>

Display::Display(int width, int height, const std::string& title){

    this->isPressedW = false;
    this->isPressedS = false;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    //32 bit color + transparency
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    //Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window =  SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_OPENGL);
    

    //Gpu connects directly to the window, instead of being the OS in complete command of the window
    glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();

    if(status != GLEW_OK){
        std::cerr << "Glew failed to initialize." << std::endl;
    }

    isClosed = false;
}

Display::~Display(){
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Display::IsClosed(){
    return isClosed;
}

void Display::Update(){
    SDL_GL_SwapWindow(window);
}

void Display::ListenInput(Camera *camera, Mouse *mouse){
    SDL_Event e; //OS event

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if(keystate[SDL_SCANCODE_W]){
        camera->OffsetUpdate(0.0,0.0,1);        
    }

    if(keystate[SDL_SCANCODE_S]){
        camera->OffsetUpdate(0.0,0.0,-1);         
    }

    while(SDL_PollEvent(&e)){
        switch( e.type ){
            case SDL_MOUSEMOTION:
                mouse->moveMouse(e.motion.xrel,e.motion.yrel);
                break;
            case SDL_QUIT:
                isClosed = true;
            default: break;
            break;
        }

    }

}



