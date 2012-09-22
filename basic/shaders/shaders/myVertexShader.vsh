
#define MAX_LIGHTS 8
#define NUM_LIGHTS 3
varying vec3 lightDir[MAX_LIGHTS];
varying vec3 normal;
varying vec4 vVertex;
uniform float time;
uniform float xs, zs;
uniform bool waveIt;

void main()
{
  //wave shader
  vec4 vpos = vec4(gl_Vertex);
  if(waveIt)
  {
	  float s;
	  s = 1.0 + 0.3*noise1(xs*time)*noise1(zs*time);
	  	  
	  vpos.z = 2.5*noise1(xs*time + 0.5*vpos.x) + vpos.z;
	  vpos.y = 0.5*sin(xs*time + 50.0*vpos.x) + vpos.y;
	  vpos.x = 10.0*noise1(zs*time - 3.0*vpos.x) +vpos.x;
  }
  gl_Position = gl_ModelViewProjectionMatrix*vpos;
  
  gl_FrontColor = gl_Color;
  normal = gl_NormalMatrix * gl_Normal;



  vVertex = gl_ModelViewMatrix * gl_Vertex;


}
