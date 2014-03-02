{
    vec4 vTexCoord = vec4(oUv,0,1);
    vTexCoord = Diffuse.TexMat * vTexCoord;
    vec4 vTex = texture(DiffuseTex,vTexCoord.xy);
    vDiffuse *= vTex.rgb; 
    fOutAlpha *= vTex.a * Diffuse.fAlpha; 
    //vDiffuse = vTexCoord;
}
