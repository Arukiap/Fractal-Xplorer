#version 410 core

attribute vec3 pos;
attribute vec2 texCoord;

varying vec3 vPos;
varying vec2 vTexCoord;
varying float vSystemTime;
varying vec2 vSystemResolution;

uniform float systemTime;
uniform vec2 systemResolution;

void main(){
    gl_Position = vec4(pos,1.0);
    vTexCoord = texCoord;
    vSystemTime = systemTime;
    vSystemResolution = systemResolution;
}