SamplerState Sampler : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
};

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}