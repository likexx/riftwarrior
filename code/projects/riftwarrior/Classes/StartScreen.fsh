#ifdef GL_ES
precision mediump float;
#endif
 
// 1
varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float u_time;


void main()
{
  // 2
  vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);
 
  // 3
//  vec2 texCoord = v_texCoord;
 vec2 texCoord = v_texCoord;
  texCoord.x += sin(u_time) * (onePixel.x * 6.0);
  texCoord.y += cos(u_time) * (onePixel.y * 6.0);


  // 4

  vec4 color;
  color.rgb = vec3(0.5);

  color -= texture2D(u_texture, texCoord - onePixel) * 5.0;
  color += texture2D(u_texture, texCoord + onePixel) * 5.0;

 vec3 normalColor = texture2D(u_texture, texCoord + onePixel).rgb;

  // 5
  color.rgb = vec3((color.r + color.g + color.b) / 3.0);
 
  gl_FragColor = vec4(normalColor, 1);
}
