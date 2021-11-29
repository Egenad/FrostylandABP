#ifndef VECTOR3F_H
#define VECTOR3F_H


class Vector3f
{
    public:
        Vector3f();
        Vector3f(float, float);
        Vector3f(float, float, float);
        Vector3f(const Vector3f&);
        virtual ~Vector3f();
        float getX() const;
        float getY() const;
        float getZ() const;
        void setX(float);
        void setY(float);
        void setZ(float);
        void changeAllValues(float, float, float);
    private:
        float _x;
        float _y;
        float _z;
};

#endif // VECTOR3F_H
