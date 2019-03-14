#include <iostream>
#define SDL_MAIN_HANDLED //https://stackoverflow.com/questions/32342285/undefined-reference-to-winmain16-c-sdl-2/32343111#32343111
#include <SDL2/SDL.h>


void display(){
    SDL_Init(SDL_INIT_EVERYTHING);
}

int main(int argc, char* argv[]){
    std::cout << "Hello world!" << std::endl;
    display();
    return 0;
}