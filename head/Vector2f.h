#ifndef VECTOR2F_H
#define VECTOR2F_H


class Vector2f
{
    public:
        Vector2f();
        Vector2f(float, float);
        Vector2f(const Vector2f&);
        Vector2f& operator=(Vector2f other);
        virtual ~Vector2f();
        float getX() const;
        float getY() const;
        void setX(float);
        void setY(float);
        void changeAllValues(float,float);
    private:
        float _x;
        float _y;
};

#endif // VECTOR2F_H
