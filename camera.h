#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    public:
        Camera(float x,float y, float z);
        void Reset();
        void Update(float x,float y,float z);
        void OffsetUpdate(float x, float y, float z);
        float GetX(){
            return this->x;
        }
        float GetY(){
            return this->y;
        }
        float GetZ(){
            return this->z;
        }
    private:
        float x,y,z;
};

#endif // CAMERA_H