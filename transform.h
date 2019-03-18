#ifndef TRANSFORM_H
#define TRANSFORM_H
#define GLM_ENABLE_EXPERIMENTAL //GLM: GLM_GTX_transform is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it.

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Transform{
    public:
        Transform(const glm::vec3& pos = glm::vec3(),
                  const glm::vec3& rot = glm::vec3(),
                  const glm::vec3& scale = glm::vec3(1.0f,1.0f,1.0f)
                 ):
                    pos(pos),
                    rot(rot),
                    scale(scale) {}

        //get model matrix
        inline glm::mat4 GetModel() const {
            glm::mat4 posMatrix = glm::translate(pos);    
            glm::mat4 scaleMatrix = glm::scale(scale);

            glm::mat4 rotXMatrix = glm::rotate(rot.x,glm::vec3(1,0,0));
            glm::mat4 rotYMatrix = glm::rotate(rot.y,glm::vec3(0,1,0));
            glm::mat4 rotZMatrix = glm::rotate(rot.z,glm::vec3(0,0,1));

            glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix; //not commutative, the order of this matters don't change

            return posMatrix * rotMatrix * scaleMatrix; //order matters still
        }

        inline glm::vec3& GetPos() {return pos;}
        inline glm::vec3& GetRot() {return rot;}
        inline glm::vec3& GetScale() {return scale;}

        inline void SetPos(const glm::vec3& a_pos) {pos = a_pos;}
        inline void SetRot(const glm::vec3& a_rot) {rot = a_rot;}
        inline void SetScale(const glm::vec3& a_scale) {scale = a_scale;}
    private:
        glm::vec3 pos;
        glm::vec3 rot; //can happen gimble lock
        glm::vec3 scale;
};

#endif //TRANSFORM_H