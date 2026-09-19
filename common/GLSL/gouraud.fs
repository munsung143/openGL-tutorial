#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 LightValue;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureInterpolate;
uniform vec3 color;
uniform float colorInterpolate;

/* 카메라 위치 */

void main()
{
	vec3 textureColor = vec3(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureInterpolate));
	vec3 objectColor = mix(textureColor, color, colorInterpolate);

	vec3 result = LightValue * objectColor;
	FragColor = vec4(result, 1.0);

	/*
	light.amb * (mat.amb * obj)
	+
	light.diff * ((diff * mat.diff) * obj)
	+
	light.spec * ((spec * mat.spec) * obj)
	*/
}