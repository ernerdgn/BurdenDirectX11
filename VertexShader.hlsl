struct VERTEX_SHADER_INPUT
{
    float4 pos : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VERTEX_SHADER_OUTPUT
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
    float4 m_cam_position;
};

VERTEX_SHADER_OUTPUT vsmain(VERTEX_SHADER_INPUT input)
{
    VERTEX_SHADER_OUTPUT output = (VERTEX_SHADER_OUTPUT) 0;
    
    //output.pos = lerp(input.pos, input.pos1, (sin(m_angle) + 1.0f) / 2.0f);
    
    /* WORLD SPACE */
    output.pos = mul(input.pos, m_world); //mul(): matrix multiplication method
    output.direction_to_cam = normalize(output.pos.xyz - m_cam_position.xyz);
    /* VIEW SPACE */
    output.pos = mul(output.pos, m_view);
    /* PROJECTION SPACE */
    output.pos = mul(output.pos, m_projection);
    
    output.texcoord = input.texcoord;
    output.normal = input.normal;
	
    return output;
}