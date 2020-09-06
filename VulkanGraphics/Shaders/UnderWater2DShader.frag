#version 450
#extension GL_ARB_separate_shader_objects : enable

struct MapBits
{
     int UseDiffuseMapBit;
     int UseSpecularMapBit;
     int UseNormalMapBit;
     int UseDepthMapBit;
     int UseAlphaMapBit;
     int UseEmissionMapBit;
     int UseSkyBoxBit;
};

struct Material {
    vec3 ambient;
    vec3 Diffuse;
    vec3 specular;    
    float shininess;
    float reflectivness;
}; 

struct DirectionalLightStruct {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLightStruct {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLightStruct {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D AlphaMap;
layout(binding = 5) uniform sampler2D EmissionMap;
layout(binding = 6) uniform MeshProperties
{
    Material material;
   // MapBits mapBitsFlags;
   vec2 UVOffset;
        int UseDiffuseMapBit;
     int UseSpecularMapBit;
     int UseNormalMapBit;
     int UseDepthMapBit;
     int UseAlphaMapBit;
     int UseEmissionMapBit;
     int UseReflectionMapBit;
     int UseSkyBoxBit;
    float minLayers;
    float maxLayers;
    float heightScale;
    float timer;
} meshProperties;
layout(binding = 7) uniform Light
{
    DirectionalLightStruct dLight;
    PointLightStruct pLight;
    SpotLightStruct sLight;
    vec3 viewPos;
} light;
layout(binding = 8) uniform WaveProperties
{
    vec3 WaterColor;
    vec2 Distortion_scale;
    float intensity;
    float speed;
    float waveAmp;
    float waveSpeed;
    float wavePeriod;
} wavesProp;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 Normal;
layout(location = 3) in mat3 TBN;

layout(location = 0) out vec4 FragColor;

void RemoveAlphaPixels(vec2 UV)
{
    if((textureSize(AlphaMap, 0).x > 1 &&
        texture(AlphaMap, UV).r == 0) ||
        texture(DiffuseMap, UV).a == 0)
   {
        discard;
   }
}

vec3 DirectionalLight(vec3 V, vec3 N, vec2 UV, DirectionalLightStruct directionalLight)
{
    vec3 L = normalize(-(TBN * directionalLight.direction));
    vec3 R = reflect(-L, N);
    vec3 H = normalize(L + V);  

    vec3 color = texture(DiffuseMap, UV).rgb;
   	vec3 ambient = directionalLight.ambient * color;
	vec3 diffuse = directionalLight.diffuse * (max(dot(L, N), 0.0) * color);
    vec3 specular = vec3(1.0f, 1.0f, 1.0f);
    if(meshProperties.UseSpecularMapBit  == 1)
    {
        specular = directionalLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * texture(SpecularMap, UV).rgb;
    }
    else
    {
        specular = directionalLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * meshProperties.material.specular;  
    }

    return (ambient + diffuse + specular);
}

vec3 PointLight(vec3 TangentLightPos, vec3 TangentFragPos, vec3 V, vec3 N, vec2 UV, PointLightStruct pointLight)
{
        float distance = length(TangentLightPos - TangentFragPos);
        float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));    
  
		vec3 L = normalize(TangentLightPos - TangentFragPos);
		vec3 R = reflect(-L, N);
		vec3 H = normalize(L + V);  

        vec3 color = texture(DiffuseMap, UV).rgb;
   		vec3 ambient = pointLight.ambient * color;
		vec3 diffuse = pointLight.diffuse * (max(dot(L, N), 0.0) * color);
        vec3 specular = vec3(1.0f, 1.0f, 1.0f);
        if(meshProperties.UseSpecularMapBit  == 1)
        {
            specular = pointLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * texture(SpecularMap, UV).rgb;
        }
        else
        {
            specular = pointLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * meshProperties.material.specular;  
        }

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        return (ambient + diffuse + specular);
}

vec3 SpotLight(vec3 TangentLightPos, vec3 TangentFragPos, vec3 V, vec3 N, vec2 UV, SpotLightStruct spotLight)
{
        float distance = length(TangentLightPos - TangentFragPos);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));    

		vec3 L = normalize(TangentLightPos - TangentFragPos);
		vec3 R = reflect(-L, N);
		vec3 H = normalize(L + V);  

        vec3 TangentDirection = TBN * spotLight.direction;
        float theta = dot(L, normalize(-TangentDirection)); 
        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        vec3 color = texture(DiffuseMap, UV).rgb;
   		vec3 ambient = spotLight.ambient * color;
		vec3 diffuse = spotLight.diffuse * (max(dot(L, N), 0.0) * color);
        vec3 specular = vec3(1.0f, 1.0f, 1.0f);
        if(meshProperties.UseSpecularMapBit  == 1)
        {
            specular = spotLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * texture(SpecularMap, UV).rgb;
        }
        else
        {
            specular = spotLight.specular * pow(max(dot(N, H), 0.0), meshProperties.material.shininess) * meshProperties.material.specular;  
        }

        ambient *= attenuation * intensity;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
}


float rand(vec2 coord){
	return fract(sin(dot(coord, vec2(12.9898, 78.233)))* 43758.5453123);
}

float noise(vec2 coord){
	vec2 i = floor(coord);
	vec2 f = fract(coord);

	// 4 corners of a rectangle surrounding our point
	float a = rand(i);
	float b = rand(i + vec2(1.0, 0.0));
	float c = rand(i + vec2(0.0, 1.0));
	float d = rand(i + vec2(1.0, 1.0));

	vec2 cubic = f * f * (3.0 - 2.0 * f);

	return mix(a, b, cubic.x) + (c - a) * cubic.y * (1.0 - cubic.x) + (d - b) * cubic.x * cubic.y;
}

void main()
{   
    vec2 UV = TexCoords;
    UV.x = 1.0f - UV.x;
    UV.y = 1.0f - UV.y;

    vec2 NoiseUV = UV * 8.0f;
    vec2 NoiceUV2 = UV * 8.0f + 4.0f;

    vec2 Motion1 = vec2(meshProperties.timer * 0.3, meshProperties.timer * -0.4f);
    vec2 Motion2 = vec2(meshProperties.timer * 0.1, meshProperties.timer * 0.5f);

    vec2 distort1 = vec2(noise(NoiseUV + Motion1), noise(NoiceUV2 + Motion2)) - vec2(0.5f);
    vec2 distort2 = vec2(noise(NoiseUV + Motion2), noise(NoiceUV2  + Motion1)) - vec2(0.5f);
    vec2 distort = (distort1 + distort2) / 60.0f;

    UV += distort;

    RemoveAlphaPixels(UV);

    vec3 V = light.viewPos;
    vec3 N = Normal;

    vec3 TangentLightDirection = TBN * light.dLight.direction;
    vec3 TangentLightPos = TBN * light.pLight.position;
    vec3 TangentViewPos  = TBN * light.viewPos;
    vec3 TangentFragPos  = TBN * FragPos;

    if (meshProperties.UseNormalMapBit  == 1)
    {
        N = texture(normalMap, UV).rgb;
        N = normalize(N * 2.0 - 1.0);   
   }

   vec3 result = DirectionalLight( V,  N,  UV, light.dLight);
   result += PointLight( TangentLightPos,  TangentFragPos,  V,  N,  UV, light.pLight);
   result = mix(result, wavesProp.WaterColor, 0.3f);
   result = mix(vec3(0.5f), result, 1.4f);
   FragColor = vec4(result, 1.00f);
}