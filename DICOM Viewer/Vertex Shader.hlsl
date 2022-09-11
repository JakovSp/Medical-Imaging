
//
//	Vertex Shader.hlsl
//  Defining a vertex shader
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

struct VertexShaderInput
{
	float3 pos : SV_Position;
// 	vector<float, 4> color : COLOR;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
//	float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 pos = float4(input.pos, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;

	//output.color = input.color;

	return output;
}