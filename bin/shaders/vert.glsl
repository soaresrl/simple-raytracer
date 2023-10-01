#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InColor;

uniform mat4 ModelViewProjection = mat4(1.0);

out vec3 Color;

void main(){
	Color = vec3(1.0,1.0,1.0);

	gl_PointSize = 4;

	gl_Position = ModelViewProjection * vec4(InPosition, 1.0);
}