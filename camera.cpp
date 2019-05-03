#include "camera.h"

Camera::Camera(float x, float y, float z, float speed){
    this->x = x;
    this->y = y; 
    this->z = z;
    this->speed = speed;
}

void Camera::Reset(){
    this->x = this->y = this->z = 0;
}

void Camera::Update(float x, float y, float z){
    this->x = x;
    this->y = y; 
    this->z = z;
}

void Camera::OffsetUpdate(float x, float y, float z){
    this->x += x * this->speed;
    this->y += y * this->speed; 
    this->z += z * this->speed;
}