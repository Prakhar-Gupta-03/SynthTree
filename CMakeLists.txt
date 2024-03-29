 cmake_minimum_required(VERSION 3.5)

project(L-System-Based-3D-Tree-Modelling)
set(TARGET ${CMAKE_PROJECT_NAME})
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES
	"src/main.cpp"
	"src/L_system/l_system.cpp"
	"src/utils/utils.cpp"
	"src/utils/vector3D.cpp"
	"src/camera/camera.cpp"
	"src/color/color.cpp"
	"src/turtle/turtle.cpp"
	"src/shapes/cylinder.cpp"
	"src/shapes/leaf.cpp"
	"src/shapes/flower.cpp"
	"depends/imgui/imgui_impl_glfw.cpp"
	"depends/imgui/imgui_impl_opengl3.cpp"
	"depends/imgui/imgui.cpp"
	"depends/imgui/imgui_demo.cpp"
	"depends/imgui/imgui_draw.cpp"
	"depends/imgui/imgui_widgets.cpp"
	)

add_executable(${TARGET} ${SOURCES})

target_include_directories(${TARGET} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/utils
	${PROJECT_SOURCE_DIR}/depends/imgui
	${GLFW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS/../include}
	)
target_link_libraries(${TARGET} ${OPENGL_LIBRARIES} glfw GLEW::GLEW)

