#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "display.h"



int main(int argc, char* argv[]){

    Display display(800,800,"Hello world!");

    while(!display.IsClosed()){
        display.Clear(0.0f,0.15f,0.3f,1.0f);
        display.Update();
    }

    return 0;
}