#version 140


uniform float Alpha;
uniform bool UseTex;
uniform sampler2D TexId;
uniform vec3 LightPos;
uniform vec3 LightDiffuse;
uniform float MinDistance;

noperspective in vec3 fMVPos; // linear interpolation
smooth in vec3 fNormal;
smooth in vec2 fTexCoords;
smooth in vec3 fColor;

out vec4 fragColor;

const float cstOutsideAttenuation = 0.6f;


void main()
{
  fragColor = vec4( fColor, Alpha );

  if (UseTex)
  {
    fragColor.rgb *= texture2D( TexId, fTexCoords ).rgb;
  }

  float ct;
  vec3 vertex_pos = fMVPos;
  vec3 light_dir = LightPos-vertex_pos;

  ct = max( dot(normalize(fNormal),normalize(light_dir)), 0 );

  float d = length( light_dir );
  if (d >= MinDistance)
  {
    ct *= cstOutsideAttenuation*(MinDistance/d);
  }
  else
  {
    fragColor.rgb *= LightDiffuse.rgb;
  }
  fragColor.rgb *= ct;
}
