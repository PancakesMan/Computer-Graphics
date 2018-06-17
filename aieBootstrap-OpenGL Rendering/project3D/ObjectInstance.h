#pragma once
#include <glm\glm.hpp>
#include <map>

namespace aie {
	class ShaderProgram;
}

class ObjectInstance {
public:
	// ctor takes name, mesh pointer, and shader pointer
	ObjectInstance(const char* name, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	~ObjectInstance();

	void draw();

	// set of functions to bind variables to the shader
	void addBinding(char* name, glm::vec2 value);
	void addBinding(char* name, glm::vec3 value);
	void addBinding(char* name, glm::vec4 value);
	void addBinding(char* name, glm::mat2 value);
	void addBinding(char* name, glm::mat3 value);
	void addBinding(char* name, glm::mat4 value);
	void addBinding(char* name, int count, glm::mat3* value);
	void addBinding(char* name, float value);
	void addBinding(char* name, int value);

	// functions to change objects position, rotation, and scale
	void setPos(glm::vec3 pos) { m_pos = pos; }
	void setEuler(glm::vec3 euler) { m_euler = euler; }
	void setScale(glm::vec3 scale) { m_scale = scale; }

	// return transform of object
	glm::mat4 getTransform();

protected:
	// struct used to store multiple types
	// as a single value in a map
	struct Mat3Arr {
		glm::mat3* arr;
		int size;
	};

private:
	// name for display purposes
	const char* m_name;

	// maps for binding vectors to shader
	std::map<char*, glm::vec2>  m_vec2Bindings;
	std::map<char*, glm::vec3>  m_vec3Bindings;
	std::map<char*, glm::vec4>  m_vec4Bindings;

	// maps for binding matrices to shader
	std::map<char*, glm::mat2>  m_mat2Bindings;
	std::map<char*, glm::mat3>  m_mat3Bindings;
	std::map<char*, glm::mat4>  m_mat4Bindings;

	// map for binding mat3 arrays to shader
	std::map<char*, Mat3Arr>    m_mat3arrBindings;

	// maps for binding int and float to shader
	std::map<char*, float>      m_floatBindings;
	std::map<char*, int>        m_intBindings;

	// shader pointer, mesh pointer
	aie::ShaderProgram*   m_shader;
	aie::OBJMesh*         m_mesh;

	// ambient light for shader
	glm::vec3             m_ambientLight;

	// position, rotation, scale
	glm::vec3 m_pos, m_euler, m_scale;
};