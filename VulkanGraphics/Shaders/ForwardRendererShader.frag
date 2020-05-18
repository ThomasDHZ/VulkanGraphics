#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Lighter
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
  
layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform MeshProp
{
	Lighter light;
	Material material;
    vec3 viewPos;
} mesh;

void main()
{
    // ambient
    vec3 ambient = mesh.light.ambient * texture(DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(mesh.light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mesh.light.diffuse * (diff * texture(DiffuseMap, TexCoords).rgb);
    
    // specular
    vec3 viewDir = normalize(mesh.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mesh.material.shininess);
    vec3 specular = mesh.light.specular * spec * texture(SpecularMap, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
} 


