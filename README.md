# Fractal Xplorer

Fractal Xplorer is a shader-based interactive 3D fractal generator.

It is written in C++, using the OpenGL API and GLSL shading language.

# Compiling and Running - For programmers

In order to compile and run, run the following command in your console according to your operating system:

## Windows users:
```
g++ -g *.cpp -I include\\ -Llib -lglew32s -lSDL2main -lSDL2 -o fractals.exe -lopengl32
```
## Linux users:
```
g++ -g *.cpp -I include/ -Llib -lGLEW -lSDL2main -lSDL2 -o fractals.out -lOpenGL
```

Note: While the windows libraries are all included in this project, you will need to install those refered in the command for linux system if you are running this on linux. These are GLEW, SDL2 and OpenGL.

Alternatively, you can also compile and run in a simpler manner if you are running Visual Studio Code. Just open the project on VSCode and press CTRL+Shift+B. This will run an automated task to compile and run the project immediately.
