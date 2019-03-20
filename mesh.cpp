#include "mesh.h"
#include <vector>

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices){
    drawCount = numIndices;

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;

    positions.reserve(numVertices);
    texCoords.reserve(numVertices);

    for(unsigned int i = 0; i < numVertices; i++){
        positions.push_back(*vertices[i].GetPos());
        texCoords.push_back(*vertices[i].GetTextCoord());
    }

    //position buffer

    glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER,numVertices*sizeof(positions[0]),&positions[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    //texture buffer

    glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER,numVertices*sizeof(texCoords[0]),&texCoords[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,numIndices*sizeof(indices[0]),&indices[0],GL_STATIC_DRAW);


    glBindVertexArray(0);

}

Mesh::Mesh(const std::string& fileName){

}

Mesh::Mesh(const OBJFile& mesh){

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    //position buffer

    glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER,mesh.vertices.size()*sizeof(mesh.vertices[0]),&mesh.vertices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    //texture buffer

    glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER,mesh.uvs.size()*sizeof(mesh.uvs[0]),&mesh.uvs[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh.vertexIndices.size()*sizeof(mesh.vertexIndices[0]),&mesh.vertexIndices[0],GL_STATIC_DRAW);


    glBindVertexArray(0);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::Draw(){
    glBindVertexArray(vertexArrayObject);

    glDrawElements(GL_TRIANGLES,drawCount,GL_UNSIGNED_INT,0);

    glBindVertexArray(0);
}