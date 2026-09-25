#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <texture.h>

//        2────────3
//       /│       /│
//      / │      / │
//     6────────7  │
//     │  │     │  │
//     │  0─────│──1
//     │ /      │ /
//     │/       │/
//     4────────5
struct CubeMesh {
	float verticesData[192] = {
		// 앞
		 1,  1, -1,		0, 0, -1,	0, 1, // 3 0
		 1, -1, -1,		0, 0, -1,	0, 0, // 1 1
		-1, -1, -1,		0, 0, -1,	1, 0, // 0 2
		-1,  1, -1,		0, 0, -1,	1, 1, // 2 3

		// 뒤				  
		-1,  1,  1,		0, 0, 1, 	0, 1, // 6 4
		-1, -1,  1,		0, 0, 1, 	0, 0, // 4 5
		 1, -1,  1,		0, 0, 1, 	1, 0, // 5 6
		 1,  1,  1,		0, 0, 1, 	1, 1, // 7 7

		 // 좌					  
		 -1,  1, -1,	-1, 0, 0,	0, 1, // 2 8
		 -1, -1, -1,	-1, 0, 0,	0, 0, // 0 9
		 -1, -1,  1,	-1, 0, 0,	1, 0, // 4 10
		 -1,  1,  1,	-1, 0, 0,	1, 1, // 6 11

		 // 우					  
		  1,  1,  1,	1, 0, 0,	0, 1, // 7 12
		  1, -1,  1,	1, 0, 0,	0, 0, // 5 13
		  1, -1, -1,	1, 0, 0,	1, 0, // 1 14
		  1,  1, -1,	1, 0, 0,	1, 1, // 3 15

		  // 상					  
		  -1,  1, -1,	0, 1, 0,	0, 1, // 2 16
		  -1,  1,  1,	0, 1, 0,	0, 0, // 6 17
		   1,  1,  1,	0, 1, 0,	1, 0, // 7 18
		   1,  1, -1,	0, 1, 0,	1, 1, // 3 19

		   // 하					  
			-1,  -1,  1,	0, -1, 0,	0, 1, // 4 20
			-1,  -1, -1,	0, -1, 0,	0, 0, // 0 21
			 1,  -1, -1,	0, -1, 0,	1, 0, // 1 22
			 1,  -1,  1,	0, -1, 0,	1, 1, // 5 23
	};
	// 오른손 법칙 기준으로
	unsigned int indices[36] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};
};
//        0────────1
//       /        /
//      /        /
//     2────────3
struct PlaneMesh {
	float verticesData[32] = {
		  -1,  0, -1,	0, 1, 0,	0, 1, // 0 0
		  -1,  0,  1,	0, 1, 0,	0, 0, // 2 1
		   1,  0,  1,	0, 1, 0,	1, 0, // 3 2
		   1,  0, -1,	0, 1, 0,	1, 1, // 1 3
	};
	// 오른손 법칙 기준으로
	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3,
	};
};

struct Material {
	Texture* diffuseTexture;
	Texture* specularTexture;
	glm::vec3 color = glm::vec3(1, 1, 1);
	float shininess = 32;
};
struct LightProfile {
	glm::vec3 amb = glm::vec3(0.2f, 0.2f, 0.2f); /* 주변광의 색상(일반적으로 분산광*0.5 이하) */
	glm::vec3 diff = glm::vec3(1, 1, 1); /* 분산광의 색상(일반적으로 하얀색) */
	glm::vec3 spec = glm::vec3(1, 1, 1); /* 반사광의 색상(일반적으로 분산광과 동일) */

	float constant = 1;
	float linear = 0.09f;
	float quad = 0.032f;

	glm::vec3 dir = glm::vec3(0, -1, 0); /* 빛의 방향 */

	float inCutOff = 12.5f; /* 빛이 약해지기 시작하는 각도의 코사인 값 */
	float outCutOff = 30; /* 빛이 끝나는 각도의 코사인 값 */

	void SetLightColor(glm::vec3 color) {
		amb = color * 0.2f;
		diff = color;
		spec = color;
	}
};

class GameObject {
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 size = glm::vec3(1, 1, 1);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	float* verticesData;
	int* indices;
	size_t vSize;
	size_t iSize;

	Shader* shader;
	void (*uniformFunc)(GameObject&);
	Material material;
	LightProfile lightprofile;

	GameObject(float* vdata, int* idata, size_t vsize, size_t isize);
	void SetShader(Shader* shader, void (*uniformFunc)(GameObject&));
	void Draw();
	glm::mat4 getModel();
};