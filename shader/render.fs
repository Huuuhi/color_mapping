#version 330 core


in vec3 Normal;  
in vec3 FragPos;  
in float fragScalarData;

uniform sampler2D textureMap;
uniform float minX;
uniform float maxX;

out vec4 FragColor;

void main()
{
	float normalX = (fragScalarData - minX) / (maxX - minX);
	vec4 color = texture(textureMap, vec2(normalX, 0.0));
	FragColor = color;
} 