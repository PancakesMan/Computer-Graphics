#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class FlyCamera
{
public:
	FlyCamera() : theta(0), phi(-20), position(20, 10, 0) {}

	glm::mat4 GetProjectionMatrix(float w, float h);
	glm::mat4 GetViewMatrix();

	void Update();

private:
	float theta, phi;
	glm::vec3 position;

	int lastMouseX, lastMouseY;
};