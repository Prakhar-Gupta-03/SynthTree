#include "utils/utils.hpp"
#include "camera/camera.hpp"
#include "L_system/l_system.h"
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "bits/stdc++.h"
int main(int, char **)
{
	// Setup window
	GLFWwindow *window = setupWindow(SCREEN_W, SCREEN_H, "Parametric Representations of Surfaces");
	ImGuiIO &io = ImGui::GetIO(); // Create IO

	double last_time = 0;
	double delta_time = 0;

	unsigned int shader_program = createProgram("shaders/vshader.vs", "shaders/fshader.fs");
	Camera *cam = new Camera(glm::vec3(-5.0f, 3.0f, 3.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
							 45.0f, 0.1f, 10000.0f, window);

	cam->setProjectionTransformation(shader_program);
	cam->setViewTransformation(shader_program);

	std::vector<std::string> alphabets = {"A", "F"};
	std::vector<std::string> symbols = {"+", "[", "]", "!", "/"};
	std::unordered_map<std::string, double> constants = {
		{"alpha1",35},
		{"alpha2",-35},
		{"r1",0.75},
		{"r2",0.77},
		{"rho1",0},
		{"rho2",0},
		{"q",0.50},
		{"e",0.40},
		{"min",0.0},
	};
	std::vector<std::string> production_rules = {
		"A(s,w):s>=min?!(w)F(s)[+(alpha1)/(rho1)A(s*r1,w*(q^e))][+(alpha2)/(rho2)A(s*r2,w*((1-q)^e))]",
		"F(s):True?F(s)"
	};
	std::string axiom = "A(-0.1,30)";
	int steps = 9;
	L_System l_system(axiom, alphabets, constants, production_rules, symbols);
	std::cout<<l_system.generateWord(steps)<<std::endl;
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
			cam->setViewTransformation(shader_program);
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(BLACK.x, BLACK.y, BLACK.z, BLACK.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	cleanup(window);

	return 0;
}
