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

	/*if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false) {
		printf("Render target error!\n");
		return false;
	}*/

	m_testLight.dds[0] = { 1, 1, 1 };
	m_testLight.dds[1] = { 1, 1, 1 };
	m_testLight.dds[2] = { 1, 1, 0 };

	// Initialise Lighting
	m_light.direction = { 1, 1, 1 };
	m_light.diffuse = { 1, 1, 1 };
	m_light.specular = { 1, 1, 0 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

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

	m_planeShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_planeShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_planeShader.link() == false)
	{
		printf("Shader error: %s\n", m_shader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	if (m_bunny.load("./models/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_quadMesh.initialiseQuad();

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

	// test instancing spear
	m_spearInstance = new ObjectInstance("Soulspear", &m_bunny, &m_shader);
	m_spearInstance2 = new ObjectInstance("Soulspear", &m_bunny, &m_shader);

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

	// demonstrate a few shapes
	/*Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));*/

	/*mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);*/

	// demonstrate 2D gizmos
	Gizmos::add2DAABB(glm::vec2(getWindowWidth() / 2, 100),
					  glm::vec2(getWindowWidth() / 2 * (fmod(getTime(), 3.f) / 3), 20),
					  vec4(0, 1, 1, 1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	//m_light.direction = glm::normalize(vec3(glm::cos(getTime() * 2), glm::sin(getTime() * 2), 0));
	//m_testLight.dds[0] = glm::normalize(vec3(glm::cos(getTime() * 2), glm::sin(getTime() * 2), 0));

	m_flyCamera.Update();
}

void Application3D::draw() {
	// IMGUI
	ImGui::Begin("Lights");
	ImGui::SliderFloat3("Ambient Light Colour", &m_ambientLight.x, 0, 1);
	ImGui::BeginChildFrame(ImGuiID("Directional Light"), ImVec2(0, 0));
	ImGui::SliderFloat3("Direction Light", &m_testLight.dds[0].x, -20, 20);
	ImGui::SliderFloat3("Diffusion Light", &m_testLight.dds[1].x, 0, 1);
	ImGui::SliderFloat3("Specular Light", &m_testLight.dds[2].x, 0, 1);
	ImGui::EndChildFrame();
	ImGui::End();
	//

	// bind our render target
	//m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	/*m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);*/
	m_projectionMatrix = m_flyCamera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_flyCamera.GetViewMatrix();
	
	// bind shader
	//m_shader.bind();
	auto pvmBunny = m_projectionMatrix * m_viewMatrix * m_spearInstance->getTransform();	
	m_spearInstance->addBinding("Ia", m_ambientLight);
	////m_shader.bindUniform("Id", m_light.diffuse);
	////m_shader.bindUniform("Is", m_light.specular);
	////m_shader.bindUniform("LightDirection", m_light.direction);
	m_spearInstance->addBinding("Light", m_testLight.light);
	m_spearInstance->addBinding("ProjectionViewModel", pvmBunny);
	m_spearInstance->addBinding("ModelMatrix", m_bunnyTransform);
	m_spearInstance->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_spearInstance->addBinding("roughness", 1.0f);
	m_spearInstance->addBinding("reflectionCoefficient", 1.0f);
	m_spearInstance->draw();

	pvmBunny = m_projectionMatrix * m_viewMatrix * m_spearInstance2->getTransform();
	m_spearInstance2->addBinding("Ia", m_ambientLight);
	////m_shader.bindUniform("Id", m_light.diffuse);
	////m_shader.bindUniform("Is", m_light.specular);
	////m_shader.bindUniform("LightDirection", m_light.direction);
	m_spearInstance2->addBinding("Light", m_testLight.light);
	m_spearInstance2->addBinding("ProjectionViewModel", pvmBunny);
	m_spearInstance2->addBinding("ModelMatrix", m_bunnyTransform);
	m_spearInstance2->addBinding("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));
	m_spearInstance2->addBinding("roughness", 1.0f);
	m_spearInstance2->addBinding("reflectionCoefficient", 1.0f);
	m_spearInstance2->draw();

	//m_renderTarget.unbind();
	//clearScreen();

	// draw quad
	m_planeShader.bind();
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_planeShader.bindUniform("ProjectionViewModel", pvm);
	m_planeShader.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);
	//m_renderTarget.getTarget(0).bind(0);
	m_quadMesh.draw();
	

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

}