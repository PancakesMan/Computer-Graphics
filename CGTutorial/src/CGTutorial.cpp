#pragma once
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <stdio.h>

#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

using namespace aie;

class Application
{
public:
	Application(int width, int height, const char* title)
	{
		m_width = width;
		m_height = height;
		m_title = title;
	}

	bool Startup()
	{
		if (glfwInit() == false)
			return false;

		m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

		if (m_window == nullptr) {
			Shutdown();
			return false;
		}

		glfwMakeContextCurrent(m_window);

		if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
			Shutdown();
			return false;
		}

		auto major = ogl_GetMajorVersion();
		auto minor = ogl_GetMinorVersion();
		printf("GL: %i.%i\n", major, minor);

		glClearColor(0.25, 0.25, 0.25, 1);
		glEnable(GL_DEPTH_TEST); // enabled depth buffer

		m_setup = m_window;
		return true;
	}

	void Run()
	{
		if (m_window || Startup())
		{
			Gizmos::create(65535, 65535, 65535, 65535);
			double prevTime = glfwGetTime(), currTime = 0, deltaTime = 0;

			while (glfwWindowShouldClose(m_window) == false &&
				glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				currTime = glfwGetTime();
				deltaTime = currTime - prevTime;
				prevTime = currTime;

				Update(float(deltaTime));
				Draw();

				glfwPollEvents();
				glfwSwapBuffers(m_window);
			}

			Shutdown();
		}
	}

	void Shutdown()
	{
		if (m_window)
			glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

private:
	int m_width, m_height;
	const char* m_title;
	bool m_setup;

	GLFWwindow* m_window;
};

class MyApplication : public Application
{
public:
	MyApplication(int w, int h, const char* title) : Application(w, h, title)
	{
		m_timer = 0;
	}

	void Update(float deltaTime)
	{
		// Timer
		m_timer += deltaTime;

		mat4* def = new mat4(1);
		rotate = glm::rotate(*def, m_timer, vec3(0, 1, 0));
	}

	void Draw()
	{
		Gizmos::clear();
		Gizmos::addTransform(glm::mat4(1));

		vec4 white(1), black(0, 0, 0, 1);

		vec4 sunPos = vec4(0);
		vec4 earthPos = sunPos + vec4(6, 0, 0, 0);
		vec4 moonPos = vec4(0.7, 0, 0, 0);

		Gizmos::addSphere(sunPos, 1, 10, 10, vec4(1, 0.3, 0, 1), &rotate);
		Gizmos::addSphere(rotate * earthPos, 0.3, 10, 10, vec4(0, 1, 1, 1), &rotate);
		Gizmos::addSphere(rotate * (rotate * moonPos) + (rotate * earthPos), 0.1, 10, 10, vec4(1, 1, 1, 1), &rotate);

		// Render grid
		for (int i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : black);
			Gizmos::addLine(vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}

		Gizmos::draw(projection * view);
	}

private:
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f,
		16 / 9.f, 0.1f, 1000.f);

	mat4 rotate;

	float m_timer;
};

int main()
{
	MyApplication* myApp = new MyApplication(1280, 720, "Test");
	myApp->Run();
}