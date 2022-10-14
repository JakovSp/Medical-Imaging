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

cbuffer VAConstant : register(b3)
{
	uint nSequence[64];
	uint v1[24];
	uint v2[24];
};

cbuffer VAPerFrame : register(b4)
{
	float4 vecVertices[8];
	float4 vecView;
	float samplingrate;
	uint frontIndex;
};

struct VertexShaderInput
{
	float2 id : POSITION;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXTURE0;
};

VertexShaderOutput main(VertexShaderInput input, uint InstanceID : SV_InstanceID){
	VertexShaderOutput output;
	float4 Position;
	float4 pos;

	uint nvSequence[64] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		1, 4, 5, 0, 3, 7, 2, 6,
		2, 6, 0, 5, 7, 3, 1, 4,
		3, 0, 6, 4, 1, 2, 7, 5,
		4, 3, 7, 1, 0, 6, 5, 2,
		5, 2, 1, 7, 6, 0, 4, 3, 
		6, 7, 3, 2, 5, 4, 0, 1,
		7, 5, 4, 6, 2, 1, 3, 0
	};

	uint nv1[24] = {
		0, 1, 4, 1,
		1, 0, 1, 4,
		0, 2, 5, 2,
		2, 0, 2, 5,
		0, 3, 6, 3,
		3, 0, 3, 6,
	};

	uint nv2[24] = {
		1, 4, 7, 5,
		5, 1, 4, 7,
		2, 5, 7, 6,
		6, 2, 5, 7,
		3, 6, 7, 4,
		4, 3, 6, 7,
	};

	float dPlane = 0.0f - InstanceID*samplingrate;
	for (int e = 0; e < 4; e++) {
		int vidx1 = nvSequence[int(frontIndex * 8 + nv1[input.id.x * 4 + e])];
		int vidx2 = nvSequence[int(frontIndex * 8 + nv2[input.id.x * 4 + e])];
		float4 vecV1 = vecVertices[vidx1];
		float4 vecV2 = vecVertices[vidx2];
		float4 vecStart = vecV1;
		float4 vecDir = vecV2 - vecV1;
		float denom = dot(vecDir, vecView);
		float lambda = (denom != 0.0) ?
			(dPlane - dot(vecStart, vecView)) / denom : -1.0f;
		if (lambda >= 0.0f && lambda <= 1.0f) {
			Position = vecStart + lambda * vecDir;
			break;
		}
	}

	Position.w = 1.0f;
	pos = Position;
	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;
	output.tex.x = 0.5 + Position.x*0.5;
	output.tex.y = 0.5 + Position.y*0.5;
	output.tex.z = 0.5 + Position.z*(1.0f/(2.0f * 0.146));


	return output;
}