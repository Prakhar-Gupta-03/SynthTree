#include "cylinder.h"

std::vector<float> Cylinder::generateTriangles(){
    float PI = 3.141592653589793;
    std::vector<glm::vec3> vertices;
    float segments = 5.0f;
    for(float i = 0.0f; i<segments; i++){
        float theta1 = 2 * i*PI/segments;
        float theta2 = 2 * (i+1)*PI/segments;

        glm::vec3 v1_bottom = glm::vec3(a.x + radius_bottom*cos(theta1), a.y, a.z + radius_bottom*sin(theta1));
        glm::vec3 v2_bottom = glm::vec3(a.x + radius_bottom*cos(theta2), a.y, a.z + radius_bottom*sin(theta2));

        glm::vec3 v1_top = glm::vec3(b.x + radius_top*cos(theta1), b.y, b.z + radius_top*sin(theta1));
        glm::vec3 v2_top = glm::vec3(b.x + radius_top*cos(theta2), b.y, b.z + radius_top*sin(theta2));

        vertices.push_back(a);
        vertices.push_back(v1_bottom);
        vertices.push_back(v2_bottom);

        vertices.push_back(b);
        vertices.push_back(v1_top);
        vertices.push_back(v2_top);

        vertices.push_back(v1_bottom);
        vertices.push_back(v1_top);
        vertices.push_back(v2_top);

        vertices.push_back(v1_bottom);
        vertices.push_back(v2_bottom);
        vertices.push_back(v2_top);

    }

    std::vector<float> v;
    for(int i = 0; i<vertices.size(); i++){
        v.push_back(vertices[i].x);
        v.push_back(vertices[i].y);
        v.push_back(vertices[i].z);
    }

    return v;
    
}