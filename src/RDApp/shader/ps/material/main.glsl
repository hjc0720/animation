#version 400
layout(location = 0) out vec4 vColor;

in vec3 oNormal;
in vec2 oUv;
in vec4 oWorldPos;

uniform float fAlpha;

uniform sampler2D DiffuseTex;
void main()
{
    vColor = texture(DiffuseTex,oUv);
    //vec4 fDiffContrib = 1.f,fSpecContrib = 0.f;
    //vec4 fAmbient = 0.f;
    //vec3 vNorm = oNormal, ViewDir = normalize( EyeWorld - oWorldPos);
    //vColor = fDiffContrib + fSpecContrib + fEmissiveContrib + fAmbient;
    //vColor.a = fDiffContrib.a * fAlpha;
    //return vColor;
}
