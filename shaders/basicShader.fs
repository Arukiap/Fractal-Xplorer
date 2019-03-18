#version 120

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse; //uniform means cpu can write to this var and gpu read it

void main(){
    //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    gl_FragColor = texture2D(diffuse,texCoord0);
}