#include <map>
#include <list>

#include "face.h"

// Функция расчета нормали к грани
// [in] firstPoint - точка №1
// [in] secondPoint - точка №2
// [in] thirdPoint - точка №3
vec3* getNormal(const vec3* firstPoint, const vec3* secondPoint, const vec3* thirdPoint) {
	// Находим вектора поверхности
	vec3 firstVector(firstPoint->x - secondPoint->x, firstPoint->y - secondPoint->y, firstPoint->z - secondPoint->z);
	vec3 secondVector(firstPoint->x - thirdPoint->x, firstPoint->y - thirdPoint->y, firstPoint->z - thirdPoint->z);

	// Найдем нормаль при помощи векторного умножения
	float x = firstVector.y * secondVector.z - firstVector.z * secondVector.y;
	float y = firstVector.z * secondVector.x - firstVector.x * secondVector.z;
	float z = firstVector.x * secondVector.y - firstVector.y * secondVector.x;

	// Нормируем вектор
	float length = sqrt(x * x + y * y + z * z);

	return new vec3(x / length, y / length, z / length);
}

//
// Calculate smooth (average) per-vertex normals
//
// [out] normals - output per-vertex normals
// [in] verts - input per-vertex positions
// [in] faces - triangles (triplets of vertex indices)
// [in] nverts - total number of vertices (# of elements in verts and normals arrays)
// [in] nfaces - total number of faces (# of elements in faces array)
//
void calc_mesh_normals(vec3* normals, const vec3* verts, const int* faces, int nverts, int nfaces) {
	// Создаем контейнер для хранения нормалей к граням, содержащим вершину
    std::multimap<const vec3* /* вершина */, Face /* грань */> vertexAttachment;
    std::list<Face*> facesList;

    // Проходим по граням, собираем их в vertexAttachment
    for (int i = 0; i < nfaces * 3; i += 3) {
        Face* face = new Face(verts[faces[i]], verts[faces[i + 1]], verts[faces[i + 2]]);

        facesList.push_front(face);
        vertexAttachment.insert(std::pair<const vec3*, Face>(&verts[faces[i]], *face));
        vertexAttachment.insert(std::pair<const vec3*, Face>(&verts[faces[i + 1]], *face));
        vertexAttachment.insert(std::pair<const vec3*, Face>(&verts[faces[i + 2]], *face));
    }

    // Проходим по вершинам. Для каждой грани вершины смотрим, не пересекает ли её нормаль другие грани. Считаем нормаль как сумму нормалей граней, её включающих
    for (int i = 0; i < nverts; i++) {
        normals[i] = vec3(0,0,0);

        // отладочные переменные - TODO убрать
        vec3* tmp = &normals[i];
        const vec3 *curVec = &verts[i];

        std::pair <std::multimap<const vec3*, Face>::iterator, std::multimap<const vec3*, Face>::iterator> vertsList = vertexAttachment.equal_range(curVec);
        for (std::multimap<const vec3*, Face>::iterator it = vertsList.first; it !=vertsList.second; ++it) {
            for (Face* const& curFace : facesList) {
                it->second.crossCheck(*curFace);
            }

            it->second.rotateNormal();
            normals[i] += it->second.getNormal();
        }
    }
}

int main() {
    vec3 normals[] = { vec3(), vec3(), vec3(), vec3(), vec3() };
    const vec3 verts[] = { vec3(2,0,2),
                           vec3(5,3,0),
                           vec3(7,0,2),
                           vec3(4,1,0),
                           vec3(4,0,7) };

    const int faces[] = {	0, 1, 4,    //Грань №1
                            1, 2, 4,    //Грань №2
                            2, 3, 4,    //Грань №3
                            3, 0, 4,    //Грань №4
                            0, 1, 3,    //Грань №5
                            1, 2, 3     //Грань №6
						};

    calc_mesh_normals(normals, verts, faces, 5, 4);
    std::cout << normals[4].x << "; " << normals[4].y << "; " << normals[4].z << std::endl;

	system("pause");
}
