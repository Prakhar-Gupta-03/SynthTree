#include "utils/utils.hpp"
#include "camera/camera.hpp"
#include "L_system/l_system.h"
#include "turtle/turtle.h"
#include "color/color.h"
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "globals.hpp"
#include "bits/stdc++.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../depends/stb/stb_image.h"
#include "imgui.h"

GLuint tree_texture, leaf_texture, flower_texture;
GLuint tex1, tex2, tex3;
GLuint leaf_tex1, leaf_tex2, leaf_tex3;
GLuint flower_tex1, flower_tex2, flower_tex3;
bool leaves = true, flower = true;
int tree_num = 1;

bool loadImagetoTexture(const char *filename, GLuint &tex)
{
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int image_width, image_height;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if(image_data==NULL) return false;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_data);
	return true;
}

std::vector<std::vector<float>> getVertices(std::vector<std::string> production_rules, std::unordered_map<std::string, double> &constants, std::string axiom, int recursive_steps, float scale, float leaf_threshold){
	std::vector<std::string> alphabets = {"A", "F"};
	std::vector<std::string> symbols = {"+", "[", "]", "!", "/"};

	L_System l_system(axiom, alphabets, constants, production_rules, symbols);

	std::string word = l_system.generateWord(recursive_steps); 

	std::vector<std::string> modules = l_system.generateModule(word); 

	glm::vec3 turtle_pos = glm::vec3(0,-500,0);

	glm::mat4 HLU = glm::transpose(glm::mat3(glm::vec3(0,1,0), glm::vec3(-1,0,0), glm::vec3(0,0,1))); 
	
	double w = 1.0;

	Turtle turtle = Turtle(State(turtle_pos, HLU, w), leaf_threshold);

	// Parse the string and generate the vertices of the tree

	std::vector<std::vector<float>> v = turtle.parser(modules);

	for(int i = 0; i<v.size(); i++){
		for(int j = 0; j<v[i].size(); j++){
			v[i][j] *= scale;
		}
	}

	return v;
}

// This function is to draw a line between two vertices.
void drawLine(std::vector<float> vertices, unsigned int &shaderprogram, unsigned int &VAO, unsigned int &VBO)
{
	glUseProgram(shaderprogram);
	glBindVertexArray(VAO);

	// Bind the VBO and copy the vertex data to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	unsigned int vVertex_attrib = getAttrib(shaderprogram, "vVertex");
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // unsigned int texCord_attrib = getAttrib(shaderprogram, "texCord");
    // glEnableVertexAttribArray(texCord_attrib);
    // glVertexAttribPointer(texCord_attrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Draw the triangle
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 5);

	// Cleanup
	glDisableVertexAttribArray(vVertex_attrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

}

void drawTriangle(std::vector<float> vertices, unsigned int &shaderprogram, unsigned int &VAO, unsigned int &VBO)
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

void customTreeWindow(std::vector<std::vector<float>> &v_tree, bool &custom_tree, int &recursive_steps_tree1, float &alpha1, float &alpha2, float &r1, float &r2, float &rho1, float &rho2, float &q, float &e, float &min, float &w0, float &scale, float &leaf_threshold){
	// display a window to get the custom parameters
	ImGui::SetNextWindowPos(ImVec2(SCREEN_W/2 - 150, SCREEN_H/2 - 150));
	ImGui::SetNextWindowSize(ImVec2(400, 350));
	ImGui::Begin("Custom Tree Parameters");
	ImGui::Text("Custom Tree Parameters");
	static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
	static int vec4i[4] = { 1, 5, 100, 255 };
	ImGui::SliderInt("Recursive Steps", &recursive_steps_tree1, 1, 15);
	ImGui::SliderFloat("Alpha1", &alpha1, -90, 90);
	ImGui::SliderFloat("Alpha2", &alpha2, -90, 90);
	ImGui::SliderFloat("r1", &r1, 0.0, 1.0);
	ImGui::SliderFloat("r2", &r2, 0.0, 1.0);
	ImGui::SliderFloat("rho1", &rho1, 0.0, 180.0);
	ImGui::SliderFloat("rho2", &rho2, 0.0, 180.0);
	ImGui::SliderFloat("q", &q, 0.0, 1.0);
	ImGui::SliderFloat("e", &e, 0.0, 1.0);
	ImGui::SliderFloat("min", &min, 0.0, 50.0);
	ImGui::SliderFloat("w0", &w0, 0.0, 50.0);
	if(ImGui::Button("Generate Tree")){
		std::vector<std::string> tree_production_rules = {
			"A(s,w):s>=min?!(w)F(s)[+(alpha1)/(rho1)A(s*r1,w*(q^e))][+(alpha2)/(rho2)A(s*r2,w*((1-q)^e))]",
			"F(s):True?F(s)"
		};
		std::string axiom_tree1 = "A(100," + std::to_string(w0) + ")";
		std::unordered_map<std::string, double> constants_tree1 = {
			{"alpha1",alpha1},
			{"alpha2",alpha2},
			{"r1",r1},
			{"r2",r2},
			{"rho1",rho1},
			{"rho2",rho2},
			{"q",q},
			{"e",e},
			{"min",min}
		};
		// int recursive_steps_tree1 = 10;
		std::vector<std::vector<float>> v_tree1 = getVertices(tree_production_rules, constants_tree1, axiom_tree1, recursive_steps_tree1, scale, leaf_threshold);
		v_tree = v_tree1;
		custom_tree = false;
	}
	ImGui::End();
}

int main(int, char **)
{
	// Setup window
	GLFWwindow *window = setupWindow(SCREEN_W, SCREEN_H, "L-System Based Tree/Plant Modelling"); // Initialize Window
	ImGuiIO &io = ImGui::GetIO(); // Create IO

	// Time variables
	double last_time = 0, delta_time = 0; 

	// create shader program
	unsigned int shaderprogram = createProgram("shaders/vshader.vs", "shaders/fshader.fs");
	
	// Loading textures with error check
	assert(loadImagetoTexture("textures/texture1.jpg", tex1));
	assert(loadImagetoTexture("textures/texture2.jpg", tex2));
	assert(loadImagetoTexture("textures/texture3.jpg", tex3));
	assert(loadImagetoTexture("textures/leaf_texture1.jpeg", leaf_tex1));
	assert(loadImagetoTexture("textures/leaf_texture2.jpg", leaf_tex2));
	assert(loadImagetoTexture("textures/leaf_texture3.jpg", leaf_tex3));
	assert(loadImagetoTexture("textures/flower_texture1.jpg", flower_tex1));
	assert(loadImagetoTexture("textures/flower_texture2.jpg", flower_tex2));
	assert(loadImagetoTexture("textures/flower_texture3.jpg", flower_tex3));

	// Texture initialization
	leaf_texture = leaf_tex1;
	tree_texture = tex1;
	flower_texture = flower_tex1;

	// Setup camera
	glm::vec3 cam_position = glm::vec3(0.0f, 0.0f, 20.0f), cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f), cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera *cam = new Camera(cam_position, cam_look_at, cam_up, 45.0f, 0.1f, 10000.0f, window);
	cam->setProjectionTransformation(shaderprogram);
	cam->setViewTransformation(shaderprogram);
	cam->setModelTransformation(shaderprogram);

	// Defining L-System
	std::vector<std::string> tree_production_rules = {
		"A(s,w):s>=min?!(w)F(s)[+(alpha1)/(rho1)A(s*r1,w*(q^e))][+(alpha2)/(rho2)A(s*r2,w*((1-q)^e))]",
		"F(s):True?F(s)"
	};
	std::string axiom_tree1 = "A(100,30)";
	std::unordered_map<std::string, double> constants_tree1 = {
		{"alpha1",30},
		{"alpha2",-30},
		{"r1",0.80},
		{"r2",0.80},
		{"rho1",137},
		{"rho2",137},
		{"q",0.50},
		{"e",0.50},
		{"min",0.0}
	};
	int recursive_steps_tree1 = 10;

	std::string axiom_tree2 = "A(100,40)";
	std::unordered_map<std::string, double> constants_tree2 = {
		{"alpha1",5},
		{"alpha2",-30},
		{"r1",0.95},
		{"r2",0.75},
		{"rho1",-90},
		{"rho2",90},
		{"q",0.60},
		{"e",0.45},
		{"min",25.0}
	};
	int recursive_steps_tree2 = 12;

	std::string axiom_tree3 = "A(100,20)";
	std::unordered_map<std::string, double> constants_tree3 = {
		{"alpha1",-5},
		{"alpha2",30},
		{"r1",0.55},
		{"r2",0.95},
		{"rho1",137},
		{"rho2",137},
		{"q",0.40},
		{"e",0.25},
		{"min",5.0}
	};
	int recursive_steps_tree3 = 12;

	std::vector<std::vector<float>> v_tree1 = getVertices(tree_production_rules, constants_tree1, axiom_tree1, recursive_steps_tree1, 0.02, 2);
	std::vector<std::vector<float>> v_tree2 = getVertices(tree_production_rules, constants_tree2, axiom_tree2, recursive_steps_tree2, 0.02, 20);
	std::vector<std::vector<float>> v_tree3 = getVertices(tree_production_rules, constants_tree3, axiom_tree3, recursive_steps_tree3, 0.02, 5); 
	std::vector<std::vector<float>> v_tree = v_tree1;

	// Setting up VAO and VBO
	unsigned int VAO, VBO;

    //Generating and binding vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	//Generating and binding vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	bool custom_tree = false;
	int recursive_steps_tree = 12;
	float alpha1 = -5, alpha2 = 30, r1 = 0.55, r2 = 0.95, rho1 = 137, rho2 = 137, q = 0.40, e = 0.25, min = 5.0, scale = 0.02, w0 = 20;
	float leaf_threshold = 5;
	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		double curr_time = static_cast<double>(glfwGetTime());
		

		delta_time = curr_time - last_time;
		last_time = curr_time;

		if (!io.WantCaptureMouse)
		{
			cam->process_input(window, delta_time);
			cam->setViewTransformation(shaderprogram);
		}

		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(150, 275));
		ImGui::Begin("Select Texture");
		ImGui::Text("Select Texture");
		ImGui::Text("Texture 1");
		if(ImGui::ImageButton((void*)(intptr_t)tex1, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			tree_texture = tex1;
		}
		ImGui::Text("Texture 2");
		if(ImGui::ImageButton((void*)(intptr_t)tex2, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			tree_texture = tex2;
		}
		ImGui::Text("Texture 3");
		if(ImGui::ImageButton((void*)(intptr_t)tex3, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			tree_texture = tex3;
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(10, 300));
		ImGui::SetNextWindowSize(ImVec2(150, 100));
		ImGui::Begin("Leaves or not ?");
		if(ImGui::RadioButton("Leaves", leaves)){
			leaves = true;
			
		}
		if(ImGui::RadioButton("No Leaves", !leaves)){
			leaves = false;
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(10, 450));
		ImGui::SetNextWindowSize(ImVec2(150, 100));
		ImGui::Begin("Flowers or not ?");
		if(ImGui::RadioButton("Flowers", flower)){
			flower = true;
		}
		if(ImGui::RadioButton("No Flowers", !flower)){
			flower = false;
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(SCREEN_W - 160, 10));
		ImGui::SetNextWindowSize(ImVec2(150, 275));
		ImGui::Begin("Select Leaf Texture");
		ImGui::Text("Select Texture");
		ImGui::Text("Texture 1");
		if(ImGui::ImageButton((void*)(intptr_t)leaf_tex1, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			leaf_texture = leaf_tex1;
		}
		ImGui::Text("Texture 2");
		if(ImGui::ImageButton((void*)(intptr_t)leaf_tex2, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			leaf_texture = leaf_tex2;
		}
		ImGui::Text("Texture 3");
		if(ImGui::ImageButton((void*)(intptr_t)leaf_tex3, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			leaf_texture = leaf_tex3;
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(SCREEN_W - 320, 10));
		ImGui::SetNextWindowSize(ImVec2(150, 275));
		ImGui::Begin("Select Flower Texture");
		ImGui::Text("Select Texture");
		ImGui::Text("Texture 1");
		if(ImGui::ImageButton((void*)(intptr_t)flower_tex1, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			flower_texture = flower_tex1;
		}
		ImGui::Text("Texture 2");
		if(ImGui::ImageButton((void*)(intptr_t)flower_tex2, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			flower_texture = flower_tex2;
		}
		ImGui::Text("Texture 3");
		if(ImGui::ImageButton((void*)(intptr_t)flower_tex3, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))){
			flower_texture = flower_tex3;
		}
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(SCREEN_W - 160, 300));
		ImGui::SetNextWindowSize(ImVec2(150, 145));
		ImGui::Begin("Select Tree");
		ImGui::Text("Select Tree");
		if(ImGui::RadioButton("Tree 1", tree_num==1)){
			tree_num = 1;
			v_tree = v_tree1;
		}
		if(ImGui::RadioButton("Tree 2", tree_num==2)){
			tree_num = 2;
			v_tree = v_tree2;
		}
		if(ImGui::RadioButton("Tree 3", tree_num==3)){
			tree_num = 3;
			v_tree = v_tree3;
		}
		if (ImGui::RadioButton("Custom", tree_num==4)){
			tree_num = 4;
			custom_tree = true;
		}
		ImGui::End();
		if (custom_tree){
			customTreeWindow(v_tree, custom_tree, recursive_steps_tree, alpha1, alpha2, r1, r2, rho1, rho2, q, e, min, w0, scale, leaf_threshold);
		}
		// Custom Tree Parameters Sliding Bar ImG

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(WHITE.x, WHITE.y, WHITE.z, WHITE.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		// glUseProgram(shaderprogram);
		glBindTexture(GL_TEXTURE_2D, tree_texture);

		for(int i = 0; i<v_tree[0].size(); i+=15){
			std::vector<float> v = {v_tree[0][i], v_tree[0][i+1], v_tree[0][i+2], v_tree[0][i+3], v_tree[0][i+4], v_tree[0][i+5], v_tree[0][i+6], v_tree[0][i+7], v_tree[0][i+8], v_tree[0][i+9], v_tree[0][i+10], v_tree[0][i+11], v_tree[0][i+12], v_tree[0][i+13], v_tree[0][i+14]};
			drawTriangle(v, shaderprogram, VAO, VBO);
		}

		glBindTexture(GL_TEXTURE_2D, leaf_texture);

		if(leaves){
			for(int i = 0; i<v_tree[1].size(); i+=10){
				std::vector<float> v = {v_tree[1][i], v_tree[1][i+1], v_tree[1][i+2], v_tree[1][i+3], v_tree[1][i+4], v_tree[1][i+5], v_tree[1][i+6], v_tree[1][i+7], v_tree[1][i+8], v_tree[1][i+9]};
				drawLine(v, shaderprogram, VAO, VBO);
			}
		}

		glBindTexture(GL_TEXTURE_2D, flower_texture);
		if(flower){
			for(int i = 0; i<v_tree[2].size(); i+=10){
				std::vector<float> v = {v_tree[2][i], v_tree[2][i+1], v_tree[2][i+2], v_tree[2][i+3], v_tree[2][i+4], v_tree[2][i+5], v_tree[2][i+6], v_tree[2][i+7], v_tree[2][i+8], v_tree[2][i+9]};
				drawLine(v, shaderprogram, VAO, VBO);
			}
		}
		

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	cleanup(window);

	return 0;
}
