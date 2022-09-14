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

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : SV_POSITION;
};

// Per-pixel color data passed through the pixel shader.
struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 pos = float4(input.pos.xyz, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;

	return output;
}
