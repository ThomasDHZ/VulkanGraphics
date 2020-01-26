  #version 450
#extension GL_ARB_separate_shader_objects : enable

struct Light
{
	vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform LightingStruct 
{
	Light light;
	vec3 viewPos;
	float shininess;
} Lighter;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

layout(location = 0) out vec4 FragColor;

void main() 
{
    // ambient
    vec3 ambient = Lighter.light.Ambient * texture(DiffuseMap, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(Lighter.light.Position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Lighter.light.Diffuse * diff * texture(DiffuseMap, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(Lighter.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Lighter.shininess);
    vec3 specular = Lighter.light.Specular * spec * texture(SpecularMap, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
