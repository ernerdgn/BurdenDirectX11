Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_Position;
    float2 texcoord : TEXCOORD0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    
    unsigned int m_time;
};

float4 psmain(PIXEL_SHADER_INPUT input) : SV_TARGET
{
    //float r = 1.0f, g = 1.0f, b = 1.0f, alpha = 1.0f;
    float alpha = 1.0f;
    return Texture.Sample(TextureSampler, input.texcoord * 0.5f);
}