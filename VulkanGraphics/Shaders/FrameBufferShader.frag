#version 450

struct Light
{
	vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;
layout (input_attachment_index = 3, binding = 3) uniform subpassInput inputDepth;
layout(binding = 4) uniform LightingStruct 
{
	Light light;
	vec3 viewPos;
	float shininess;
} Lighter;
layout(binding = 5) uniform DebugStruct 
{
	int DebugLayer;
} Debug;

layout (location = 0) out vec4 outColor;

void main() 
{
	vec3 lighting = vec3(0.0f);

		vec3 FragPos = subpassLoad(inputPosition).rgb;
		vec3 Normal = subpassLoad(inputNormal).rgb;
		vec3 Diffuse = subpassLoad(inputAlbedo).rgb;
		float Specular = subpassLoad(inputAlbedo).a;

	if(Debug.DebugLayer == 0)
	{
		lighting = FragPos;
	}
	else if(Debug.DebugLayer == 1)
	{
		lighting = Normal;
	}
	else if(Debug.DebugLayer == 2)
	{
		lighting = Diffuse;
	}
	else
	{
		 lighting  = Diffuse * 0.1; 
		vec3 viewDir  = normalize(Lighter.viewPos - FragPos);
	
			// calculate distance between light source and current fragment
			float distance = length(Lighter.light.Position - FragPos);
			if(distance < Lighter.light.Radius)
			{
				// diffuse
				vec3 lightDir = normalize(Lighter.light.Position - FragPos);
				vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * Lighter.light.Color;
				// specular
				vec3 halfwayDir = normalize(lightDir + viewDir);  
				float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
				vec3 specular = Lighter.light.Color * spec * Specular;
				// attenuation
				float attenuation = 1.0 / (1.0 + Lighter.light.Linear * distance + Lighter.light.Quadratic * distance * distance);
				diffuse *= attenuation;
				specular *= attenuation;
				lighting += diffuse + specular;
			}
	}
	outColor = vec4(lighting, 1.0f);
}