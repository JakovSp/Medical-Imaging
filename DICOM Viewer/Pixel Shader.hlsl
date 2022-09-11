
//
//	Pixel Shader.hlsl
//  Defining a pixel shader
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	// float4 color : COLOR0;
};

float4 main(PixelShaderInput input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}