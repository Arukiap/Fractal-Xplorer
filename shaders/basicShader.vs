#version 120

attribute vec3 pos;
attribute vec2 texCoord;

varying vec2 texCoord0;

void main(){
    gl_Position = vec4(pos,1.0);
    texCoord0 = texCoord;
}