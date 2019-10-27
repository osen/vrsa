#ifdef VERTEX

attribute vec3 a_Position;
attribute vec2 a_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

varying vec2 v_TexCoord;

void main()
{
  gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1);
  v_TexCoord = a_TexCoord;
}

#endif

#ifdef FRAGMENT

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_Time;

varying vec2 v_TexCoord;

void main()
{
  vec4 tex = texture2D(u_Texture, v_TexCoord);
  vec4 color = (u_Color + tex) / 2;

  if(u_Time != 0)
  {
    float pulse = 0.5 + (1 - sin(u_Time * 5) / 2);
    color.rgb *= pulse;
  }

  gl_FragColor = color;
  gl_FragColor.w = tex.w;
}

#endif

