#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
class Camera
{
public:
	glm::vec3 position; // 위치
	glm::vec3 front = glm::vec3(0, 0, -1); // 바라보는 방향
	
	float speed = 2; // 이동속도

	bool firstMouseInput = false; // 첫 마우스 인풋 플래그
	double beforeX; // 이전 프레임 마우스 X 위치
	double beforeY; // 이전 프레임 마우스 Y 위치

	float pitch; // x축 기준 회전량
	float yaw = -90; // y축 기준 회전량

	float fovy = 45.0f;
	float orthoVolum = 10.0f;

	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)800 / (float)800, 0.1f, 100.0f);
	glm::mat4 ortho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 100.0f);
	glm::mat4* projection;

	Camera(glm::vec3 pos);
	glm::mat4 GetLookAt();
	glm::mat4 GetProjection();
	void keyInput(float deltaTime, bool shift, bool W, bool A, bool S, bool D);
	void mouseInput(double xpos, double ypos);
	void projectionKetInput(float deltaTime, bool toggleP, bool I, bool O, bool ctrl);
};