#include "utils/vector3D.h"
#include "state/state.h"
#include <bits/stdc++.h>
#include "globals.hpp"
#include "utils/utils.hpp"
#include "shapes/cylinder.h"


class Turtle{
private:
    State current_state;
    std::stack<State> states;
    float leaf_threshold;
public:
    Turtle(State s, float leaf_threshold_): current_state(s), leaf_threshold(leaf_threshold_) {}
    
    std::vector<std::vector<float>> parser(std::vector<std::string> modules);
    void saveState();
    void restoreState();
    void Ru(double angle);
    void Rh(double angle);
    void Rl(double angle);
    void changeWidth(double width);
    void forward(double distance, std::vector<float>& vertices, std::vector<float>& leaf_vertices);
};