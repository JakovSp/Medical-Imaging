
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

cbuffer Slices : register(b3) {
	uint NumberOfSlices;
};

struct VertexShaderInput
{
	float2 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint InstanceID : SV_InstanceID)
{
	VertexShaderOutput output;
	float4 pos = float4(((float)InstanceID/NumberOfSlices) - 0.5f, input.pos.yx, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;

	output.tex.x = input.tex.x;
	output.tex.y = input.tex.y;
	output.tex.z = InstanceID;

	return output;
}
