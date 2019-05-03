#ifndef MOUSE_H
#define MOUSE_H

class Mouse {
    public:
        Mouse(float x, float y);
        void moveMouse(float x, float y);
        float GetAngleX(){ return x;}
        float GetAngleY(){ return y;}
    private:
        float x,y;
};

#endif // MOUSE_H