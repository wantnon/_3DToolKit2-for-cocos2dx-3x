varying  vec2 v_texCoord;
varying  vec4 v_fragmentColor;

varying  vec4 v_lightViewportTexCoordDivW;
//use CC_Texture1 as shadowMap
void main()
{
    float zLookup=texture2D(CC_Texture1, vec2(v_lightViewportTexCoordDivW.x,v_lightViewportTexCoordDivW.y) ).z;
    float dz=v_lightViewportTexCoordDivW.z-zLookup;

    float shadowFactor=(max(0.0,dz)>0.00001)?0.6:1.0;//
    //max(0.6,1.0-max(0.0,dz)*500.0);// the shadow of scene have gradual change, makes more real and natural
    
    vec4 shadowFactorRGBA=vec4(shadowFactor,shadowFactor,shadowFactor,1.0);
    

    gl_FragColor =  texture2D(CC_Texture0, v_texCoord)*v_fragmentColor*shadowFactorRGBA;
   
}


