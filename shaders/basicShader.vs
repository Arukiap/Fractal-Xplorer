#version 410 core

attribute vec3 pos;

varying float vSystemTime;
varying float vSelectedFractal;
varying vec2 vSystemResolution;
varying vec3 vCamera_pos;
varying vec2 vMouse_delta;

uniform float systemTime;
uniform float selectedFractal;
uniform vec2 systemResolution;
uniform lowp vec3 camera_pos;
uniform lowp vec2 mouse_delta;

void main(){
    gl_Position = vec4(pos,1.0);
    vSystemTime = systemTime;
    vSystemResolution = systemResolution;
    vCamera_pos = camera_pos;
    vMouse_delta = mouse_delta;
    vSelectedFractal = selectedFractal;
}