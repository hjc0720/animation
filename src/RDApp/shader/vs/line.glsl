#version 430

layout (location = 0) in vec4 pos; 
layout (location = 1) in vec4 color; 

out vec4 oColor;

layout(binding = 0, std140) uniform Mat
{
	mat4 MVP;
};

void main()
{
	gl_Position = MVP * pos;
    oColor = color;
}
