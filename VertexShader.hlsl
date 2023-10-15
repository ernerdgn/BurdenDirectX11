struct VERTEX_SHADER_INPUT
{
    float4 pos : POSITION;
    float4 pos1 : POSITION1;
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
    float m_angle;
};

VERTEX_SHADER_OUTPUT vsmain(VERTEX_SHADER_INPUT input)
{
    //if (pos.y > 0 && pos.y < 1)
    //    pos.x += .25f;
    
    //if (pos.y > 0 && pos.y < 1 && pos.x > -1 && pos.x < 0)
    //    pos.y -= .25f;
    VERTEX_SHADER_OUTPUT output = (VERTEX_SHADER_OUTPUT) 0;
    
    output.pos = lerp(input.pos, input.pos1, (sin(m_angle) + 1.0f) / 2.0f);
    output.color = input.color;
    output.color1 = input.color1;
	
    return output;
}