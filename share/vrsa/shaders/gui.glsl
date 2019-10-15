#ifdef VERTEX

attribute vec3 a_Position;
attribute vec2 a_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_Model;

varying vec2 v_TexCoord;

void main()
{
  gl_Position = u_Projection * u_Model * vec4(a_Position, 1);
  v_TexCoord = a_TexCoord;
}

#endif

#ifdef FRAGMENT

varying vec2 v_TexCoord;

void main()
{
  gl_FragColor = vec4(v_TexCoord, 0, 1);
}

#endif

