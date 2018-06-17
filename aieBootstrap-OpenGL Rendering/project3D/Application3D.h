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
	// object instance pointers
	ObjectInstance *m_spearInstance, *m_chestInstance, *m_barrelInstance, *m_hornInstance, *m_skullInstance;

	// camera matrices
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// shader for object instances
	aie::ShaderProgram  m_shader;

	// shader for post-processing effects
	aie::ShaderProgram  m_postShader;

	Mesh                m_quadMesh, m_fullscreenQuad;
	glm::mat4           m_quadTransform;
	aie::OBJMesh        m_spear, m_chest, m_barrel, m_horn, m_skull;
	glm::mat4           m_bunnyTransform;
	aie::Texture        m_gridTexture;

	// matrix array for lights
	glm::mat3*          m_Lights;

	// light struct
	// variables can be accessed as matrix or vector array
	struct Light {
		union {
			glm::mat3 light;
			glm::vec3 dds[3];
		};
	} m_Light, m_Light2;
	glm::vec3 m_ambientLight;

	FlyCamera m_flyCamera;

	// render target used for postprocessing effects
	aie::RenderTarget m_renderTarget;

	// variables to represent currently used post-processing filter
	int m_filter = 0, m_currentFilter = -1;
};