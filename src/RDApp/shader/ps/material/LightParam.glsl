struct PointLightInfo
{
    vec3 Position;
    vec3 Intensity;
};

layout(binding = 3, std140)uniform lights
{
    vec3            eyePos;
    PointLightInfo points[POINT_COUNT];
};

void ads( int lightIndex, vec3 position, vec3 norm, inout vec3 diff,inout vec3 spec)
{
    vec3 s = normalize( points[lightIndex].Position - position );
    vec3 v = normalize(eyePos - position);
    vec3 r = reflect( -s, norm );
    vec3 I = points[lightIndex].Intensity;
    diff += DiffuseColor.rgb * max( dot(s, norm), 0.0 );
    spec += SpecColor.rgb * pow( max( dot(r,v), 0.0 ), fSpecPower );
}
