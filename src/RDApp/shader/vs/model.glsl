#version 430

layout (location = 0) in vec4 pos; 
layout (location = 2) in vec4 normal; 
layout (location = 3) in vec2 uv; 

out vec3 oNormal;
out vec2 oUv;
out vec3 oWorldPos;

layout(binding = 0, std140) uniform Mat
{
	mat4 MVP;
	mat4 ModelMat;
	mat4 NormalMat;
};

void main()
{
	gl_Position = MVP * pos;
    oNormal = normalize(vec3(NormalMat * normal));
    oUv = uv;
    oWorldPos = vec3(ModelMat * pos);
}
