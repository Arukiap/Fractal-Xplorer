#ifndef MESH_H
#define MESH_H
#define GLEW_STATIC

#include <glm/glm.hpp>
#include <GL/glew.h>

class Vertex {
    public:
        Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
        {
            this->pos = pos;
            this->texCoord = texCoord;
        }

        inline glm::vec3* GetPos() {return &pos;}
        inline glm::vec2* GetTextCoord() {return &texCoord;}
    private:
        glm::vec3 pos;
        glm::vec2 texCoord;
};

class Mesh{
    public:
        Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

        void Draw();

        virtual ~Mesh();
    private:
        enum{
            POSITION_VB,
            TEXCOORD_VB,

            INDEX_VB,

            NUM_BUFFERS
        };

        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[NUM_BUFFERS];
        unsigned int drawCount;
};

#endif // MESH_H