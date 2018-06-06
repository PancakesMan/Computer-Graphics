#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "FlyCamera.h"
#include "RenderTarget.h"
#include "ObjectInstance.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	ObjectInstance *m_spearInstance, *m_chestInstance, *m_barrelInstance, *m_hornInstance, *m_skullInstance;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram  m_shader;
	aie::ShaderProgram  m_planeShader;
	aie::ShaderProgram  m_postShader;

	Mesh                m_quadMesh, m_fullscreenQuad;
	glm::mat4           m_quadTransform;
	aie::OBJMesh        m_spear, m_chest, m_barrel, m_horn, m_skull;
	glm::mat4           m_bunnyTransform;
	aie::Texture        m_gridTexture;

	glm::mat3*          m_Lights;

	struct Light {
		glm::vec3 direction, diffuse, specular;
	} m_light;
	glm::vec3 m_ambientLight;

	struct TestLight {
		union {
			glm::mat3 light;
			glm::vec3 dds[3];
		};
	} m_Light, m_Light2, m_Light3;

	FlyCamera m_flyCamera;

	aie::RenderTarget m_renderTarget;

	int m_filter = 0, m_currentFilter = -1;
};