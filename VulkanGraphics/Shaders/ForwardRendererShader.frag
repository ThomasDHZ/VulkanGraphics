#version 450
#extension GL_ARB_separate_shader_objects : enable

struct DirectionalLight
{
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
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
	DirectionalLight directionalLight;
	Material material;
    vec3 viewPos;
} mesh;

void main()
{
    // ambient
    vec3 ambient = mesh.directionalLight.Ambient * texture(DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-mesh.directionalLight.Direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mesh.directionalLight.Diffuse * (diff * texture(DiffuseMap, TexCoords).rgb);
    
    // specular
    vec3 viewDir = normalize(mesh.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mesh.material.shininess);
    vec3 specular = mesh.directionalLight.Specular * spec * texture(SpecularMap, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
} 


