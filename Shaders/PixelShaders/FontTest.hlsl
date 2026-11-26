Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PSInput
{
	float4 Color : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

float Median(float r, float g, float b)
{
	return max
	(
		min(r, g),
		min(max(r, g), b)
	);
}

float4 main(PSInput input) : SV_Target
{
	float3 color = Texture.Sample(Sampler, input.TexCoord).rgb;
	
	float d = Median(color.r, color.g, color.b) - 0.5f;
	
	float alpha = clamp(d / fwidth(d) + 0.5f, 0.0f, 1.0f);
	
	return float4(1.0f, 1.0f, 1.0f, alpha);
}
