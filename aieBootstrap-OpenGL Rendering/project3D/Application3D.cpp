#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false) {
		printf("Render target error!\n");
		return false;
	}

	// Initialise Lighting
	m_ambientLight = { 0.05f, 0.05f, 0.05f };

	m_Light.dds[0] = { -10, 1, 1 };
	m_Light.dds[1] = { 1, 1, 1 };
	m_Light.dds[2] = { 1, 1, 1 };

	m_Light2.dds[0] = { 10, 1, 1 };
	m_Light2.dds[1] = { 1, 1, 1 };
	m_Light2.dds[2] = { 1, 1, 1 };

	m_Lights = new glm::mat3[2];
	m_Lights[0] = m_Light.light;
	m_Lights[1] = m_Light2.light;

	// Initialise Lighting
	/*m_light.direction = { 1, 1, 1 };
	m_light.diffuse = { 1, 1, 1 };
	m_light.specular = { 1, 1, 0 };*/
	

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalmap.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalmap.frag");
	if (m_shader.link() == false)
	{
		printf("Shader error: %s\n", m_shader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	if (m_spear.load("./models/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	if (m_chest.load("./models/chest/Chest.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	if (m_barrel.load("./models/barrel/big_wood_barrel.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	if (m_horn.load("./models/horn/horn.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	if (m_skull.load("./models/skull/skull.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_quadMesh.initialiseQuad();
	m_fullscreenQuad.initialiseFullscreenQuad();

	// Make the quad 10 units wide
	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0.01f,0,1
	};

	m_bunnyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1
	};

	// Object Instancing
	m_spearInstance = new ObjectInstance("Soulspear", &m_spear, &m_shader);

	m_chestInstance = new ObjectInstance("Chest", &m_chest, &m_shader);
	m_chestInstance->setScale({ 0.02, 0.02, 0.02 });
	m_chestInstance->setPos({ -4.3, 0, 2.7 });

	m_barrelInstance = new ObjectInstance("Barrel", &m_barrel, &m_shader);
	m_barrelInstance->setScale({ 0.02, 0.02, 0.02 });
	m_barrelInstance->setPos({ 4.3, 0, 2.7 });

	m_hornInstance = new ObjectInstance("Horn", &m_horn, &m_shader);
	m_hornInstance->setScale({ 0.02, 0.02, 0.02 });
	m_hornInstance->setPos({ 4.3, 1.0, -2.7 });

	m_skullInstance = new ObjectInstance("Skull", &m_skull, &m_shader);
	m_skullInstance->setScale({ 0.216, 0.216, 0.216 });
	m_skullInstance->setPos({ -4.3, 0.7, -2.7 });


	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10),
							   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_flyCamera.Update();

	if (m_currentFilter != m_filter) {
		m_currentFilter = m_filter;

		switch (m_currentFilter) {
			case 0:
			{
				m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
				m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");
				if (m_postShader.link() == false) 
					printf("Post Shader Error: %s\n", m_postShader.getLastError());
				break;
			}
			case 1:
			{
				m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
				m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/postBlur.frag");
				if (m_postShader.link() == false)
					printf("Post Shader Error: %s\n", m_postShader.getLastError());
				break;
			}
			case 2:
			{
				m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
				m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/postDistort.frag");
				if (m_postShader.link() == false)
					printf("Post Shader Error: %s\n", m_postShader.getLastError());
				break;
			}
			default:
				break;
		}
	}
}

void Application3D::draw() {
	// IMGUI
	ImGui::Begin("Lights");
	ImGui::ColorEdit3("Ambient Light Colour", &m_ambientLight.x);
	if (ImGui::CollapsingHeader((std::string("Directional Light ") + std::to_string((int)&m_Light)).c_str())) {
		ImGui::SliderFloat3((std::string("Direction ") + std::to_string((int)&m_Lights[0][0])).c_str(), &m_Lights[0][0].x, -20, 20);
		ImGui::SliderFloat3((std::string("Diffusion ") + std::to_string((int)&m_Lights[0][1])).c_str(), &m_Lights[0][1].x, 0, 1);
		ImGui::SliderFloat3((std::string("Specular ") + std::to_string((int)&m_Lights[0][2])).c_str(), &m_Lights[0][2].x, 0, 1);
	}
	if (ImGui::CollapsingHeader((std::string("Directional Light ") + std::to_string((int)&m_Light2)).c_str())) {
		ImGui::SliderFloat3((std::string("Direction ") + std::to_string((int)&m_Lights[1][0])).c_str(), &m_Lights[1][0].x, -20, 20);
		ImGui::SliderFloat3((std::string("Diffusion ") + std::to_string((int)&m_Lights[1][1])).c_str(), &m_Lights[1][1].x, 0, 1);
		ImGui::SliderFloat3((std::string("Specular ") + std::to_string((int)&m_Lights[1][2])).c_str(), &m_Lights[1][2].x, 0, 1);
	}
	ImGui::End();

	ImGui::Begin("Filters");
	ImGui::SliderInt("Filter Number", &m_filter, 0, 2);
	ImGui::End();
	//

	// bind our render target
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = m_flyCamera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_flyCamera.GetViewMatrix();
	
	// bind shader
	//m_shader.bind();

	// Spear Instance
	auto pvmBunny = m_projectionMatrix * m_viewMatrix * m_spearInstance->getTransform();	
	m_spearInstance->addBinding("Ia", m_ambientLight);
	m_spearInstance->addBinding("Lights", 2, m_Lights);
	m_spearInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_spearInstance->addBinding("ModelMatrix", m_spearInstance->getTransform());
	m_spearInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_spearInstance->addBinding("roughness", 1.0f);
	m_spearInstance->addBinding("reflectionCoefficient", 1.0f);
	m_spearInstance->draw();

	// Chest Instance
	pvmBunny = m_projectionMatrix * m_viewMatrix * m_chestInstance->getTransform();
	m_chestInstance->addBinding("Ia", m_ambientLight);
	m_chestInstance->addBinding("Lights", 2, m_Lights);
	m_chestInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_chestInstance->addBinding("ModelMatrix", m_chestInstance->getTransform());
	m_chestInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_chestInstance->addBinding("roughness", 1.0f);
	m_chestInstance->addBinding("reflectionCoefficient", 1.0f);
	m_chestInstance->draw();

	// Barrel Instance
	pvmBunny = m_projectionMatrix * m_viewMatrix * m_barrelInstance->getTransform();
	m_barrelInstance->addBinding("Ia", m_ambientLight);
	m_barrelInstance->addBinding("Lights", 2, m_Lights);
	m_barrelInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_barrelInstance->addBinding("ModelMatrix", m_barrelInstance->getTransform());
	m_barrelInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_barrelInstance->addBinding("roughness", 1.0f);
	m_barrelInstance->addBinding("reflectionCoefficient", 1.0f);
	m_barrelInstance->draw();

	// Horn Instance
	pvmBunny = m_projectionMatrix * m_viewMatrix * m_hornInstance->getTransform();
	m_hornInstance->addBinding("Ia", m_ambientLight);
	m_hornInstance->addBinding("Lights", 2, m_Lights);
	m_hornInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_hornInstance->addBinding("ModelMatrix", m_hornInstance->getTransform());
	m_hornInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_hornInstance->addBinding("roughness", 1.0f);
	m_hornInstance->addBinding("reflectionCoefficient", 1.0f);
	m_hornInstance->draw();

	// Lucy Instance
	pvmBunny = m_projectionMatrix * m_viewMatrix * m_skullInstance->getTransform();
	m_skullInstance->addBinding("Ia", m_ambientLight);
	m_skullInstance->addBinding("Lights", 2, m_Lights);
	m_skullInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_skullInstance->addBinding("ModelMatrix", m_skullInstance->getTransform());
	m_skullInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_skullInstance->addBinding("roughness", 1.0f);
	m_skullInstance->addBinding("reflectionCoefficient", 1.0f);
	m_skullInstance->draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	//unbind target to return to back buffer
	m_renderTarget.unbind();
	clearScreen();

	//bind post shader and textures
	m_postShader.bind();
	m_postShader.bindUniform("colourTarget", 0);
	m_renderTarget.getTarget(0).bind(0);

	//draw fullscreen quad
	m_fullscreenQuad.draw();
}