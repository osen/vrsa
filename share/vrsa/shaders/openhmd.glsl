#ifdef VERTEX
attribute vec2 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_Model;

varying vec2 v_TexCoords;

void main(void)
{
  v_TexCoords = a_Position;
  gl_Position = u_Projection * u_Model * vec4(a_Position, 0.0, 1.0);
}

#endif
#ifdef FRAGMENT

uniform sampler2D u_Texture;
uniform vec2 u_LensCenter;
uniform vec2 u_ViewportScale;
uniform float u_WarpScale;
uniform vec4 u_HmdWarpParam;
uniform vec3 u_Aberr;

varying vec2 v_TexCoords;

void main()
{
  vec2 output_loc = vec2(v_TexCoords.s, 1.0 - v_TexCoords.t);
  vec2 r = output_loc * u_ViewportScale - u_LensCenter;
  r /= u_WarpScale;

  float r_mag = length(r);

  vec2 r_displaced = r * (u_HmdWarpParam.w + u_HmdWarpParam.z * r_mag +
    u_HmdWarpParam.y * r_mag * r_mag +
    u_HmdWarpParam.x * r_mag * r_mag * r_mag);

  r_displaced *= u_WarpScale;

  vec2 tc_r = (u_LensCenter + u_Aberr.r * r_displaced) / u_ViewportScale;
  vec2 tc_g = (u_LensCenter + u_Aberr.g * r_displaced) / u_ViewportScale;
  vec2 tc_b = (u_LensCenter + u_Aberr.b * r_displaced) / u_ViewportScale;

  float red = texture2D(u_Texture, tc_r).r;
  float green = texture2D(u_Texture, tc_g).g;
  float blue = texture2D(u_Texture, tc_b).b;

  gl_FragColor = ((tc_g.x < 0.0) || (tc_g.x > 1.0) ||
    (tc_g.y < 0.0) || (tc_g.y > 1.0)) ?
    vec4(0.0, 0.0, 0.0, 1.0) : vec4(red, green, blue, 1.0);
}

#endif
