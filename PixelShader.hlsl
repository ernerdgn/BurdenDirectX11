Texture2D WorldMorning : register(t0);
sampler WorldMorningSampler : register(s0);

Texture2D WorldSpecular : register(t1);
sampler WorldSpecularSampler : register(s1);

Texture2D WorldClouds : register(t2);
sampler WorldCloudsSampler : register(s2);

Texture2D WorldNight : register(t3);
sampler WorldNightSampler : register(s3);

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
    float m_time;
};

float4 psmain(PIXEL_SHADER_INPUT input) : SV_TARGET
{
    float alpha = 1.0f;
    
    float4 world_morning = WorldMorning.Sample(WorldMorningSampler, 1.0f - input.texcoord);
    float world_specular = WorldSpecular.Sample(WorldSpecularSampler, 1.0f - input.texcoord).r;
    float world_clouds = WorldClouds.Sample(WorldCloudsSampler, 1.0f - input.texcoord + float2(m_time / 50.0f, 0)).r;
    float4 world_night = WorldNight.Sample(WorldNightSampler, 1.0f - input.texcoord);
    
    /* ambient */
    float ka = 1.5f;
    float3 ia = float3(.09f, .08f, .07f);
    ia *= (world_morning.rgb);
    
    float3 ambient_light = ka * ia;
    
    /* diffuse */
    float kd = 0.5f;
    //float3 id = float3(1.0f, 1.0f, 1.0f);
    float3 id_morning = float3(1.0f, 1.0f, 1.0f);
    id_morning *= (world_morning.rgb + world_clouds);
    
    float3 id_night = float3(1.0f, 1.0f, 1.0f);
    id_night *= (world_night.rgb + world_clouds * .2f);
    
    float3 diffuse_light_amount = dot(m_light_direction.xyz, input.normal);
    
    float3 id = lerp(id_night, id_morning, (diffuse_light_amount + 1.0f) / 2.0f);
    
    float3 diffuse_light = kd * id;
    
    /* specular */
    float ks = world_specular;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
    float brightness = 10.0f;
    float specular_light_amount = pow(max(.0f, dot(reflected_light, input.direction_to_cam)), brightness);
    
    float3 specular_light = ks * specular_light_amount * is;
    
    /* phong shading */
    float3 result_light = ambient_light + diffuse_light + specular_light;
    
    
    return float4(result_light, alpha);
}