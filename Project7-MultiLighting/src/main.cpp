#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "texture.h"
#include "gameobject.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL_PATH(file) "../common/GLSL/"#file
#define IMAGE_PATH(file) "../common/image/"#file

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void setUniforms_common(GameObject& object);
void setUniforms_multiLight(GameObject& object);
GLFWwindow* init();

// Global
float deltaTime = 0;
float lastFrame = 0;

CubeMesh cubeMesh;
PlaneMesh planeMesh;
Camera camera(glm::vec3(0, 0, 10));
GameObject* lightCube;
GameObject* plane;
GameObject* cubes[3];
LightProfile globalLight;

int main() {
	GLFWwindow* window = init();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 콜백 등록
	glfwSetCursorPosCallback(window, mouseCallback); // 콜백 등록
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 커서가 보이지 않도록 설정
	glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

	Shader shader_multiLight(GLSL_PATH(multiLight.vs), GLSL_PATH(multiLight.fs));
	Shader shader_common(GLSL_PATH(common.vs), GLSL_PATH(common.fs));

	Texture texture0(IMAGE_PATH(container2.png), GL_RGBA);
	Texture texture1(IMAGE_PATH(container_specular.png), GL_RGBA);
	Texture texture2(IMAGE_PATH(container.jpg), GL_RGB);
	Texture texture3(IMAGE_PATH(white.png), GL_RGBA);

	for (int i = 0; i < 3; i++) {
		cubes[i] = new GameObject((float*)cubeMesh.verticesData, (int*)cubeMesh.indices, sizeof(cubeMesh.verticesData), sizeof(cubeMesh.indices));
		cubes[i]->SetShader(&shader_common, setUniforms_common);
		cubes[i]->size = glm::vec3(0.2f, 0.2f, 0.2f);
	}
	lightCube = new GameObject((float*)cubeMesh.verticesData, (int*)cubeMesh.indices, sizeof(cubeMesh.verticesData), sizeof(cubeMesh.indices));
	lightCube->SetShader(&shader_multiLight, setUniforms_multiLight);
	plane = new GameObject((float*)planeMesh.verticesData, (int*)planeMesh.indices, sizeof(planeMesh.verticesData), sizeof(planeMesh.indices));
	plane->SetShader(&shader_multiLight, setUniforms_multiLight);
	
	lightCube->material.diffuseTexture = &texture0;
	lightCube->material.specularTexture = &texture1;
	lightCube->position = glm::vec3(0, 2, 0);
	
	plane->material.diffuseTexture = &texture2;
	plane->material.specularTexture = &texture3;
	plane->size = glm::vec3(10, 1, 10);

	cubes[0]->position = glm::vec3(0, 3, -2);
	cubes[1]->position = glm::vec3(1.732f * 2, 3, 1);
	cubes[2]->position = glm::vec3(-1.732f * 2, 3, 1);

	cubes[0]->lightprofile.SetLightColor(glm::vec3(1, 0, 0));
	cubes[1]->lightprofile.SetLightColor(glm::vec3(0, 1, 0));
	cubes[2]->lightprofile.SetLightColor(glm::vec3(0, 0, 1));
	cubes[0]->material.color = glm::vec3(1, 0, 0);

	globalLight.SetLightColor(glm::vec3(1, 1, 0));
	globalLight.spec = (glm::vec3(1, 1, 1));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 3; i++) {
			cubes[i]->Draw();
		}
		lightCube->Draw();
		plane->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// 델타타임 계산
		float current = glfwGetTime();
		deltaTime = current - lastFrame;
		lastFrame = current;


		glm::mat4 m = glm::mat4(1.0f); // 단위행렬 I
		m = glm::rotate(m, deltaTime * 0.2f, glm::vec3(0.0, 0.0, 1.0));
		globalLight.dir = m * glm::vec4(globalLight.dir, 1);
		

		
	}

	glfwTerminate();
	return 0;
}
void setUniforms_common(GameObject& object) {
	Shader* shader = object.shader;
	shader->setMat4("model", object.getModel());
	shader->setMat4("view", camera.GetLookAt());
	shader->setMat4("projection", camera.GetProjection());
	shader->setVec3("color", object.material.color);
}
void setUniforms_multiLight(GameObject& object) {
	object.material.diffuseTexture->activeAndBind(GL_TEXTURE0);
	object.material.specularTexture->activeAndBind(GL_TEXTURE1);
	Shader* shader = object.shader;
	shader->setMat4("model", object.getModel());
	shader->setMat4("view", camera.GetLookAt());
	shader->setMat4("projection", camera.GetProjection());
	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);
	shader->setFloat("material.shininess", object.material.shininess);
	shader->setVec3("viewPos", camera.position);

	shader->setVec3("dirLight.dir", globalLight.dir);
	shader->setVec3("dirLight.phn.amb", globalLight.amb);
	shader->setVec3("dirLight.phn.diff", globalLight.diff);
	shader->setVec3("dirLight.phn.spec", globalLight.spec);

	for (int i = 0; i < 3; i++) {
		std::string s = "spotLights[";
		s = s + std::to_string(i) + "].";
		shader->setVec3(s + "dir", cubes[i]->lightprofile.dir);
		shader->setVec3(s + "pos", cubes[i]->position);
		shader->setVec3(s + "phn.amb", cubes[i]->lightprofile.amb);
		shader->setVec3(s + "phn.diff", cubes[i]->lightprofile.diff);
		shader->setVec3(s + "phn.spec", cubes[i]->lightprofile.spec);
		shader->setFloat(s + "att.constant", cubes[i]->lightprofile.constant);
		shader->setFloat(s + "att.linear", cubes[i]->lightprofile.linear);
		shader->setFloat(s + "att.quad", cubes[i]->lightprofile.quad);
		//shader->setFloat(s + "att.constant", 1);
		//shader->setFloat(s + "att.linear", 0);
		//shader->setFloat(s + "att.quad", 0);
		shader->setFloat(s + "inCutOff", glm::cos(glm::radians(cubes[i]->lightprofile.inCutOff)));
		shader->setFloat(s + "outCutOff", glm::cos(glm::radians(cubes[i]->lightprofile.outCutOff)));
	}
}

GLFWwindow* init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cerr << "Failed to initialize GLAD" << std::endl;
	}
	return window;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// 화면 크기와 비율이 변해도 항상 정사각형 영역을 랜더링
	if (width > height) {
		glViewport(0, -((width - height)/2), width, width);
	}
	else {
		glViewport(-((height - width) / 2), 0, height, height);
	}
}

bool ctrl = false;
bool alt = false;
bool shift = false;
bool Z = false;
bool X = false;
bool C = false;
bool kp1 = false;
bool kp2 = false;
bool kp3 = false;
bool W = false;
bool A = false;
bool S = false;
bool D = false;
bool I = false;
bool O = false;

bool toggleP = false;
bool flagP = false;

void processInput(GLFWwindow* window) {
	ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
	shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	Z = glfwGetKey(window, GLFW_KEY_Z);
	X = glfwGetKey(window, GLFW_KEY_X);
	C = glfwGetKey(window, GLFW_KEY_C);
	kp1 = glfwGetKey(window, GLFW_KEY_KP_1);
	kp2 = glfwGetKey(window, GLFW_KEY_KP_2);
	kp3 = glfwGetKey(window, GLFW_KEY_KP_3);
	W = glfwGetKey(window, GLFW_KEY_W);
	A = glfwGetKey(window, GLFW_KEY_A);
	S = glfwGetKey(window, GLFW_KEY_S);
	D = glfwGetKey(window, GLFW_KEY_D);
	I = glfwGetKey(window, GLFW_KEY_I);
	O = glfwGetKey(window, GLFW_KEY_O);
	

	if (glfwGetKey(window, GLFW_KEY_P)) {
		if (!flagP) {
			toggleP = !toggleP;
			flagP = true;
		}
	}
	else {
		flagP = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	camera.keyInput(deltaTime, shift, W, A, S, D);
	camera.projectionKetInput(deltaTime, toggleP, I, O, ctrl);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.mouseInput(xpos, ypos);
}