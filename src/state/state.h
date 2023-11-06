#include "utils/vector3D.h"
#include "color/color.h"
#include "globals.hpp"
class State{
private:
    glm::vec3 position;
    glm::mat3 HLU;
    Color color;
    double width;
public:
    State(glm::vec3 pos, glm::mat3 HLU_, Color c, double w):
        position(pos), HLU(HLU_), color(c), width(w) {}
    Color getColor() const { return color; }
    double getWidth() const { return width; }
    void setColor(Color c) { color = c; }
    void setWidth(double w) { width = w; }
    glm::vec3 getPosition() const { return position; }
    glm::mat3 getHLU() const { return HLU; }
    void setPosition(glm::vec3 pos) { position = pos; }
    void setHLU(glm::mat3 HLU_) { HLU = HLU_; }
    State copy() const { return State(position, HLU, color, width); }

};