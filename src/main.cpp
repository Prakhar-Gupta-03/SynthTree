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

GLuint tree_texture, leaf_texture;
GLuint tex1, tex2, tex3;
GLuint leaf_tex1, leaf_tex2, leaf_tex3;
bool leaves = true;

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


int main(int, char **)
{
	// Setup window
	GLFWwindow *window = setupWindow(SCREEN_W, SCREEN_H, "L-System Based Tree/Plant Modelling"); // Initialize Window
	ImGuiIO &io = ImGui::GetIO(); // Create IO

	// // Time variables
	double last_time = 0, delta_time = 0; 

	// create shader program
	unsigned int shaderprogram = createProgram("shaders/vshader.vs", "shaders/fshader.fs");
	
	assert(loadImagetoTexture("textures/texture1.jpg", tex1));
	assert(loadImagetoTexture("textures/texture2.jpg", tex2));
	assert(loadImagetoTexture("textures/texture3.jpg", tex3));
	assert(loadImagetoTexture("textures/leaf_texture1.jpeg", leaf_tex1));
	assert(loadImagetoTexture("textures/leaf_texture2.jpg", leaf_tex2));
	assert(loadImagetoTexture("textures/leaf_texture3.jpg", leaf_tex3));
	leaf_texture = leaf_tex1;
	tree_texture = tex1;

	// Setup camera
	glm::vec3 cam_position = glm::vec3(0.0f, 0.0f, 20.0f), cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f), cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera *cam = new Camera(cam_position, cam_look_at, cam_up, 45.0f, 0.1f, 10000.0f, window);
	cam->setProjectionTransformation(shaderprogram);
	cam->setViewTransformation(shaderprogram);
	cam->setModelTransformation(shaderprogram);

	// Defining L-System
	std::vector<std::string> alphabets = {"A", "F"};
	std::vector<std::string> symbols = {"+", "[", "]", "!", "/"};
	std::string axiom = "A(100,30)";
	std::unordered_map<std::string, double> constants = {
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
	std::vector<std::string> production_rules = {
		"A(s,w):s>=min?!(w)F(s)[+(alpha1)/(rho1)A(s*r1,w*(q^e))][+(alpha2)/(rho2)A(s*r2,w*((1-q)^e))]",
		"F(s):True?F(s)"
	};
	
	// Creating L-System with the above defined parameters
	L_System l_system(axiom, alphabets, constants, production_rules, symbols);

	int recursive_steps = 10;
	// generating the string that the turtle will parse to generate the vertices of the tree
	std::string word = l_system.generateWord(recursive_steps); 
	// splitting the string into modules. Each module is a single instruction for the turtle
	std::vector<std::string> modules = l_system.generateModule(word); 
	
	glm::vec3 turtle_pos = glm::vec3(0,-500,0); // initial position of the turtle
	// initial HLU space of the turtle
	glm::mat4 HLU = glm::transpose(glm::mat3(glm::vec3(0,1,0), glm::vec3(-1,0,0), glm::vec3(0,0,1))); 
	
	// Setting up initial color and width of the tree that the turtle will draw
	Color c = Color(0,0,0); 
	double w = 1; 
	
	Turtle turtle = Turtle(State(turtle_pos, HLU, c, w));
	// Parse the string and generate the vertices of the tree
	std::vector<std::vector<float>> v = turtle.parser(modules);
	std::vector<float> vertices = v[0], leaf_vertices = v[1];
	// scaling the vertices to fit the screen
	for(int i = 0; i<vertices.size(); i++){
		vertices[i] = vertices[i]/50.0f;
	}
	for(int i = 0; i<leaf_vertices.size(); i++){
		leaf_vertices[i] = leaf_vertices[i]/50.0f;
	}

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

		ImGui::Begin("Leaves or not ?");
		if(ImGui::RadioButton("Leaves", leaves)){
			leaves = true;
			
		}
		if(ImGui::RadioButton("No Leaves", !leaves)){
			leaves = false;
		}
		ImGui::End();

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


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(WHITE.x, WHITE.y, WHITE.z, WHITE.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, tree_texture);

		for(int i = 0; i<vertices.size(); i+=15){
			std::vector<float> v = {vertices[i], vertices[i+1], vertices[i+2], vertices[i+3], vertices[i+4], vertices[i+5], vertices[i+6], vertices[i+7], vertices[i+8], vertices[i+9], vertices[i+10], vertices[i+11], vertices[i+12], vertices[i+13], vertices[i+14]};
			turtle.drawTriangle(v, shaderprogram, VAO, VBO);
		}

		glBindTexture(GL_TEXTURE_2D, leaf_texture);

		if(leaves){
			for(int i = 0; i<leaf_vertices.size(); i+=10){
				std::vector<float> v = {leaf_vertices[i], leaf_vertices[i+1], leaf_vertices[i+2], leaf_vertices[i+3], leaf_vertices[i+4], leaf_vertices[i+5], leaf_vertices[i+6], leaf_vertices[i+7], leaf_vertices[i+8], leaf_vertices[i+9]};
				turtle.drawLine(v, shaderprogram, VAO, VBO);
			}
		}
		

		// turtle.drawTriangle(vertices, shaderprogram, VAO, VBO);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	cleanup(window);

	return 0;
}
