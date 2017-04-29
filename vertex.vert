#version 330
layout(location = 0) in vec3 modelspace;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP*vec4(modelspace,1.0f);
}