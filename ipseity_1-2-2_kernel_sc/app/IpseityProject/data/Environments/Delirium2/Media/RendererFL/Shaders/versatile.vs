#version 140


uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NM;
uniform vec3 Color;

in vec3 position;
in vec3 normal;
in vec2 texcoords;

noperspective out vec3 fMVPos;
smooth out vec3 fNormal;
smooth out vec2 fTexCoords;
smooth out vec3 fColor;


void main()
{
  vec4 p = vec4(position, 1.0f);

  gl_Position = MVP*p;

  fMVPos = vec3(MV*p);
  fNormal = NM*normal;
  fTexCoords = texcoords;
  fColor = Color;
}
