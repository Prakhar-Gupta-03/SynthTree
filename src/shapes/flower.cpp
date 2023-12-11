#include "flower.h"

void f(std::vector<float>& v, glm::vec3 a, glm::vec2 b){
    v.push_back(a.x);
    v.push_back(a.y);
    v.push_back(a.z);
    v.push_back(50 * b.x);
    v.push_back(50 * b.y);
}

void flower_cord(glm::vec3 start, glm::vec3 flower_start, glm::vec3 normal1, glm::vec3 normal2, std::vector<float>& vertices){
    f(vertices, start, glm::vec2(0.5f, 0.5f));

    f(vertices, flower_start, glm::vec2(0.5f, 0.5f));

    for(float i = 0; i<3.14; i+=0.1f){
        glm::vec3 v1 = flower_start + 3 * i * normal1 + 2.25f * sinf(i) * normal2;
        glm::vec3 v2 = flower_start + 3 * i * normal1 - 2.25f * sinf(i) * normal2;

        f(vertices, v1, glm::vec2(i/3.14f, 0.5 + sinf(i)));

        f(vertices, v2, glm::vec2(i/3.14f, 0.5f - sinf(i)));
    }
}
std::vector<float> Flower::generateVertices() {
    glm::vec3 cyl_dir = normalize(b - a);
    std::vector<float> vertices;
    for(int i = 0; i<1; i++){
        glm::vec3 start = a + float((double) rand() / (RAND_MAX)) * (b-a);
        glm::vec3 arb_dir = 2.0f * glm::vec3(float((double) rand() / (RAND_MAX)), float((double) rand() / (RAND_MAX)), float((double) rand() / (RAND_MAX))) - 1.0f;
        arb_dir = normalize(arb_dir);

        glm::vec3 normal1 =  arb_dir;
        glm::vec3 normal2 = cross(cyl_dir, normal1);
        glm::vec3 normal3 = cross(normal1, normal2);

        float width = (cyl_top_rad * distance(a, start) + cyl_bottom_rad * distance(b, start)) / (distance(a, b));

        glm::vec3 flower_start = start + width * normal1;

        flower_cord(start, flower_start, normal2, normal3, vertices);
        flower_cord(start, flower_start, -normal2, normal3, vertices);
        flower_cord(start, flower_start, normal3, normal2, vertices);
        flower_cord(start, flower_start, -normal3, normal2, vertices);

        flower_cord(start, flower_start, normalize(normal2 + normal3), normal3, vertices);
        flower_cord(start, flower_start, normalize(normal2 - normal3), normal3, vertices);
        flower_cord(start, flower_start, normalize(-normal2 + normal3), normal3, vertices);
        flower_cord(start, flower_start, normalize(-normal2 - normal3), normal3, vertices);

    }

    return vertices;

}