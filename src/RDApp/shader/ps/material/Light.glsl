{
    vDiffuse = vec4(0);
    for( int i = 0; i < POINT_COUNT; i++ )
        ads( i, oWorldPos, oNormal,vDiffuse,vSpec );
}
