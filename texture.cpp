#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& fileName){
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(fileName.c_str(),&width,&height,&numComponents,4);

    if(imageData == NULL){
        std::cerr << "Texture loading failed for file " << fileName << std::endl;
    }

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT); //controls texture wrapping, repeats on wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT); 

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //linearly interpolate when texture has not the same resolution of the object
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,imageData);

    stbi_image_free(imageData);
}

Texture::~Texture(){
    glDeleteTextures(1,&texture);
}

void Texture::Bind(){
    glBindTexture(GL_TEXTURE_2D,texture);
}