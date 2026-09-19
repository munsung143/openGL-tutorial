#include "camera.h"

Camera::Camera(glm::vec3 pos) : position(pos) {
	perspective = glm::perspective(glm::radians(fovy), (float)800 / (float)800, 0.1f, 100.0f);
	ortho = glm::ortho(-orthoVolum, orthoVolum, -orthoVolum, orthoVolum, 0.01f, 100.0f);
	projection = &perspective;

}
glm::mat4 Camera::GetLookAt() {
	return glm::lookAt(position, position + front, glm::vec3(0, 1, 0));
}
glm::mat4 Camera::GetProjection() {
	return *projection;
}

void Camera::keyInput(float deltaTime, bool shift, bool W, bool A, bool S, bool D) {
	float curspeed = speed * (shift ? 2 : 1);
	if (W) {
		position += deltaTime * curspeed * front;
	}
	if (S) {
		position -= deltaTime * curspeed * front;
	}
	if (A) {
		position -= deltaTime * curspeed * glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
	}
	if (D) {
		position += deltaTime * curspeed * glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
	}
}
void Camera::projectionKetInput(float deltaTime, bool toggleP, bool I, bool O, bool ctrl) {
	float speed = 20 * deltaTime;
	speed = ctrl ? -speed : speed;
	if (toggleP) {
		projection = &ortho;
	}
	else {
		projection = &perspective;
	}
	if (I) {
		fovy += speed;
		if (fovy > 179.9f) fovy = 179.9f;
		else if (fovy < 0.1f) fovy = 0.1f;
		perspective = glm::perspective(glm::radians(fovy), (float)800 / (float)800, 0.1f, 100.0f);
	}
	if (O) {
		orthoVolum += speed * 2;
		if (orthoVolum > 800) orthoVolum = 800;
		else if (orthoVolum < 0.1f) orthoVolum = 0.1f;
		ortho = glm::ortho(-orthoVolum, orthoVolum, -orthoVolum, orthoVolum, 0.01f, 100.0f);
	}

}

void Camera::mouseInput(double xpos, double ypos) {
	// 마우스가 우하단에 있을수록 큰 값이 반환된다

	// yaw 0에서 front는 x축(1, 0, 0)
	// 왜냐하면 x=cos(0), z=sin(0)
	// 이상태에서 마우스를 좌측으로 이동할수록, x, z는 작아져야함
	// 그래서 yaw가 작아지도록 누적시킴

	float mouseSensitivity = 0.06f;
	if (!firstMouseInput) {
		beforeX = xpos;
		beforeY = ypos;
		firstMouseInput = true;
	}
	int yOffset = beforeY - ypos;
	int xOffset = xpos - beforeX;
	beforeX = xpos;
	beforeY = ypos;
	pitch += yOffset * mouseSensitivity;
	yaw += xOffset * mouseSensitivity;

	if (pitch > 89) pitch = 89;
	else if (pitch < -89) pitch = -89;

	front.y = glm::sin(glm::radians(pitch));
	front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
}