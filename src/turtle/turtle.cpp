#include "turtle.h"

// Using the modules, the turtle will parse the string and return a vector of vertices that will be drawn. The meaning of the symbols are the same as used in the paper provided.
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

// This function is to move the turtle forward by a certain distance and add the vertices to the vector of vertices.
void Turtle::forward(double distance, std::vector<float>& vertices){ 
    glm::vec3 position = current_state.getPosition();
    glm::mat3 HLU = current_state.getHLU();
    glm::vec3 direction_f = glm::transpose(HLU)[0], direction_l = glm::transpose(HLU)[1];

    glm::vec3 new_position = position + float(distance)*direction_f;
    current_state.setPosition(new_position);
    
    double top_radius = current_state.getWidth();
    double bottom_radius = current_state.getWidth();
    if(states.size()>0){
        bottom_radius = states.top().getWidth();
    }

    Cylinder cylinder = Cylinder(position, new_position, bottom_radius, top_radius);
    std::vector<float> cylinder_vertices = cylinder.generateTriangles();
    for(int i = 0; i<cylinder_vertices.size(); i++){
        vertices.push_back(cylinder_vertices[i]);
    }
}
// This function is to save the current state of the turtle. This is used when the turtle encounters a '[' symbol. 
void Turtle::saveState(){
    states.push(current_state);
    current_state = current_state.copy();
}
// This function is to restore the state of the turtle to the last saved state. This is used when the turtle encounters a ']' symbol.
void Turtle::restoreState(){
    current_state = states.top();
    states.pop();
}
// This function is to rotate the turtle about the up vector of the turtle by a certain angle.
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
// This function is to rotate the turtle about the left vector of the turtle by a certain angle.
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
// This function is to rotate the turtle about the forward vector of the turtle by a certain angle.
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
// This function is to change the width of the turtle.
void Turtle::changeWidth(double width){
    current_state.setWidth(width);
}
// This function is to draw a line between two vertices.
void Turtle::drawLine(std::vector<float> vertices, unsigned int &shaderprogram, unsigned int &VAO, unsigned int &VBO)
{
	glUseProgram(shaderprogram);
	glBindVertexArray(VAO);

	// Bind the VBO and copy the vertex data to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	unsigned int vVertex_attrib = getAttrib(shaderprogram, "vVertex");
	glEnableVertexAttribArray(vVertex_attrib);
	glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);

	// Cleanup
	glDisableVertexAttribArray(vVertex_attrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

}

void Turtle::drawTriangle(std::vector<float> vertices, unsigned int &shaderprogram, unsigned int &VAO, unsigned int &VBO)
{
    glUseProgram(shaderprogram);
    glBindVertexArray(VAO);

    // Bind the VBO and copy the vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    unsigned int vVertex_attrib = getAttrib(shaderprogram, "vVertex");
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    unsigned int texCord_attrib = getAttrib(shaderprogram, "texCord");
    glEnableVertexAttribArray(texCord_attrib);
    glVertexAttribPointer(texCord_attrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Draw the triangle
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 5);

    // Cleanup
    glDisableVertexAttribArray(vVertex_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

}