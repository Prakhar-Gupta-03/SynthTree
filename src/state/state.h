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
    Vector3D getPosition() const { return position; }
    Vector3D getH() const { return H; }
    Vector3D getL() const { return L; }
    Vector3D getU() const { return U; }
    Color getColor() const { return color; }
    double getWidth() const { return width; }

    void setPosition(Vector3D pos) { position = pos; }
    void setH(Vector3D h) { H = h; }
    void setL(Vector3D l) { L = l; }
    void setU(Vector3D u) { U = u; }
    void setColor(Color c) { color = c; }
    void setWidth(double w) { width = w; }

};