Texture3D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, input.tex);
	color = float4(color.x, color.x, color.x, color.x);

	return color;
}
