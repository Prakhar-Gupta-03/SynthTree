#include "utils/vector3D.h"
#include "color/color.h"
#include "globals.hpp"
#include <bits/stdc++.h>

class Cylinder{
private:
    glm::vec3 a, b;
    double radius_bottom, radius_top;
public:
    Cylinder(glm::vec3 a_, glm::vec3 b_, double radius_bottom_, double radius_top_):
        a(a_), b(b_), radius_bottom(radius_bottom_), radius_top(radius_top_) {}
    std::vector<float> generateTriangles();
};