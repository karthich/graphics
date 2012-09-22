#define MAX_LIGHTS 8
#define NUM_LIGHTS 3

varying vec3 normal;
varying vec4 vVertex;
uniform float time;
uniform int light1,light2,light0;
uniform int material_change;

struct Material
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shiny;
}matl_custom[2],matls[2];

void initMaterials()
{
	matl_custom[0].emissive.wxyz = vec4(0.0,     0.0,    0.0,    1.0);
	matl_custom[0].ambient = vec4(0.02,    0.02,   0.02,   1.0);
	matl_custom[0].diffuse = vec4(0.01,    0.01,   0.01,   1.0);
	matl_custom[0].specular = vec4(0.4,     0.4,    0.4,    1.0);   
	matl_custom[0].shiny = 10.0f;

	matl_custom[1].emissive = vec4(0.0,      0.0,      0.0,      1.0);
	matl_custom[1].ambient = vec4(0.2125,   0.1275,   0.054,    1.0);
	matl_custom[1].diffuse=vec4(0.714,    0.4284,   0.18144,  1.0);
	matl_custom[1].specular = vec4(0.393548, 0.271906, 0.166721, 1.0); 
	matl_custom[1].shiny = 25.6f;
}


void main (void)
{
  
  vec3 eyeVec, lightDir[MAX_LIGHTS];

  eyeVec = -vVertex.xyz;
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
      lightDir[i] =vec3(gl_LightSource[i].position.xyz - vVertex.xyz);
  
  vec4 final_color = gl_FrontLightModelProduct.sceneColor;
  vec3 N = normalize(normal);
  if(material_change==0)
  {
	  for (i=0; i<NUM_LIGHTS; ++i)
	  {
	    if((light1==1&&i==1)||(light2==1&&i==2)||(light0==1&&i==0))
	    {
		    vec3 L = normalize(lightDir[i]);
		    float lambertTerm = dot(N,L);
		    if (lambertTerm > 0.0)
		    {
		      final_color += gl_LightSource[i].diffuse *
			gl_FrontMaterial.diffuse *
			lambertTerm;
		      vec3 E = normalize(eyeVec);
		      vec3 R = reflect(-L, N);
		      float specular = pow(max(dot(R, E), 0.0),gl_FrontMaterial.shininess);
		      final_color += gl_LightSource[i].specular *gl_FrontMaterial.specular *specular;
	//	      final_color += gl_LightSource[i].ambient;
		}
	    }
	   
	  }
  }
  else
  {
	for (i=0; i<NUM_LIGHTS; ++i)
	  {
	    if((light1==1&&i==1)||(light2==1&&i==2)||(light0==1&&i==0))
	    {
		    vec3 L = normalize(lightDir[i]);
		    float lambertTerm = dot(N,L);
		    if (lambertTerm > 0.0)
		    {
		      final_color += gl_LightSource[i].diffuse *
			gl_FrontMaterial.diffuse *
			lambertTerm;
		      vec3 E = normalize(eyeVec);
		      vec3 R = reflect(-L, N);
		      float specular = pow(max(dot(R, E), 0.0),gl_FrontMaterial.shininess);
		      final_color += gl_LightSource[i].specular *gl_FrontMaterial.specular *specular;
	//	      final_color += gl_LightSource[i].ambient;
		}
	    }
	   
	  }
   }
  
	
  gl_FragColor = final_color;
}



