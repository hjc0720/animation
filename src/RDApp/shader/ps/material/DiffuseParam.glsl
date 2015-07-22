//begin Diffuse
layout(binding = 2, std140)uniform DiffuseBuffer
{
	mat4 TexMat;
    float fAlpha;
}Diffuse;

layout(binding = 0)uniform sampler2D DiffuseTex;
//end Diffuse
