#include "leaf.h"

void appending(std::vector<float>& v, glm::vec3 a, glm::vec2 b){
    v.push_back(a.x);
    v.push_back(a.y);
    v.push_back(a.z);
    v.push_back(50 * b.x);
    v.push_back(50 * b.y);
}

void leaf_cord(glm::vec3 start, glm::vec3 leaf_start, glm::vec3 normal1, glm::vec3 normal2, std::vector<float>& vertices){
    appending(vertices, start, glm::vec2(0.5f, 0.5f));

    appending(vertices, leaf_start, glm::vec2(0.5f, 0.5f));

    for(float i = 0; i<3.14; i+=0.1f){
        glm::vec3 v1 = leaf_start + 10 * i * normal1 + 10 * sinf(i) * normal2;
        glm::vec3 v2 = leaf_start + 10 * i * normal1 - 10 * sinf(i) * normal2;

        appending(vertices, v1, glm::vec2(i/3.14f, 0.5 + sinf(i)));

        appending(vertices, v2, glm::vec2(i/3.14f, 0.5f - sinf(i)));
    }
}
std::vector<float> Leaf::generateVertices() {
    glm::vec3 cyl_dir = normalize(b - a);
    std::vector<float> vertices;
    for(int i = 0; i<5; i++){
        glm::vec3 start = a + float((double) rand() / (RAND_MAX)) * (b-a);
        glm::vec3 arb_dir = 2.0f * glm::vec3(float((double) rand() / (RAND_MAX)), float((double) rand() / (RAND_MAX)), float((double) rand() / (RAND_MAX))) - 1.0f;
        arb_dir = normalize(arb_dir);

        glm::vec3 normal1 =  arb_dir;
        glm::vec3 normal2 = cross(cyl_dir, normal1);

        float width = (cyl_top_rad * distance(a, start) + cyl_bottom_rad * distance(b, start)) / (distance(a, b));

        glm::vec3 leaf_start = start + width * normal1;

        leaf_cord(start, leaf_start, normal1, normal2, vertices);
    }

    return vertices;

}