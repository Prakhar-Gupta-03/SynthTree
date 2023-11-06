#include "utils/vector3D.h"
#include "state/state.h"
#include <bits/stdc++.h>

class Turtle{
private:
    State current_state;
    std::stack<State> states;
public:
    Turtle(State s): current_state(s) {}
    
    std::vector<float> parser(std::vector<std::string> modules);
    void saveState();
    void restoreState();
    void Ru(double angle);
    void Rh(double angle);
    void Rl(double angle);
    void changeWidth(double width);
    void forward(double distance, std::vector<float>& vertices);
};