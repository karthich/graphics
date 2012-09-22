varying vec3 normal;
varying vec4 vVertex;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 3

uniform struct Lights
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position; 
    bool isOn;
}myLights[MAX_LIGHTS];




void main (void)
{
  vec3 eyeVec;
  vec3 lightDir[MAX_LIGHTS];

  eyeVec = -vVertex.xyz;
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
    lightDir[i] =
      vec3(gl_LightSource[i].position.xyz - vVertex.xyz);

  vec4 final_color =
       gl_FrontLightModelProduct.sceneColor;
  vec3 N = normalize(normal);
  
  for (i=0; i<NUM_LIGHTS; ++i)
  {
    vec3 L = normalize(lightDir[i]);
    float lambertTerm = dot(N,L);
    if (lambertTerm > 0.0)
    {
      final_color +=
        gl_LightSource[i].diffuse *
        gl_FrontMaterial.diffuse *
        lambertTerm;
      vec3 E = normalize(eyeVec);
      vec3 R = reflect(-L, N);
      float specular = pow(max(dot(R, E), 0.0),
                           gl_FrontMaterial.shininess);
      final_color +=
        gl_LightSource[i].specular *
        gl_FrontMaterial.specular *
        specular;
    }
  }
  gl_FragColor = final_color;
}
