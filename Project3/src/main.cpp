#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 정점 셰이더 생성
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 셰이더 문자열 1개 전달
	glCompileShader(vertexShader); // 컴파일
	// 아래는 컴파일 에러 체크 과정
	int  success; char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// 마찬가지로 프레그먼트 셰이더 생성
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// 셰이더 프로그램 생성 및 링크, 기존 셰이더 제거
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::POGRAM\n" << infoLog << std::endl;
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProgram); // 프로그램 활성화, 이후 호출된 모든 셰이더, 랜더링 명령은 이 프로그램을 사용함


	float vertices[] = {
	-1.0f, -1.0f, 0.0f,
	 -1.0f, 1.0f, 0.0f,
	 -0.5f,  -1.0f, 0.0f
	};

	// OpenGL에는 여러 타입의 버퍼 오브젝트들이 있으며, GPU에 많은 양의 정점 데이터를 담을 수 있는
	// VBO의 타입은 GL_ARRAY_BUFFER 이다.

	// GPU가 데이터를 다루는 타입으로 세 가지를 지정 가능하다
	// GL_STREAM_DRAW : 데이터가 한 번만 세팅되고, 최소한으로 사용됨
	// GL_STATIC_DRAW : 데이터가 한 번만 세팅되고, 자주 사용됨
	// GL_DYNAMIC_DRAW : 데이터가 자주 변하며, 자주 사용됨

	// VBO 는 정점의 정보 그 자체를 가진다.
	// 정점 속성(vertex attribute)는 vbo 데이터를 어떻게 나누어, 정점 셰이더의 어느 변수에, 어떻게 연결시킬지 방식을 결정한다.
	// VAO는 이러한 정점 속성들을 저장한다.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // VAO 생성
	glBindVertexArray(VAO); // VAO 바인드
	unsigned int VBO;
	glGenBuffers(1, &VBO); // VBO 생성
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO 바인드

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 크기 36바이트인 정점 데이터 전달 (바인딩된 VBO에)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 바인드된 VBO에 정점 속성 부여 (현재 바인딩된 VAO에 저장됨)
	glEnableVertexAttribArray(0);

	// EBO를 활용한 사각형 렌더링
	// VAO활성화 시 EBO또한 같이 활성화 시키는 기능이 있음 즉 VAO가 EBO를 참조한다고 보면 됨.

	float rectVertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int VAO2, VBO2, EBO;
	glGenVertexArrays(1, &VAO2); // VAO 생성
	glBindVertexArray(VAO2); // VAO 바인드
	glGenBuffers(1, &VBO2); // VBO 생성
	glBindBuffer(GL_ARRAY_BUFFER, VBO2); // VBO 바인드
	glGenBuffers(1, &EBO); // EBO 생성
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // EBO 바인드

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW); // VBO 데이터
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // EBO 데이터
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 정점 속성 정의
	glEnableVertexAttribArray(0);




	while (!glfwWindowShouldClose(window)) {
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 삼각형은 일반 채우기 모드로 그리기
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 사각형은 와이어프레임 모드로 그리기
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}