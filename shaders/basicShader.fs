#version 410 core

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;

void main(){
    if(vTexCoord.x > 0.5){
        gl_FragColor = vec4(1.0,1.0,0.5,1.0);
    } else{
        gl_FragColor = vec4(0.0,1.0,0.5,1.0);
    }
    
}