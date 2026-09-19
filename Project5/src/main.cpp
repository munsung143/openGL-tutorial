#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "texture.h"
#include "cube.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL_PATH(file) "../common/GLSL/"#file
#define IMAGE_PATH(file) "../common/image/"#file

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Cube& cube);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

GLFWwindow* init();

float deltaTime = 0;
float lastFrame = 0;

Camera camera(glm::vec3(0, 0, 10));

int main() {
	GLFWwindow* window = init();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 콜백 등록
	glfwSetCursorPosCallback(window, mouseCallback); // 콜백 등록
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 커서가 보이지 않도록 설정
	glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

	Shader shader(GLSL_PATH(cameraShader.vs), GLSL_PATH(cameraShader.fs));
	Texture texture0(IMAGE_PATH(container.jpg), GL_RGB);
	Texture texture1(IMAGE_PATH(awesomeface.png), GL_RGBA);
	Cube cube(glm::vec3(0, 0, 0));


	while (!glfwWindowShouldClose(window)) {
		processInput(window, cube);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture0.activeAndBind(GL_TEXTURE0); // 0번 유닛을 통한 텍스처 전달
		texture1.activeAndBind(GL_TEXTURE1);

		// 사이즈 -> z회전 -> x회전 -> y회전 -> 이동 
		glm::mat4 model = glm::mat4(1.0f); // 단위행렬 I
		model = glm::translate(model, cube.position);
		model = glm::rotate(model, cube.rotation.y, glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, cube.rotation.x, glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, cube.rotation.z, glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, cube.size);

		shader.use();
		shader.setInt("texture1", 0); // 0번 유닛으로 온 텍스처가 texture1로 간다
		shader.setInt("texture2", 1);
		shader.setMat4("model", model);
		shader.setMat4("view", camera.GetLookAt());
		shader.setMat4("projection", camera.GetProjection());
		cube.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// 델타타임 계산
		float current = glfwGetTime();
		deltaTime = current - lastFrame;
		lastFrame = current;
	}

	glfwTerminate();
	return 0;
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

void processInput(GLFWwindow* window, Cube& cube) {
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

	cube.keyInput(deltaTime, Z, X, C, ctrl, kp1, kp2, kp3);
	camera.keyInput(deltaTime, shift, W, A, S, D);
	camera.projectionKetInput(deltaTime, toggleP, I, O, ctrl);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.mouseInput(xpos, ypos);
}