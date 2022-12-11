#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>

struct PointLight {
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 atten;

	void Load(GLuint program)
	{
		std::string prefix = "pointl.";
		glUniform3fv(glGetUniformLocation(program, (prefix + "pos").c_str()), 1, glm::value_ptr(pos));
		glUniform3fv(glGetUniformLocation(program, (prefix + "ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, (prefix + "diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, (prefix + "specular").c_str()), 1, glm::value_ptr(specular));
		glUniform3fv(glGetUniformLocation(program, (prefix + "atten").c_str()), 1, glm::value_ptr(atten));
	}
};

struct DirLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void Load(GLuint program)
	{
		std::string prefix = "dirl.";
		glUniform3fv(glGetUniformLocation(program, (prefix + "direction").c_str()), 1, glm::value_ptr(direction));
		glUniform3fv(glGetUniformLocation(program, (prefix + "ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, (prefix + "diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, (prefix + "specular").c_str()), 1, glm::value_ptr(specular));
	}
};

struct SpotLight {
	glm::vec3 pos;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float cutoff;
	glm::vec3 atten;

	void Load(GLuint program)
	{
		std::string prefix = "spotl.";
		glUniform3fv(glGetUniformLocation(program, (prefix + "pos").c_str()), 1, glm::value_ptr(pos));
		glUniform3fv(glGetUniformLocation(program, (prefix + "direction").c_str()), 1, glm::value_ptr(direction));
		glUniform3fv(glGetUniformLocation(program, (prefix + "ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, (prefix + "diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, (prefix + "specular").c_str()), 1, glm::value_ptr(specular));
		glUniform1f(glGetUniformLocation(program, (prefix + "cutoff").c_str()), cutoff);
		glUniform3fv(glGetUniformLocation(program, (prefix + "atten").c_str()), 1, glm::value_ptr(atten));
	}
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emission;
	float shininess;

	void Load(GLuint program)
	{
		std::string prefix = "material.";
		glUniform3fv(glGetUniformLocation(program, (prefix + "ambient").c_str()), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, (prefix + "diffuse").c_str()), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, (prefix + "specular").c_str()), 1, glm::value_ptr(specular));
		glUniform3fv(glGetUniformLocation(program, (prefix + "emission").c_str()), 1, glm::value_ptr(emission));
		glUniform1f(glGetUniformLocation(program, (prefix + "shininess").c_str()), shininess);
	}
	
};