#ifdef GL_ES
precision mediump float;
#endif
 
varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_time;
 
// 1
const float speed = 2.0;
const float bendFactor = 0.2;
void main()
{
  // 2
  float height = 1.0 - v_texCoord.y;
  // 3
  float offset = pow(height, 2.5);
 
  // 4 multiply by sin since it gives us nice bending
  offset *= (sin(u_time * speed) * bendFactor);
 
  // 5
  vec3 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb;
  gl_FragColor = vec4(normalColor, 1);
}
