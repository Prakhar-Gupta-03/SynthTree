#include "utils/utils.hpp"
#include "camera/camera.hpp"
#include "L_system/l_system.h"
#include "turtle/turtle.h"
// #include "state/state.h"
#include "color/color.h"
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "globals.hpp"
#include "bits/stdc++.h"

void draw(std::vector<float> vertices, unsigned int shaderprogram, unsigned int VAO, unsigned int VBO)
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
	

int main(int, char **)
{
	// Setup window
	GLFWwindow *window = setupWindow(SCREEN_W, SCREEN_H, "Parametric Representations of Surfaces");
	ImGuiIO &io = ImGui::GetIO(); // Create IO

	double last_time = 0;
	double delta_time = 0;

	unsigned int shaderprogram = createProgram("shaders/vshader.vs", "shaders/fshader.fs");
	Camera *cam = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
							 45.0f, 0.1f, 10000.0f, window);
	cam->setProjectionTransformation(shaderprogram);
	cam->setViewTransformation(shaderprogram);

	unsigned int vModel_uniform = getUniform(shaderprogram, "vModel");
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));

	std::vector<std::string> alphabets = {"A", "F"};
	std::vector<std::string> symbols = {"+", "[", "]", "!", "/"};
	std::unordered_map<std::string, double> constants = {
		{"alpha1",25},
		{"alpha2",-15},
		{"r1",0.60},
		{"r2",0.85},
		{"rho1",180},
		{"rho2",180},
		{"q",0.45},
		{"e",0.50},
		{"min",0.0},
	};
	std::vector<std::string> production_rules = {
		"A(s,w):s>=min?!(w)F(s)[+(alpha1)/(rho1)A(s*r1,w*(q^e))][+(alpha2)/(rho2)A(s*r2,w*((1-q)^e))]",
		"F(s):True?F(s)"
	};
	std::string axiom = "A(100,20)";
	int steps = 10;
	L_System l_system(axiom, alphabets, constants, production_rules, symbols);
	std::string word = l_system.generateWord(steps);
	std::vector<std::string> modules = l_system.generateModule(word);
	glm::vec3 pos = glm::vec3(0,0,0);
	glm::mat4 HLU = glm::transpose(glm::mat3(glm::vec3(0,1,0), glm::vec3(-1,0,0), glm::vec3(0,0,1)));
	Color c = Color(1,1,1);
	double w = 1;
	
	Turtle turtle = Turtle(State(pos, HLU, c, w));
	std::vector<float> vertices = turtle.parser(modules);
	for(int i = 0; i<vertices.size(); i++){
		vertices[i] = vertices[i]/100.0f;
	}

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

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(WHITE.x, WHITE.y, WHITE.z, WHITE.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(int i = 0; i<vertices.size(); i+=6){
			draw({vertices[i], vertices[i+1], vertices[i+2], vertices[i+3], vertices[i+4], vertices[i+5]}, shaderprogram, VAO, VBO);
		}
		// draw(vertices, shaderprogram, VAO, VBO);


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	cleanup(window);

	return 0;
}
