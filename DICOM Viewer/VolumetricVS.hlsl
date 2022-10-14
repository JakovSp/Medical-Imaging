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

cbuffer SamplingMatrix : register(b3)
{
	matrix samplingmatrix;
};

struct VertexShaderInput
{
	float4 pos : SV_POSITION;
	float4 tex : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint InstanceID : SV_InstanceID)
{
	VertexShaderOutput output;
	float4 pos = float4(input.pos.xyz, input.pos.w*InstanceID);
	pos = mul(pos, samplingmatrix);
	pos.w = 1.0f;

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;

	float4 tex = float4(input.tex.xyz, input.tex.w * InstanceID);
	tex = mul(tex, samplingmatrix);
	output.tex = tex.xyz;
	// output.tex.x = 1.0f/150 * InstanceID; // input.tex.x* InstanceID;
	// output.tex.y = input.tex.y;
	// output.tex.z = input.tex.z;

	return output;
}
