struct VERTEX_SHADER_INPUT
{
    float4 pos : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct VERTEX_SHADER_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VERTEX_SHADER_OUTPUT vsmain(VERTEX_SHADER_INPUT input)
{
    VERTEX_SHADER_OUTPUT output = (VERTEX_SHADER_OUTPUT) 0;
	
    return output;
}