#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

class Texture{
    public:
        Texture(const std::string& fileName);

        void Bind();

        virtual ~Texture();
    private:
        GLuint texture;
};

#endif //TEXTURE_H