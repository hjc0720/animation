#version 430

layout (location = 0) in vec4 pos; 
layout (location = 2) in vec4 normal; 
layout (location = 3) in vec2 uv; 

out vec3 oNormal;
out vec2 oUv;
out vec4 oWorldPos;

layout(binding = 0, std140) uniform Mat
{
	mat4 MVP;
	mat3 NormalMat;
	mat4 ModelMat;
};

void main()
{
	gl_Position = MVP * pos;
    oNormal = normalize(NormalMat * normal.xyz);
    oUv = uv;
    oWorldPos = ModelMat * pos;
}
