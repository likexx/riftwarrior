//1
attribute vec4 a_position;
attribute vec2 a_texCoord;

//2
uniform mat4 u_MVPMatrix;

//3
#ifdef GL_ES
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

//4
void main()
{
    //5
    gl_Position = u_MVPMatrix * a_position;
    //6
    v_texCoord = a_texCoord;
}

