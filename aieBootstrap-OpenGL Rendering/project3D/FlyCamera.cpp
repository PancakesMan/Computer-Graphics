#pragma once
#include "FlyCamera.h"
#include "Input.h"

glm::mat4 FlyCamera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.0f);
}

glm::mat4 FlyCamera::GetViewMatrix()
{
	float thetaR = theta * 180 / 3.14159265358979f;
	float phiR = phi * 180 / 3.14159265358979f;

	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));

	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

void FlyCamera::Update()
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = theta * 180 / 3.14159265358979f;
	float phiR = phi * 180 / 3.14159265358979f;

	// calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float deltaTime = 0.1f;

	float mx = input->getMouseX();
	float my = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		theta += 0.00005f * (mx - lastMouseX);
		phi -= 0.00005f * (my - lastMouseY);
		phi = phi > 70 ? 70 : phi < -70 ? -70 : phi;
	}
	lastMouseX = mx;
	lastMouseY = my;

	if (input->isKeyDown(aie::INPUT_KEY_X))
		position += up * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_Z))
		position += -up * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		position += right * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		position += -right * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		position += forward * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		position += -forward * deltaTime;
}