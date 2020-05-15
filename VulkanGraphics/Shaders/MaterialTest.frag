#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
  
layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D texSampler2;
layout(binding = 3) uniform Material
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Shininess;
} material;
layout(binding = 4) uniform AmbiantLight
{
	vec3 Color;
	float Strength;
} ambiantLight;
layout(binding = 5) uniform Lighter
{
	vec3 lightPos;
	vec3 viewPos;
	vec3 lightColor;
	vec3 objectColor;
} light;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(light.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * light.objectColor;
    outColor = vec4(result, 1.0);
} 


