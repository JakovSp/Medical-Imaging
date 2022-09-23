
//
//	Pixel Shader.hlsl
//  Defining a sample pixel shader for texturing
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

Texture2DArray TextureArray : register(t0);
SamplerState Sampler : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 color = TextureArray.Sample(Sampler, input.tex.xyz);
	color = float4(color.x, color.x, color.x, 1.0f - color.x);

	return color;
}
