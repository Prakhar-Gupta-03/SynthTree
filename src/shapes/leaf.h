#include <bits/stdc++.h>
#include "globals.hpp"
#include "utils/utils.hpp"


class Leaf{
private:
    glm::vec3 a, b;
    float cyl_bottom_rad, cyl_top_rad;
public:
    Leaf(glm::vec3 a_, glm::vec3 b_, float cyl_bottom_rad_, float cyl_top_rad_): a(a_), b(b_), cyl_bottom_rad(cyl_bottom_rad_), cyl_top_rad(cyl_top_rad_) {}
    std::vector<float> generateVertices();
};