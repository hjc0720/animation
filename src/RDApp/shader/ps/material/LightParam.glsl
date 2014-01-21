struct PointLightInfo
{
    vec4 Position;
    vec3 Intensity;
};

layout(binding = 3, std140)uniform lights
{
    vec4            eyePos;
    PointLightInfo points[POINT_COUNT];
};

void ads( int lightIndex, vec4 position, vec3 norm,
        out vec4 diff,out vec4 spec)
{
    vec3 s = normalize( vec3(points[lightIndex].Position –
                position) );
    vec3 v = normalize(vec3(eyePos - position));
    vec3 r = reflect( -s, norm );
    vec3 I = lights[lightIndex].Intensity;
    diff += DiffuseColor * max( dot(s, norm), 0.0 );
    spec += SpecColor * pow( max( dot(r,v), 0.0 ), Shininess );
}
