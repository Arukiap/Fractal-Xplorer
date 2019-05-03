#include "display.h"
#define GLEW_STATIC
#include "GL/glew.h"
#include <iostream>

Display::Display(int width, int height, const std::string& title){

    mWidth = width;
    mHeight = height;

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

void Display::Update(Camera *camera, float offset, Mouse *mouse){
    SDL_GL_SwapWindow(window);

    SDL_Event e; //OS event

    bool mousefirst  = true;
    bool mouseMotion = false;

    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            isClosed = true;
        }
        switch( e.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
                //printf("Key down\n");
                /* Check the SDLKey values and move change the coords */
                switch( e.key.keysym.sym ){
                    case SDLK_a:
                        camera->OffsetUpdate(-offset,0.0,0.0);
                        break;
                    case SDLK_d:
                        camera->OffsetUpdate(offset,0.0,0.0);
                        break;
                    case SDLK_w:
                        camera->OffsetUpdate(0.0,0.0,offset);
                        break;
                    case SDLK_s:
                        camera->OffsetUpdate(0.0,0.0,-offset);                        
                        break;
                    case SDLK_q:
                        camera->OffsetUpdate(0.0,offset,0.0);                        
                        break;
                    case SDLK_z:
                        camera->OffsetUpdate(0.0,-offset,0.0);                        
                        break;    
                    default:
                        break;
                }
            break;
            case SDL_MOUSEMOTION:
                mouse->moveMouse(e.motion.xrel,e.motion.yrel);
                break;
            break;
        }

    }

}

