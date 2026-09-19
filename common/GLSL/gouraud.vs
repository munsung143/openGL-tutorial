#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 LightValue;

struct Material {
	vec3 ambient; /* 오브젝트가 주변광을 얼마나 받을지 */
	vec3 diffuse; /* 오브젝트가 분산광을 얼마나 받을지 */
	vec3 specular; /* 오브젝트가 반사광을 얼마나 받을지 */
	float shininess; /* 반사광의 모이는 정도 */
};
struct Light {
	vec3 position; /* 빛의 위치 */
	vec3 ambient; /* 주변광의 색상 */
	vec3 diffuse; /* 분산광의 색상 */
	vec3 specular; /* 반사광의 색상 */
};
uniform Material material;
uniform Light light;

uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 FragPos = vec3(model * vec4(aPosition, 1.0f));

	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);    
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);  

	LightValue = ambient + diffuse + specular;
}