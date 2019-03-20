#include "objloader.h"
#include <fstream>
#include <iostream>

#define MAXWORDLENGTH 256

OBJFile::OBJFile(const char* fileName){
    FILE * file = fopen(fileName,"r");
    if(file == NULL){
        std::cerr << ".OBJ file read failed" << std::endl;
    }

    while(true){
        char lineHeader[MAXWORDLENGTH];
        int word = fscanf(file,"%s",lineHeader);
        if (word == EOF){
            break;
        }
        if(strcmp(lineHeader,"v") == 0){ //is vertex
            glm::vec3 vertex;
            fscanf(file,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
            vertices.push_back(vertex);
        } else if(strcmp(lineHeader,"vt") == 0){ //is texture
            glm::vec2 uv;
            fscanf(file,"%f %f\n",&uv.x,&uv.y);
            uvs.push_back(uv);
        } else if(strcmp(lineHeader,"vn") == 0){ //is normal
            glm::vec3 normal;
            fscanf(file,"%f %f %f\n",&normal.x,&normal.y,&normal.z);
            normals.push_back(normal);
        } else if(strcmp(lineHeader,"f") == 0){ //is face
            std::string v1, v2, v3;
            unsigned int vIndex[3],uvIndex[3],normalIndex[3];
            int matches = fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                        &vIndex[0],
                                        &uvIndex[0],
                                        &normalIndex[0],
                                        &vIndex[1],
                                        &uvIndex[1],
                                        &normalIndex[1],
                                        &vIndex[2],
                                        &uvIndex[2],
                                        &normalIndex[2]);
            if(matches != 9){
                std::cerr << "error reading .OBJ file (faces parsing error, try exporting with other options)" << std::endl;
            }
            vertexIndices.push_back(vIndex[0]);
            vertexIndices.push_back(vIndex[1]);
            vertexIndices.push_back(vIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    
    }

    for(unsigned int i=0;i<vertexIndices.size();i++){
        vertexIndices[i] -= 1;
    }

}