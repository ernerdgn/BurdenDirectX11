Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 direction_to_cam : TEXCOORD1;
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
    
    float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0f - input.texcoord.y));
    
    /* ambient */
    float ka = 5.0f;  //1.5 for default maybe
    float3 ia = float3(.09f, .08f, .07f);
    ia *= (color.rgb);
    
    float3 ambient_light = ka * ia;
    
    /* diffuse */
    float kd = 0.5f;
    
    float3 diffuse_light_amount = dot(m_light_direction.xyz, input.normal);
    
    float3 id = float3(1, 1, 1);
    id *= (color.rgb);
    
    float3 diffuse_light = kd * id * diffuse_light_amount;
    
    /* specular */
    float ks = .0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
    float brightness = 10.0f;
    float specular_light_amount = pow(max(.0f, dot(reflected_light, input.direction_to_cam)), brightness);
    
    float3 specular_light = ks * specular_light_amount * is;
    
    /* phong shading */
    float3 result_light = ambient_light + diffuse_light + specular_light;
    
    
    return float4(result_light, alpha);
}