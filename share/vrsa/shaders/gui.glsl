#ifdef VERTEX

attribute vec2 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_Model;

varying vec2 v_TexCoord;

void main()
{
  gl_Position = u_Projection * u_Model * vec4(a_Position, 0, 1);
  v_TexCoord = a_Position;
  v_TexCoord.y = 1 - v_TexCoord.y;
}

#endif

#ifdef FRAGMENT

uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main()
{
  gl_FragColor = texture2D(u_Texture, v_TexCoord);
}

#endif

