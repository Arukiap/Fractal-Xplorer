#version 410 core

attribute vec3 pos;
attribute vec2 texCoord;

varying vec3 vPos;
varying vec2 vTexCoord;
varying float vSystemTime;

uniform float systemTime;

void main(){
    gl_Position = vec4(pos,1.0);
    vTexCoord = texCoord;
    vSystemTime = systemTime;
}