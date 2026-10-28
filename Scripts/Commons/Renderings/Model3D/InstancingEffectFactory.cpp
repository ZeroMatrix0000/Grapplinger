/*
 * FileName:     InstancingEffectFactory.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * インスタンス描画用のエフェクトファクトリ
 */

#include "Pch.h"
#include "InstancingEffectFactory.h"

#include "Model3DSource.h"

// コンストラクタ
Renderings::InstancingEffectFactory::InstancingEffectFactory(Ref<ID3D11Device5> deviceRef)
	: m_directory{}
	, m_pModelSource{}
	, m_deviceRef{ deviceRef }
{
}

// エフェクトを作成
std::shared_ptr<DirectX::IEffect> Renderings::InstancingEffectFactory::CreateEffect(const EffectInfo& effectInfo, ID3D11DeviceContext* pContext)
{
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	if (effectInfo.diffuseTexture && !std::wstring{ effectInfo.diffuseTexture }.empty())
	{
		CreateTexture
		(
			effectInfo.diffuseTexture,
			pContext,
			texture.GetAddressOf()
		);
	}
	// 法線マップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;
	if (effectInfo.normalTexture && !std::wstring{ effectInfo.normalTexture }.empty())
	{
		CreateTexture
		(
			effectInfo.normalTexture,
			pContext,
			normalMap.GetAddressOf()
		);
	}
	// 鏡面反射マップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularMap;
	if (effectInfo.specularTexture && !std::wstring{ effectInfo.specularTexture }.empty())
	{
		CreateTexture
		(
			effectInfo.specularTexture,
			pContext,
			specularMap.GetAddressOf()
		);
	}

	// モデルソースに情報を追加
	m_pModelSource->AddPartInfo(effectInfo, texture.Detach(), normalMap.Detach(), specularMap.Detach());

	return std::make_unique<DirectX::BasicEffect>(m_deviceRef.Get());
}

// テクスチャを作成
void Renderings::InstancingEffectFactory::CreateTexture(const wchar_t* name, ID3D11DeviceContext* pContext, ID3D11ShaderResourceView** ppTextureView)
{
	if (!name || !ppTextureView)
	{
		return;
	}

	*ppTextureView = nullptr;

	std::wstring path = m_directory + name;

	if (std::filesystem::path{ path }.extension() == L".dds")
	{
		Utility::ThrowIfFailed(DirectX::CreateDDSTextureFromFile
		(
			m_deviceRef.Get(),
			path.c_str(),
			nullptr,
			ppTextureView
		));
	}
	else
	{
		Utility::ThrowIfFailed(DirectX::CreateWICTextureFromFile
		(
			m_deviceRef.Get(),
			pContext,
			path.c_str(),
			nullptr,
			ppTextureView
		));
	}
}
