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

uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main()
{
  gl_FragColor = texture2D(u_Texture, v_TexCoord);
}

#endif

