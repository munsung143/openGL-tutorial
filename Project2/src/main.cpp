#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	// GLFW 초기화
	glfwInit();

	// GLFW 설정
	// GLFW_ 로 시작하는 enum들은 설정할 다양한 옵션을 의미.
	// https://www.glfw.org/docs/latest/window.html#window_hints
	// 아래 코드는 OpenGL 3.3 Core Profile을 사용하겠다는 의미.
	// Core Profile은 OpenGL 구버전 기능을 제외하고, 특정 작은 subset들을 사용.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 윈도우 객체 생성, 윈도우 관련 정보들을 가지고 있음.
	// GLFW의 다른 함수들을 사용하기 위해 요구됨.
	// (아래 코드들 2번 쓰면 윈도우 2개뜸)
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 현재 스레드에 해당 윈도우에 대한 컨텍스트 생성
	glfwMakeContextCurrent(window);

	// GLAD 초기화
	// OS 별로 다른 OpenGL 함수를 가져오는 함수(glfwGetProcAddress)를 GLAD에 전달.
	// GLADloadproc은 (const char *name)을 인자로 받아 void*를 반환하는 함수 포인터 타입.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 콜백 등록
	// 두 번째 인자는 void(GLFWWindow*, int, int) 함수에 대한 포인터
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 게임 루프
	while (!glfwWindowShouldClose(window)) {

		// 인풋 처리
		processInput(window);

		// 렌더링 영역
		// 색상 버퍼 색 설정 및 초기화(클리어)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 버퍼 스왑
		// 색상 버퍼 : 스크린에 출력될 각 픽셀 색상을 담은 2D 버퍼
		// 최종 출력 색상을 담은 앞 버퍼와,
		// 랜더링 값 계산의 결과가 쓰여지는 뒤 버퍼를
		// 계산이 끝난 후에 스왑해서 계산 중간 과정이 출력되지 않도록 하는 기법
		glfwSwapBuffers(window);

		// 등록된 이벤트 호출, 윈도우 상태 업데이트 등
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


// 유저가 윈도우의 크기를 바꾸는 경우, 뷰포트 크기 또한 바뀌어야 한다.
// 따라서 해당 콜백 함수를 다음과 같이 만들 수 있음
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// 뷰포트 설정
	// 윈도우에 대하여 랜더링할 영역 사이즈 지정.
	// x, y : 뷰포트의 왼쪽 아래 모서리 좌표 (범위 -1~1)
	// width, height : 뷰포트의 너비와 높이
	// 아래는 윈도우 크기와 뷰포트 크기를 동일하게 맞춘다.
	glViewport(0, 0, width, height);
}

// ESC 누르면 꺼지도록 설정
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}