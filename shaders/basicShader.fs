#version 120

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;

void main(){
    gl_FragColor = vTexCoord.xyyx;
}