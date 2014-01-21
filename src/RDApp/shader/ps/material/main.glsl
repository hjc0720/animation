#version 430
layout(location = 0) out vec4 vColor;

in vec3 oNormal;
in vec2 oUv;
in vec4 oWorldPos;

layout(binding = 1, std140)uniform Material
{
    vec4    DiffuseColor;
    vec4    SpecColor;
    vec4    AmbientColor;
    float   fSpecPower;
    float   fAlpha;
};
//begin param
//end param
void main()
{
    vec4 vDiffuse = DiffuseColor, vSpec = vec4(0);
    //begin code
    //end code
    vColor = vDiffuse + vSpec; 
}
