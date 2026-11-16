/*
 * FileName:     PlayerShadow.hlsl
 * Author:       Takao Hayata
 * Last Updated: 2026/10/16
 *
 * プレイヤーの丸影
 */

#include "../Header.hlsli"

// テクスチャ
Texture2D<float4> Texture : register(t0);
// サンプラー
SamplerState Sampler : register(s0);

// 環境
cbuffer Environment : register(b0)
{
	// 環境光色
	float3 AmbientColor;
	// 光の方向
	float3 LightDirection;
	// ライトの色
	float3 LightColor;
	// 目線の位置
	float3 EyePosition;
}
// マテリアル
cbuffer Material : register(b1)
{
	// 拡散反射色
	float3 DiffuseColor;
	// 放射色
	float3 EmissiveColor;
	// 鏡面反射色
	float3 SpecularColor;
	// 鏡面反射の強さ
	float SpecularPower;
}
// プレイヤー
cbuffer Shadow : register(b2)
{
	// 影の個数
	int ShadowCount;
	// 影の座標
	float3 ShadowPosition[100];
	// 影の半径
	float ShadowRadius[100];
}

float3 Random3(float3 pos)
{
	pos = float3
	(
		dot(pos, float3(12.1f, 31.7f, 24.6f)),
		dot(pos, float3(26.5f, 18.3f, 82.4f)),
		dot(pos, float3(31.3f, 20.6f, 60.3f))
	);
	return -1.0f + 2.0f * frac(sin(pos) * 48.53f);
}

float3 PerlinNoise(float3 pos)
{
	float3 p = floor(pos);
	float3 f = frac(pos);
	float3 u = f * f * (3.0 - 2.0 * f);
	float3 v000 = Random3(p + float3(0, 0, 0));
	float3 v001 = Random3(p + float3(0, 0, 1));
	float3 v010 = Random3(p + float3(0, 1, 0));
	float3 v011 = Random3(p + float3(0, 1, 1));
	float3 v100 = Random3(p + float3(1, 0, 0));
	float3 v101 = Random3(p + float3(1, 0, 1));
	float3 v110 = Random3(p + float3(1, 1, 0));
	float3 v111 = Random3(p + float3(1, 1, 1));
	return lerp
	(
		lerp
		(
			lerp(cross(v000, f - float3(0, 0, 0)), cross(v001, f - float3(0, 0, 1)), u.z),
			lerp(cross(v010, f - float3(0, 1, 0)), cross(v011, f - float3(0, 1, 1)), u.z),
			u.y
		),
		lerp
		(
			lerp(cross(v100, f - float3(1, 0, 0)), cross(v101, f - float3(1, 0, 1)), u.z),
			lerp(cross(v110, f - float3(1, 1, 0)), cross(v111, f - float3(1, 1, 1)), u.z),
			u.y
		),
		u.x
	);
}

float Shadow(float3 worldPosition, float3 normal)
{
	// 影の濃さ
	float shadow = 0.0f;
	
	for (int i = 0; i < ShadowCount; i++)
	{
		// プレイヤーとの距離
		float3 delta = worldPosition - ShadowPosition[i];
		// 縦に伸ばす
		delta *= float3(1.0f, 0.1f, 1.0f);
		
		shadow = max(shadow, step(dot(delta, delta), ShadowRadius[i] * ShadowRadius[i]) * step(0.1f, dot(normal, float3(0.0f, 1.0f, 0.0f))) * step(delta.y, ShadowRadius[i] * 0.2f) * 0.4f);
	}

	return shadow;
}

float4 main(PSInput input) : SV_Target
{
	// テクスチャ色
	float4 textureColor = Texture.Sample(Sampler, input.texCoord);
	
	// 法線
	float3 normal = normalize(input.normal);
	
	float3 p = PerlinNoise(input.worldPosition * 4.0f);
	normal = normalize(p / 2.0f + normal);
	
	// 環境光色
	float3 ambientColor = textureColor.rgb * AmbientColor * DiffuseColor * input.color.rgb;
	
	// 光の方向
	float3 lightDirection = normalize(-LightDirection);
	// 拡散反射の強さ
	float diffuse = saturate(dot(normal, lightDirection));
	// 拡散反射色
	float3 diffuseColor = textureColor.rgb * DiffuseColor * LightColor * diffuse * input.color.rgb;
	
	// 目線への方向
	float3 eyeDirection = normalize(EyePosition - input.worldPosition);
	// 反射方向
	float3 halfVector = normalize(lightDirection + eyeDirection);
	
	// 鏡面反射
	float specular = pow(saturate(dot(normal, halfVector)), SpecularPower);
	// 鏡面反射色
	float3 specularColor = SpecularColor * LightColor * specular;
	
	// 描画色
	float3 color = ambientColor + diffuseColor + specularColor + EmissiveColor;
	
	// 影の濃さ
	float shadow = Shadow(input.worldPosition, normal);
	
	return float4(color * (1.0f - shadow), textureColor.a);
}