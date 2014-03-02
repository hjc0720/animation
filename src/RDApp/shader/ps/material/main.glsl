#version 430
layout(location = 0) out vec4 vColor;

in vec3 oNormal;
in vec2 oUv;
in vec3 oWorldPos;

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
    vec3 vDiffuse = DiffuseColor.rgb, vSpec = vec3(0);
    float fOutAlpha = fAlpha * DiffuseColor.a;
    //begin code
    //end code
    //vColor = vDiffuse + vSpec; 
    vColor = vec4(vDiffuse ,1);
}
