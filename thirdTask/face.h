#ifndef POINT_H
#define POINT_H

#include <iostream>

#include "glm/glm.hpp"

using namespace glm;

class Face
{
public:
    Face(vec3 a, vec3 b, vec3 c);

    bool crossCheck(Face face);
    bool rotateNormal();

    vec3 vertexA() { return _vertexA; };
    vec3 vertexB() { return _vertexB; };
    vec3 vertexC() { return _vertexC; };

    float A() { return _A; };
    float B() { return _B; };
    float C() { return _C; };
    float D() { return _D; };

    vec3 getNormal() { return _normal; };

    friend bool operator== (const Face &f1, const Face &f2);

private:
    vec3 _point;        // Точка, из которой исходит нормаль
    vec3 _normal;       // Нормаль
    int _crossCount;    // Количество граней, которые нормаль пересекла

    // Вершины треугольника
    vec3 _vertexA;
    vec3 _vertexB;
    vec3 _vertexC;

    // Ax+By+Cz+D=0
    float  _A;
    float  _B;
    float  _C;
    float  _D;
};

#endif // POINT_H
