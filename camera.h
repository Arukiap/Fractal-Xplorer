#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL //GLM: GLM_GTX_transform is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it.
#include "glm/gtx/transform.hpp"

class Camera{
    public:
        Camera(const glm::vec3 pos, float fov, float aspect, float zNear, float zFar){
            projection = glm::perspective(fov,aspect,zNear,zFar);
            position = pos;
            forward = glm::vec3(0.0f,0.0f,-1.0f);
            up = glm::vec3(0.0f,1.0f,0.0f);
        }
        
        inline glm::mat4 GetViewProjection() const {
            return projection * glm::lookAt(position,position + forward,up);
        }
    private:
        glm::mat4 projection;
        glm::vec3 position;
        glm::vec3 forward; //where camera is pointing
        glm::vec3 up; //what direction is up?
};

#endif //CAMERA_H