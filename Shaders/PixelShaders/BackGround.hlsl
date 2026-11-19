/*
 * FileName:     BackGround.hlsl
 * Author:       Takao Hayata
 * Last Updated: 2026/11/19
 *
 * 背景のデバッグ描画
 */

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer ConstBuffer : register(b0)
{
	float4 Quaternion;
	float2 TextureSize;
}

struct PSInput
{
	float4 Color : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

float3 rotate(float3 v, float4 q)
{
	float3 qv = q.xyz;
	float3 c = cross(qv, v);
	return v + 2.0f * (cross(qv, c) + q.w * c);
}

float4 main(PSInput input) : SV_Target
{	
	float2 uv = input.TexCoord - 0.5f;
	uv.x *= TextureSize.x / TextureSize.y;
	uv.y *= -1.0f;
	uv *= 0.7f;
	
	float3 pos = normalize(float3(uv, -1.0f));
	pos = rotate(pos, Quaternion);
	
	float y = pos.y / 2.0f + 0.5f;
	
	float3 darkColor = lerp(float3(0.0f, 0.0f, 0.0f), float3(0.2f, 0.4f, 1.0f), y * 4.0f);
	float3 lightColor = lerp(float3(0.2f, 0.4f, 1.0f), float3(0.5f, 0.7f, 1.0f), (y * 4.0f - 1.0f) / 3.0f);
	
	float3 color = lerp(darkColor, lightColor, step(0.25f, y));
	
	return float4(color, 1.0f);
}
