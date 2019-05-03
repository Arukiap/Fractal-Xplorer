#version 410 core

attribute vec3 pos;
attribute vec2 texCoord;

varying vec3 vPos;
varying vec2 vTexCoord;
varying float vSystemTime;
varying vec2 vSystemResolution;
varying vec3 vCamera_pos;
varying vec2 vMouse_delta;

uniform float systemTime;
uniform vec2 systemResolution;
uniform lowp vec3 camera_pos;
uniform lowp vec2 mouse_delta;

void main(){
    gl_Position = vec4(pos,1.0);
    vTexCoord = texCoord;
    vSystemTime = systemTime;
    vSystemResolution = systemResolution;
    vCamera_pos = camera_pos;
    vMouse_delta = mouse_delta;
}