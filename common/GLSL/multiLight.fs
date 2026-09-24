#version 330 core
out vec4 FragColor;

in vec2 TexCoord; /* uv */
in vec3 Normal; /* 노멀 벡터 */
in vec3 FragPos; /* 픽셀의 월드상 위치 */

struct Material {
  sampler2D diffuse; /* 주변광 및 분산광의 원 색상 */
	sampler2D specular; /* 반사의 정도 */
	float shininess; /* 반사광의 모이는 정도 */
};

struct Phong{
	vec3 amb; /* 주변광의 색상(어두운 색상) */
	vec3 diff; /* 분산광의 색상 */
	vec3 spec; /* 반사광의 색상 */
};

struct Attenuation{
	float constant;
	float linear;
	float quad;
};

struct DirectionalLight {
	vec3 dir; /* 빛의 방향 */
	Phong phn;
};

struct PointLight {
	vec3 pos; /* 빛의 위치 */
	Phong phn;
	Attenuation att;
};

struct SpotLight{
	vec3  pos;
  vec3  dir;
  float inCutOff;
	float outCutOff;
	Phong phn;
	Attenuation att;
};
uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

/* 카메라 위치 */
uniform vec3 viewPos;

vec3 CalcPhong(Phong phn, vec3 lightDir, vec3 normal, vec3 diffuseTexel, vec3 specularTexel, vec3 viewDir){
	vec3 ambient = phn.amb * diffuseTexel;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = phn.diff * (diff * diffuseTexel);

	vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = phn.spec * (spec * specularTexel); 

	return ambient + diffuse + specular;
}

vec3 CalcDirectionalLight(DirectionalLight lit, vec3 normal, vec3 diffuseTexel, vec3 specularTexel, vec3 viewDir) {

	vec3 lightDir = normalize(-lit.dir);
	vec3 calcedPhong = CalcPhong(lit.phn, lightDir, normal, diffuseTexel, specularTexel, viewDir);

  return calcedPhong;
}

vec3 CalcPointLight(Pointlight lit, vec3 normal, vec3 diffuseTexel, vec3 specularTexel, vec3 viewDir) {

	vec3 lightDir = normalize(lit.pos - FragPos);    
	vec3 calcedPhong = CalcPhong(lit.phn, lightDir, normal, diffuseTexel, specularTexel, viewDir);

	float dist = length(lit.pos - FragPos);
	float attenuation = 1.0 / (lit.att.constant + lit.att.linear * dist + lit.att.quad * dist * dist );

	return calcedPhong * attenuation;
}

vec3 CalcSpotLight(SpotLight lit, vec3 normal, vec3 diffuseTexel, vec3 specularTexel, vec3 viewDir){

	vec3 lightDir = normalize(lit.pos - FragPos);
	vec3 calcedPhong = CalcPhong(lit.phn, lightDir, normal, diffuseTexel, specularTexel, viewDir);

	float dist = length(lit.pos - FragPos);
	float attenuation = 1.0 / (lit.att.constant + lit.att.linear * dist + lit.att.quad * dist * dist );

	float theta = dot(lightDir, normalize(-lit.dir));  
	float intensity = clamp((theta - lit.outCutOff) / lit.inCutOff - lit.outCutOff, 0.0, 1.0);

	return calcedPhong * attenuation * intensity;
}

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 diffuseTexel = vec3(texture(material.diffuse, TexCoord));
	vec3 specularTexel = vec3(texture(material.specular, TexCoord));
	FragColor = vec4(result, 1.0);
}