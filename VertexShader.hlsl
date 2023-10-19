struct VERTEX_SHADER_INPUT
{
    float4 pos : POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

struct VERTEX_SHADER_OUTPUT
{
    float4 pos : SV_POSITION;
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

VERTEX_SHADER_OUTPUT vsmain(VERTEX_SHADER_INPUT input)
{
    VERTEX_SHADER_OUTPUT output = (VERTEX_SHADER_OUTPUT) 0;
    
    //output.pos = lerp(input.pos, input.pos1, (sin(m_angle) + 1.0f) / 2.0f);
    
    /* WORLD SPACE */
    output.pos = mul(input.pos, m_world); //mul(): matrix multiplication method
    /* VIEW SPACE */
    output.pos = mul(output.pos, m_view);
    /* PROJECTION SPACE */
    output.pos = mul(output.pos, m_projection);
    
    output.color = input.color;
    output.color1 = input.color1;
	
    return output;
}