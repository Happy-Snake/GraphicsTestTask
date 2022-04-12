#include "face.h"

Face::Face(vec3 a, vec3 b, vec3 c) :
    _normal(),
    _crossCount(0),
    _vertexA(a), _vertexB(b), _vertexC(c)
{
    // -------- Находим нормаль ---------
    // Находим вектора поверхности
    vec3 firstVector(a.x - b.x, a.y - b.y, a.z - b.z);
    vec3 secondVector(a.x - c.x, a.y - c.y, a.z - c.z);

    // Найдем нормаль при помощи векторного умножения
    _A = firstVector.y * secondVector.z - firstVector.z * secondVector.y;
    _B = firstVector.z * secondVector.x - firstVector.x * secondVector.z;
    _C = firstVector.x * secondVector.y - firstVector.y * secondVector.x;

    // Нормируем вектор
    float length = sqrt(_A * _A + _B * _B + _C * _C);

    _normal = vec3(_A / length, _B / length, _C / length);


    // -------- Находим D уравнения поверхности -------
    _D = - (_A * _vertexA.x + _B * _vertexA.y + _C * _vertexA.z);


    // -------- Находим точку внутри грани, из которой будет исходить вектор -------
    // В качестве искомой точки возьмем центр вписанной окружности, т.к. такая точка есть в люом треугольнике и она всегда внутри него

    // Находим проекцию на Оxy
    vec2 triangleA(_vertexA.x, _vertexA.y);
    vec2 triangleB(_vertexB.x, _vertexB.y);
    vec2 triangleC(_vertexC.x, _vertexC.y);

    // Находим стороны проекции
    float sideA = sqrt(pow(triangleB.x - triangleC.x, 2) + pow(triangleB.y - triangleC.y, 2));
    float sideB = sqrt(pow(triangleC.x - triangleA.x, 2) + pow(triangleC.y - triangleA.y, 2));
    float sideC = sqrt(pow(triangleB.x - triangleA.x, 2) + pow(triangleB.y - triangleA.y, 2));

    // Находим периметр
    float p = sideA + sideB + sideC;

    // Находим координаты центра вписанной окружности
    float x = (sideA * triangleA.x + sideB * triangleB.x + sideC * triangleC.x) / p;
    float y = (sideA * triangleA.y + sideB * triangleB.y + sideC * triangleC.y) / p;

    // Находим координату z, лежащую на грани
    float z = - (_A * x + _B * y + _D) / _C;

    _point = vec3(x, y, z);
}


// Определить, пересекает ли нормаль, проведенная из точки, плоскость
// [in] normal - рассматриваемая нормаль
// [in] point - точка начала нормали
// [in] face - нормаль к пересекаемой (или не пересекаемой) грани
bool Face::crossCheck(Face face) {
    if ((_normal.x == 0 && _normal.y == 0 && _normal.z == 0) ||
        (face.getNormal().x == 0 && face.getNormal().y == 0 && face.getNormal().z == 0)) {
        std::cout << "ERROR - normal can't be a zero vector!";
        exit(1);
    }

    if (*this == face)
        return false;

    // Определяем, направлен ли вектор в сторону плоскости грани
    float t = -(face.A() * _point.x + face.B() * _point.y + face.C() * _point.z + face.D()) /
            (_A * face.A() + _B * face.B() + _C * face.C());

     //Если t меньше нуля, вектор не пересекает плоскость
     if (t < 0) return 0;

     // Найдем точку пересечения вектора и плоскости
     vec3 crossPoint(_point.x + _A * t, _point.y + _B * t, _point.z + _C * t);

     // Проверим, лежит ли точка внутри грани - если расстояние от точки пересечения любой из вершин больше
     float firstVector = (face.vertexA().x - crossPoint.x) * (face.vertexB().y - face.vertexA().y) - (face.vertexB().x - face.vertexA().x) * (face.vertexA().y - crossPoint.y);
     float secondVector = (face.vertexB().x - crossPoint.x) * (face.vertexC().y - face.vertexB().y) - (face.vertexC().x - face.vertexB().x) * (face.vertexB().y - crossPoint.y);
     float thirdVector = (face.vertexC().x - crossPoint.x) * (face.vertexA().y - face.vertexC().y) - (face.vertexA().x - face.vertexC().x) * (face.vertexC().y - crossPoint.y);

     if ((firstVector < 0 && secondVector < 0 && thirdVector < 0) ||
             (firstVector > 0 && secondVector > 0 && thirdVector > 0)) {
         // Пересечение плоскости
          _crossCount++;
         return true;
     } else {
         return false;
     }
}

// При необходимости развернуть нормаль
bool Face::rotateNormal()
{
    // Если количество пересечений нечетное - нужно отразить нормаль
    if (_crossCount % 2 != 0) {
        _normal.x *= -1;
        _normal.y *= -1;
        _normal.z *= -1;

        return true;
    }
    return false;
}

bool operator== (const Face &f1, const Face &f2)
{
    return (f1._vertexA == f2._vertexA &&
            f1._vertexB == f2._vertexB &&
            f1._vertexC == f2._vertexC);
}
