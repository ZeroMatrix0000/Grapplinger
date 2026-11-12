/*
 * FileName:     GroundDrid.hlsl
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * グリッド床
 */

#include "../Header.hlsli"

// 定数バッファ
cbuffer Camera : register(b0)
{
	// ビュー行列
	float4x4 View;
	// プロジェクション行列
	float4x4 Projection;
}

VSOutput main(VSInput input)
{
	VSOutput output;
	
	// ワールド座標
	float3 worldPosition = mul(float4(input.position, 1.0f), input.world);
	
	// 行列を適用
	output.position = float4(worldPosition, 1.0f);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	
	output.texCoord = input.texCoord;
	output.worldPosition = worldPosition;
	output.normal = mul(input.normal, (float3x3) input.world);
	
	float b = frac(worldPosition.x);
	
	output.color = input.color;
	output.color.rgb *= (1.0f - b);
	
	return output;
}