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

uniform float u_Interact;
uniform float u_Time;
uniform sampler2D u_Texture;
uniform sampler2D u_Glyph;

varying vec2 v_TexCoord;

void main()
{
  gl_FragColor = texture2D(u_Texture, v_TexCoord);
  vec4 glyphColor = texture2D(u_Glyph, v_TexCoord);

  if(glyphColor.w == 1)
  {
    //glyphColor.w = 1;
    gl_FragColor *= glyphColor;
  }
  else
  {
    float a = gl_FragColor.w;
    //gl_FragColor /= 2;
    gl_FragColor.w = a;
    glyphColor *= glyphColor.w;
    gl_FragColor.xyz += glyphColor.xyz;
    gl_FragColor = clamp(gl_FragColor, vec4(0, 0, 0, 0), vec4(1, 1, 1, 1));
  }

  float t = sin(u_Time * 5);
  //t = 1 - t;
  t /= 10;

  if(u_Interact == 0.5)
  {
    gl_FragColor *= vec4(0.6, 0.6, 0.75 + t, 1);
  }
  else if(u_Interact == 1)
  {
    gl_FragColor *= vec4(0.6, 0.6, 1.0, 1);
  }

}

#endif

