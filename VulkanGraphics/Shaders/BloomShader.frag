#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1) uniform FrameBufferSettings
{
    float Gamma;
    float HDRValue;
} settings;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main() {
const float weights[] = float[](0.0024499299678342,
									0.0043538453346397,
									0.0073599963704157,
									0.0118349786570722,
									0.0181026699707781,
									0.0263392293891488,
									0.0364543006660986,
									0.0479932050577658,
									0.0601029809166942,
									0.0715974486241365,
									0.0811305381519717,
									0.0874493212267511,
									0.0896631113333857,
									0.0874493212267511,
									0.0811305381519717,
									0.0715974486241365,
									0.0601029809166942,
									0.0479932050577658,
									0.0364543006660986,
									0.0263392293891488,
									0.0181026699707781,
									0.0118349786570722,
									0.0073599963704157,
									0.0043538453346397,
									0.0024499299678342);

 vec2 tex_offset = 1.0 / textureSize(texSampler, 0); // gets size of single texel
     vec3 result = texture(texSampler, fragTexCoord).rgb * weights[0];

         for(int i = 1; i < weights.length(); ++i)
         {
            result += texture(texSampler, fragTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
            result += texture(texSampler, fragTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
         }
 
         for(int i = 1; i < weights.length(); ++i)
         {
             result += texture(texSampler, fragTexCoord + vec2(0.0, tex_offset.y * i)).rgb * weights[i];
             result += texture(texSampler, fragTexCoord - vec2(0.0, tex_offset.y * i)).rgb * weights[i];
         }

    outColor = vec4(result, 1.0f);
}