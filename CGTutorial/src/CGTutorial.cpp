#pragma once
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

int main()
{
	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	// The rest of our code goes here

	glfwTerminate();
    return 0;
}