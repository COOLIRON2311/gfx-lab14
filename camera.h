#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat3 normal;
	glm::vec3 viewPosition;
};

class Camera
{
	float pitch = 0.0f;
	float yaw = -90.0f;
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model;
	float WalkSpeed = 0.5f;
	float RotSpeed = 2.0f;

	Camera()
	{
		Reset();
	}
	
	void W()
	{
		Pos += WalkSpeed * Front;
	}
	
	void S()
	{
		Pos -= WalkSpeed * Front;
	}
	
	void A()
	{
		Pos -= glm::normalize(glm::cross(Front, Up)) * WalkSpeed;
	}
	
	void D()
	{
		Pos += glm::normalize(glm::cross(Front, Up)) * WalkSpeed;
	}

	void Update()
	{
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front = glm::normalize(newFront);
	}

	void YawPlus()
	{
		yaw += RotSpeed;
		Update();
	}
	
	void YawMinus()
	{
		yaw -= RotSpeed;
		Update();
	}

	void PitchPlus()
	{
		pitch += RotSpeed;
		Update();
	}
	
	void PitchMinus()
	{
		pitch -= RotSpeed;
		Update();
	}

	void Perspective()
	{
		proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);	
	}

	void Ortho()
	{
		proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	}

	void Reset()
	{	
		Pos = glm::vec3(0.0f, 0.0f, 3.0f);
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(Pos, Pos + Front, Up);
		proj = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
		model = glm::mat4(1.0f);
		yaw = -90.0f;
		pitch = 0.0f;
	}
	
	glm::mat4 MVP()
	{
		view = glm::lookAt(Pos, Pos + Front, Up);
		return proj * view * model;
	}

	glm::mat4 Model()
	{
		return model;
	}
	
	glm::mat4 View()
	{
		view = glm::lookAt(Pos, Pos + Front, Up);
		return view;
	}
	
	glm::mat4 Proj()
	{
		return proj;
	}

	Transform GetTransform()
	{
		Transform transform;
		transform.model = model;
		transform.view = View();
		transform.proj = proj;
		transform.normal = glm::mat3(glm::transpose(glm::inverse(transform.model * transform.view)));
		transform.viewPosition = Pos;
		return transform;
	}
};