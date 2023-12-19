Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 direction_to_cam : TEXCOORD2;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    
    float4 m_light_direction;
};

float4 psmain(PIXEL_SHADER_INPUT input) : SV_TARGET
{
    float alpha = 1.0f;
    
    /* ambient */
    float ka = .1f;
    float3 ia = float3(1.0f, 1.0f, 1.0f);
    
    float3 ambient_light = ka * ia;
    
    /* diffuse */
    float kd = 0.5f;
    float3 id = float3(1.0f, 1.0f, 1.0f);
    
    float3 diffuse_light_amount = max(.0f, dot(m_light_direction.xyz, input.normal));
    
    float3 diffuse_light = kd * diffuse_light_amount * id;
    
    /* specular */
    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
    float brightness = 10.0f;
    float specular_light_amount = pow(max(.0f, dot(reflected_light, input.direction_to_cam)), brightness);
    
    float3 specular_light = ks * specular_light_amount * is;
    
    /* phong shading */
    float3 result_light = ambient_light + diffuse_light + specular_light;
    
    
    return float4(result_light, 1.0f);
}