#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform vec3 color;

void main()
{
	vec3 result = color;
	//result += vec3(1, 1, 1);
	FragColor = vec4(result, 1.0);
}