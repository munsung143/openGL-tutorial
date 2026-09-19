#include "cube.h"

Cube::Cube(glm::vec3 pos) : position(pos) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Cube::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Cube::keyInput(float deltaTime, bool Z, bool X, bool C, bool ctrl, bool kp1, bool kp2, bool kp3) {
	float speed = 1 * deltaTime;
	speed = ctrl ? -speed : speed;
	if (kp1) {
		rotation.x += Z ? speed : 0;
		size.x += X ? speed : 0;
		position.x += C ? speed : 0;
	}
	if (kp2) {
		rotation.y += Z ? speed : 0;
		size.y += X ? speed : 0;
		position.y += C ? speed : 0;
	}
	if (kp3) {
		rotation.z += Z ? speed : 0;
		size.z += X ? speed : 0;
		position.z += C ? speed : 0;
	}
}

LightCube::LightCube(glm::vec3 pos) : position(pos) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // uv
	glEnableVertexAttribArray(2);
}

void LightCube::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

glm::mat4 LightCube::getModel() {
	// 사이즈 -> z회전 -> x회전 -> y회전 -> 이동 
	glm::mat4 model = glm::mat4(1.0f); // 단위행렬 I
	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, size);
	return model;
}