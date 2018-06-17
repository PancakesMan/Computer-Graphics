#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using glm::mat4;
using glm::vec3;

class Camera
{
public:
	Camera();
	~Camera();

	// pure virtual update function
	virtual void update(float deltaTime) = 0;

	// setters
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);

	// getters
	mat4 getWorldTransform() { return worldTransform; }
	mat4 getView() { return viewTransform; }
	mat4 getProjection() { return projectionTransform; }
	mat4 getProjectionView() { return projectionViewTransform; }

private:
	mat4 worldTransform, viewTransform, projectionTransform, projectionViewTransform;

	void updateProjectionViewTransform();
};