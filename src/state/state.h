#include "utils/vector3D.h"
#include "color/color.h"

class State{
private:
    Vector3D position;
    Vector3D H; // Heading
    Vector3D L; // Left
    Vector3D U; // Up
    Color color;
    double width;
public:
    State(Vector3D pos, Vector3D h, Vector3D l, Vector3D u, Color c, double w):
        position(pos), H(h), L(l), U(u), color(c), width(w) {}
    

};