#include <iostream>
#define SDL_MAIN_HANDLED //https://stackoverflow.com/questions/32342285/undefined-reference-to-winmain16-c-sdl-2/32343111#32343111
#include <SDL2/SDL.h>


void display(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Janela fofa",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,800,SDL_WINDOW_OPENGL);
    SDL_Delay(3000);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]){
    display();
    return 0;
}