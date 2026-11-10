Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer ConstBuffer : register(b0)
{
	float2 TextureSize;
	float Time;
}

struct PSInput
{
	float4 Color : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_Target
{
	float2 uv = input.TexCoord;
	float4 color = Texture.Sample(Sampler, uv);
	
	return color * input.Color;
}
