
//
//	Vertex Shader.hlsl
//  Defining a sample vertex shader
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

cbuffer WorldTransforms : register(b0)
{
	matrix World;
	matrix Dlrow;
};

cbuffer ViewTransforms : register(b1)
{
	matrix View;
};

cbuffer ProjectionTransforms : register(b2)
{
	matrix Projection;
};

cbuffer TextureBuffer : register(b3)
{
	float vertZ;
	float texZ;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

// Per-pixel color data passed through the pixel shader.
struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 pos = float4(input.pos.xy, vertZ, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;

	output.tex = input.tex;
	output.tex.z = texZ;

	return output;
}
