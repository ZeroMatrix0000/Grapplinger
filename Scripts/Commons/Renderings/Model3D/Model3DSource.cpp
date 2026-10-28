/*
 * FileName:     Model3DSource.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 3Dモデルのソース
 */

#include "Pch.h"
#include "Model3DSource.h"

#include "InstancingEffectFactory.h"

// コンストラクタ
Renderings::Model3DSource::Model3DSource(Ref<const PixelShader> pixelShader)
	: m_model{}
	, m_partInfos{}
	, m_depthStencilState{}
	, m_drawingOrder{}
	, m_pixelShader{ pixelShader }
{
}

void Renderings::Model3DSource::Initialize(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef, Ref<InstancingEffectFactory> fxRef)
{
	fxRef->SetModel3DSource(Ref{ *this });
	m_model = DirectX::Model::CreateFromCMO(deviceRef.Get(), filePath.c_str(), fxRef.GetValue());
}

// パーツごとの情報を追加
void Renderings::Model3DSource::AddPartInfo
(
	const DirectX::IEffectFactory::EffectInfo& effectInfo,
	ID3D11ShaderResourceView* pTexture,
	ID3D11ShaderResourceView* pNormalMap,
	ID3D11ShaderResourceView* pSpecularMap
)
{
	auto& partInfo = m_partInfos.emplace_back();
	partInfo.effectInfo = effectInfo;
	partInfo.texture.Attach(pTexture);
	partInfo.normalMap.Attach(pNormalMap);
	partInfo.specularMap.Attach(pSpecularMap);
}

// 深度ステンシルステートを設定
void Renderings::Model3DSource::SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc, Ref<ID3D11Device5> deviceRef)
{
	Utility::ThrowIfFailed(deviceRef->CreateDepthStencilState(&desc, m_depthStencilState.ReleaseAndGetAddressOf()));
}

// 生成
Renderings::Model3DSource Renderings::Model3DSource::Create(Ref<const PixelShader> pixelShader, const std::wstring& filePath, Ref<ID3D11Device5> deviceRef, Ref<InstancingEffectFactory> fxRef)
{
	Model3DSource model{ pixelShader };
	model.Initialize(filePath, deviceRef, fxRef);
	return model;
}
