
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_normal;
attribute vec4 a_color;

uniform mat4 worldToLightViewportTexCoord;
uniform mat4 modelMat;

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

varying vec4 v_lightViewportTexCoordDivW;
void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
    v_fragmentColor = a_color;
    
    vec3 pos_world = vec3(modelMat*a_position);
    vec4 t=worldToLightViewportTexCoord*vec4(pos_world,1);//note: use pos_world
    v_lightViewportTexCoordDivW=t/t.w;
}


