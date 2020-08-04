#version 450
#extension GL_ARB_separate_shader_objects : enable


const int UseDiffuseMapBit    = 0x00000001;
const int UseSpecularMapBit   = 0x00000002;
const int UseNormalMapBit     = 0x00000004;
const int UseDepthMapBit      = 0x00000008;
const int UseAlphaMapBit      = 0x00000016;
const int UseEmissionMapBit   = 0x00000032;
const int UseSkyBoxBit        = 0x00000064;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D depthMap;
layout(binding = 5) uniform sampler2D AlphaMap;
layout(binding = 6) uniform sampler2D EmissionMap;
layout(binding = 7) uniform samplerCube SkyBox;
layout(binding = 8) uniform MeshProperties
{
    Material material;
    float minLayers;
    float maxLayers;
    float heightScale;
    int MappingBitFlags;
} meshProperties;
layout(binding = 9) uniform Light
{
    vec3 lightPos;
    vec3 viewPos;
} light;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in mat3 TBN;


layout(location = 0) out vec4 FragColor;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    float numLayers = mix(meshProperties.maxLayers, meshProperties.minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * meshProperties.heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{           
    vec3 TangentLightPos = TBN * light.lightPos;
    vec3 TangentViewPos  = TBN * light.viewPos;
    vec3 TangentFragPos  = TBN * FragPos;

    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = TexCoords;
    
    texCoords = ParallaxMapping(TexCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // get diffuse color
    vec3 color = texture(DiffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), meshProperties.material.shininess);

    vec3 specular = vec3(0.2) * spec;
    
    
    vec3 I = normalize(FragPos - light.viewPos);
    vec3 R = reflect(I, normalize(normal));

    if(meshProperties.MappingBitFlags == UseSpecularMapBit)
    {
     FragColor = vec4(texture(SkyBox, R).rgb, 1.0);
    }
    else
    {
     FragColor = vec4(ambient + diffuse + specular, 1.0);
    }

    //FragColor = vec4(ambient + diffuse + specular, 1.0);
}