

varying  vec2 v_texCoord;
varying  vec4 v_fragmentColor;//not used


///uniform  sampler2D CC_Texture0;

void main()
{
    vec4 texColor= texture2D(CC_Texture0, v_texCoord);
    float r=texColor.r;
    float d=pow(r,1000.0);
    gl_FragColor = vec4(d,d,d,1.0);
}
