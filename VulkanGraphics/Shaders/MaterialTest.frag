#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
  
layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D texSampler2;
layout(binding = 3) uniform Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;
layout(binding = 4) uniform AmbiantLight
{
	vec3 Color;
	float Strength;
} ambiantLight;
layout(binding = 5) uniform Lighter
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;
layout(binding = 6) uniform ViewPos
{
    vec3 viewPos;
} viewPos;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
} 


