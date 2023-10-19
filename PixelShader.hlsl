struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float3 color1 : COLOR1;
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
    return float4(lerp(input.color, input.color1, (float) ((sin((float) (m_time / (float) 500.0f)) + 1.0f) / 2.0f)), alpha);
}