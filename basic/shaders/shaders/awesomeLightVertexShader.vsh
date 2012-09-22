varying vec3 normal;
varying vec4 vVertex;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 3
varying vec3 lightDir[MAX_LIGHTS];

void main()
{
  gl_Position = ftransform();
  normal = gl_NormalMatrix * gl_Normal;
  vVertex = gl_ModelViewMatrix * gl_Vertex;


}
