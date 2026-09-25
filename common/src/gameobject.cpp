#include "gameobject.h"

GameObject::GameObject(float* vdata, int* idata, size_t vsize, size_t isize) : verticesData(vdata), indices(idata), vSize(vsize), iSize(isize) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, vSize, verticesData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // uv
	glEnableVertexAttribArray(2);
}
glm::mat4 GameObject::getModel() {
	// 사이즈 -> z회전 -> x회전 -> y회전 -> 이동 
	glm::mat4 model = glm::mat4(1.0f); // 단위행렬 I
	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, size);
	return model;
}
void GameObject::SetShader(Shader* shader, void (*uniformFunc)(GameObject&)) {
	this->shader = shader;
	this->uniformFunc = uniformFunc;
}

void GameObject::Draw() {
	this->shader->use();
	this->uniformFunc(*this);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0);
}