#pragma once
#include <glm\glm.hpp>
#include <map>

namespace aie {
	class ShaderProgram;
}

class ObjectInstance {
public:
	//ObjectInstance(aie::ShaderProgram* shader);
	ObjectInstance(const char* name, aie::OBJMesh* mesh, aie::ShaderProgram* shader);
	~ObjectInstance();

	void draw();

	void addBinding(char* name, glm::vec2 value);
	void addBinding(char* name, glm::vec3 value);
	void addBinding(char* name, glm::vec4 value);
	void addBinding(char* name, glm::mat2 value);
	void addBinding(char* name, glm::mat3 value);
	void addBinding(char* name, glm::mat4 value);
	void addBinding(char* name, float value);
	void addBinding(char* name, int value);

	void setPos(glm::vec3 pos) { m_pos = pos; }
	void setEuler(glm::vec3 euler) { m_euler = euler; }
	void setScale(glm::vec3 scale) { m_scale = scale; }

	glm::mat4 getTransform();

private:
	const char* m_name;

	std::map<char*, glm::vec2> m_vec2Bindings;
	std::map<char*, glm::vec3> m_vec3Bindings;
	std::map<char*, glm::vec4> m_vec4Bindings;

	std::map<char*, glm::mat2> m_mat2Bindings;
	std::map<char*, glm::mat3> m_mat3Bindings;
	std::map<char*, glm::mat4> m_mat4Bindings;

	std::map<char*, float>     m_floatBindings;
	std::map<char*, int>       m_intBindings;

	aie::ShaderProgram*   m_shader;
	aie::OBJMesh*         m_mesh;
	glm::vec3             m_ambientLight;

	glm::vec3 m_pos, m_euler, m_scale;
};