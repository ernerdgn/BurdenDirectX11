Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PIXEL_SHADER_INPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 direction_to_cam : TEXCOORD1;
};

float4 psmain(PIXEL_SHADER_INPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, 1.0f - input.texcoord);  // (1.0f -) for the inverting the skybox texture
}