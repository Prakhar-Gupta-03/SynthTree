#include "cylinder.h"

void append(std::vector<float>& v, glm::vec3 a, glm::vec2 b){
    v.push_back(a.x);
    v.push_back(a.y);
    v.push_back(a.z);
    v.push_back(50 * b.x);
    v.push_back(50 * b.y);
}

std::vector<float> Cylinder::generateTriangles(){
    float PI = 3.14;
    std::vector<float> v;
    float segments = 5.0f;
    for(float i = 0.0f; i<segments; i++){
        float theta1 = 2 * i*PI/segments;
        float theta2 = 2 * (i+1)*PI/segments;
        glm::vec3 v1_bottom = glm::vec3(a.x + radius_bottom*cos(theta1), a.y, a.z + radius_bottom*sin(theta1));
        glm::vec2 v1_bottom_uv = glm::vec2(theta1/(2*PI), 0.0f);
        glm::vec3 v2_bottom = glm::vec3(a.x + radius_bottom*cos(theta2), a.y, a.z + radius_bottom*sin(theta2));
        glm::vec2 v2_bottom_uv = glm::vec2(theta2/(2*PI), 0.0f);

        glm::vec3 v1_top = glm::vec3(b.x + radius_top*cos(theta1), b.y, b.z + radius_top*sin(theta1));
        glm::vec2 v1_top_uv = glm::vec2(theta1/(2*PI), 1.0f);
        glm::vec3 v2_top = glm::vec3(b.x + radius_top*cos(theta2), b.y, b.z + radius_top*sin(theta2));
        glm::vec2 v2_top_uv = glm::vec2(theta2/(2*PI), 1.0f);

        append(v, v1_bottom, v1_bottom_uv);
        append(v, v2_bottom, v2_bottom_uv);
        append(v, a, glm::vec2(0.5f, 0.5f));

        append(v, v1_top, v1_top_uv);
        append(v, v2_top, v2_top_uv);
        append(v, b, glm::vec2(0.5f, 0.5f));

        append(v, v1_bottom, v1_bottom_uv);
        append(v, v2_bottom, v2_bottom_uv);
        append(v, v1_top, v1_top_uv);

        append(v, v2_bottom, v2_bottom_uv);
        append(v, v2_top, v2_top_uv);
        append(v, v1_top, v1_top_uv);

    }

    return v;
    
}