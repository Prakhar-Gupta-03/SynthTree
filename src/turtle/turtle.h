#include "utils/vector3D.h"
#include "state/state.h"
#include <stack>

class Turtle{
private:
    State current_state;
    std::stack<State> states;
public:
    Turtle(State s): current_state(s) {}
    
    void change_state(State s){
        states.push(current_state);
        current_state = s;
    }

    void restore_state(){
        current_state = states.top();
        states.pop();
    }
};