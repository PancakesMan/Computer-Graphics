#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	void update(float deltaTime);
	void setSpeed(float speed) { m_speed = speed; }

private:
	float m_speed;
	vec3 m_up;
};