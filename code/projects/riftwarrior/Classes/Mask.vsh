//1
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

//2
uniform mat4 u_MVMatrix;
uniform mat4 u_PMatrix;

//3
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//4
void main()
{
    //5
    gl_Position = u_PMatrix * u_MVMatrix * a_position;
    v_fragmentColor = a_color;
    //6
    v_texCoord = a_texCoord;
}
