#include "utils/vector3D.h"
#include "color/color.h"
#include "globals.hpp"
class State{
private:
    glm::vec3 position;
    glm::mat3 HLU;
    double width;
public:
    State(glm::vec3 pos, glm::mat3 HLU_, double w):
        position(pos), HLU(HLU_), width(w) {}
    double getWidth() const { return width; }
    void setWidth(double w) { width = w; }
    glm::vec3 getPosition() const { return position; }
    glm::mat3 getHLU() const { return HLU; }
    void setPosition(glm::vec3 pos) { position = pos; }
    void setHLU(glm::mat3 HLU_) { HLU = HLU_; }
    State copy() const { return State(position, HLU, width); }

};