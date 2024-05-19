Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 world_position : TEXCOORD1;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    float4 m_light_direction;
    float4 m_cam_position;
    float4 m_light_position;
    float m_light_radius;
    float m_time;
};

float4 psmain(PIXEL_SHADER_INPUT input) : SV_TARGET
{
    float alpha = 1.0f;
    
    float4 texture_color = Texture.Sample(TextureSampler, float2(input.texcoord.x, 1.0f - input.texcoord.y));
    
    /* ambient */
    //float ka = 1.5f;
    float ka = 3.0f;
    float3 ia = float3(.09f, .08f, .07f);
    ia *= (texture_color.rgb);
    
    float3 ambient_light = ka * ia;
    
    /* diffuse */
    float kd = 0.5f;
    float3 id = float3(1.0f, 1.0f, 1.0f);
    
    id *= (texture_color.rgb);
    
    float3 light_direction = normalize(m_light_position.xyz - input.world_position.xyz);
    
    float distance_from_light_to_obj = length(m_light_position.xyz - input.world_position.xyz);
    float fade_area = max(0, distance_from_light_to_obj - m_light_radius);
    
    //calculating the attenuation
    float constant_ = 1.0f;
    float linear_ = 1.0f;
    float quadratic_ = 1.0f;
                        //const           linear                   quadratic
    float attenuation = constant_ + (fade_area * linear_) + (fade_area * fade_area * quadratic_);
    
    float3 diffuse_light_amount = max(0, dot(light_direction.xyz, input.normal));
    
    float3 diffuse_light = (kd * id * diffuse_light_amount) / attenuation;
    
    /* specular */
    float ks = 0.0;  //can be changed to 0 to eliminate
    float3 is = float3(1.0f, 1.0f, 1.0f);
    
    float3 direction_to_cam = normalize(input.world_position.xyz - m_cam_position.xyz);
    
    float3 reflected_light = reflect(light_direction.xyz, input.normal);
    float brightness = 50.0f;
    float specular_light_amount = pow(max(.0f, dot(reflected_light, direction_to_cam)), brightness);
    
    float3 specular_light = (ks * specular_light_amount * is) / attenuation;
    
    /* phong shading */
    float3 result_light = ambient_light + diffuse_light + specular_light;
    
    
    return float4(result_light, alpha);
}