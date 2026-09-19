#version 330 core
out vec4 FragColor;

in vec2 TexCoord; /* uv */
in vec3 Normal; /* 노멀 벡터 */
in vec3 FragPos; /* 픽셀의 월드상 위치 */

/*
색 * 스칼라 -> 기존 색 비율 그대로 증폭 / 감쇠
색 * 색(벡터) -> 기존 색에서 원하는 요소만 증폭 / 감쇠
색 + 색 -> 색 합성
*/

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

/* 오브젝트 원색 결정 요소 */
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureInterpolate;
uniform vec3 color;
uniform float colorInterpolate;

/* 카메라 위치 */
uniform vec3 viewPos;

void main()
{
	vec3 textureColor = vec3(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureInterpolate));
	vec3 objectColor = mix(textureColor, color, colorInterpolate);

	/*
	light.amb * (mat.amb * obj)
	+
	light.diff * ((diff * mat.diff) * obj)
	+
	light.spec * ((spec * mat.spec) * obj)
	*/

	vec3 ambient = light.ambient * material.ambient;

	//FragColor = vec4(ambient * objectColor, 1.0);

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);    
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);  

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}