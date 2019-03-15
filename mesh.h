#ifndef MESH_H
#define MESH_H
#define GLEW_STATIC

#include <glm/glm.hpp>
#include <GL/glew.h>

class Vertex {
    public:
        Vertex(const glm::vec3& pos)
        {
            this->pos = pos;
        }
    private:
        glm::vec3 pos;
};

class Mesh{
    public:
        Mesh(Vertex* vertices, unsigned int numVertices);

        void Draw();

        virtual ~Mesh();
    private:
        enum{
            POSITION_VB,
            NUM_BUFFERS
        };

        GLuint vertexArrayObject;
        GLuint vertexArrayBuffers[NUM_BUFFERS];
        unsigned int drawCount;
};

#endif // MESH_H