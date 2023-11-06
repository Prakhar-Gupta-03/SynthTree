#include "turtle.h"

std::vector<float> Turtle::parser(std::vector<std::string> modules) {
    std::vector<float> vertices;
    for(int i = 0; i<modules.size(); i++){
        std::string module = modules[i];
        if(module[0]=='F'){
            forward(std::stod(module.substr(2, module.size()-3)), vertices);
        }else if(module[0]=='['){
            saveState();
        }else if(module[0]==']'){
            restoreState();
        }else if(module[0]=='+'){
            Ru(std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='-'){
            Ru(-std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='!'){
            changeWidth(std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='/'){
            Rh(std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='\\'){
            Rh(-std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='&'){
            Rl(std::stod(module.substr(2, module.size()-3)));
        }else if(module[0]=='^'){
            Rl(-std::stod(module.substr(2, module.size()-3)));
        }
    }
    return vertices;
}

void Turtle::forward(double distance, std::vector<float>& vertices){
    glm::vec3 position = current_state.getPosition();
    glm::mat3 HLU = current_state.getHLU();
    glm::vec3 direction = glm::transpose(HLU)[0];
    glm::vec3 new_position = position + float(distance)*direction;
    current_state.setPosition(new_position);
    vertices.push_back(position.x);
    vertices.push_back(position.y);
    vertices.push_back(position.z);
    vertices.push_back(new_position.x);
    vertices.push_back(new_position.y);
    vertices.push_back(new_position.z);

}

void Turtle::saveState(){
    states.push(current_state);
    current_state = current_state.copy();
}

void Turtle::restoreState(){
    current_state = states.top();
    states.pop();
}

void Turtle::Ru(double angle){
    angle = glm::radians(angle);
    glm::mat3 HLU = current_state.getHLU();
    glm::mat3 rotation = glm::mat3(glm::vec3(cos(angle), -sin(angle), 0), glm::vec3(sin(angle), cos(angle), 0), glm::vec3(0,0,1));
    glm::mat3 new_HLU = glm::mat3(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            new_HLU[i][j] = HLU[i][0]*rotation[0][j] + HLU[i][1]*rotation[1][j] + HLU[i][2]*rotation[2][j];
        }
    }
    current_state.setHLU(new_HLU);
}

void Turtle::Rh(double angle){
    glm::mat3 HLU = current_state.getHLU();
    angle = glm::radians(angle);
    glm::mat3 rotation = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, cos(angle), -sin(angle)), glm::vec3(0, sin(angle), cos(angle)));
    glm::mat3 new_HLU = glm::mat3(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            new_HLU[i][j] = HLU[i][0]*rotation[0][j] + HLU[i][1]*rotation[1][j] + HLU[i][2]*rotation[2][j];
        }
    }
    current_state.setHLU(new_HLU);
}

void Turtle::Rl(double angle){
    glm::mat3 HLU = current_state.getHLU();
    angle = glm::radians(angle);
    glm::mat3 rotation = glm::mat3(glm::vec3(cos(angle), 0, sin(angle)), glm::vec3(0, 1, 0), glm::vec3(-sin(angle), 0, cos(angle)));
    glm::mat3 new_HLU = glm::mat3(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            new_HLU[i][j] = HLU[i][0]*rotation[0][j] + HLU[i][1]*rotation[1][j] + HLU[i][2]*rotation[2][j];
        }
    }
    current_state.setHLU(new_HLU);
}

void Turtle::changeWidth(double width){
    current_state.setWidth(width);
}