#pragma once
#include <imgui.h>
#include <glm\ext.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "ObjectInstance.h"

//ObjectInstance::ObjectInstance(aie::ShaderProgram* shader)
//{
//	m_shader = shader;
//}

ObjectInstance::ObjectInstance(const char* name, aie::OBJMesh* mesh, aie::ShaderProgram* shader)
{
	m_name = name;
	m_mesh = mesh;
	m_shader = shader;

	m_pos = glm::vec3(0);
	m_euler = glm::vec3(0);
	m_scale = glm::vec3(1);
}

ObjectInstance::~ObjectInstance()
{
	/*delete m_shader;
	delete m_mesh;*/
}

void ObjectInstance::draw()
{
	ImGui::Begin((std::string(m_name) + std::to_string((int)&m_name)).c_str());
	ImGui::SliderFloat3("Position", &m_pos.x, -20, 20);
	ImGui::SliderFloat3("Rotation", &m_euler.x, 0, 360);
	ImGui::SliderFloat3("Scale", &m_scale.x, 0, 10);
	ImGui::End();

	m_shader->bind();

	for (auto const& value : m_vec2Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_vec3Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_vec4Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_mat2Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_mat3Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_mat4Bindings)  m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_mat3arrBindings) m_shader->bindUniform(value.first, value.second.size, value.second.arr);
	for (auto const& value : m_floatBindings) m_shader->bindUniform(value.first, value.second);
	for (auto const& value : m_intBindings)   m_shader->bindUniform(value.first, value.second);

	m_mesh->draw();
}

void ObjectInstance::addBinding(char* name, glm::vec2 value)
{
	m_vec2Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, glm::vec3 value)
{
	m_vec3Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, glm::vec4 value)
{
	m_vec4Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, glm::mat2 value)
{
	m_mat2Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, glm::mat3 value)
{
	m_mat3Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, glm::mat4 value)
{
	m_mat4Bindings[name] = value;
}

void ObjectInstance::addBinding(char* name, int count, glm::mat3* value)
{
	m_mat3arrBindings[name] = { value, count };
}

void ObjectInstance::addBinding(char* name, float value)
{
	m_floatBindings[name] = value;
}

void ObjectInstance::addBinding(char* name, int value)
{
	m_intBindings[name] = value;
}

glm::mat4 ObjectInstance::getTransform()
{
	return glm::translate(glm::mat4(1), m_pos)
		* glm::rotate(glm::mat4(1), m_euler.z * (3.142f / 180), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), m_euler.y * (3.142f / 180), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), m_euler.x * (3.142f / 180), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), m_scale);
}