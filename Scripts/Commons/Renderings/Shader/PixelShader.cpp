/*
 * FileName:     PixelShader.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * ピクセルシェーダ
 */

#include "Pch.h"
#include "PixelShader.h"

// コンストラクタ
Renderings::PixelShader::PixelShader()
	: m_blob{}
	, m_d3dShader{}
	, m_constantBuffer{}
{
	m_constantBuffer = std::make_unique<ConstantBuffer>();
}

// 初期化処理
void Renderings::PixelShader::Initialize(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef)
{
	// ブロブデータ
	Utility::ThrowIfFailed(D3DReadFileToBlob(filePath.c_str(), m_blob.GetAddressOf()));

	// ピクセルシェーダ
	Utility::ThrowIfFailed(deviceRef->CreatePixelShader
	(
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize(),
		nullptr,
		m_d3dShader.GetAddressOf()
	));

	// 定数バッファ
	m_constantBuffer->Initialize(deviceRef, Ref{ *m_blob.Get() });
}

// 定数バッファを GPU に送信
void Renderings::PixelShader::SetConstantBuffer(UINT index, Ref<ID3D11DeviceContext4> contextRef) const
{
	// バッファがなければ終了
	if (index >= m_constantBuffer->GetBufferCount())
	{
		return;
	}

	auto& data = m_constantBuffer->GetData(index);
	Ref bufferRef = m_constantBuffer->GetBufferRef(index);

	if (data.size() != 0)
	{
		// 定数バッファを設定
		contextRef->UpdateSubresource(bufferRef.Get(), 0, nullptr, data.data(), 0, 0);
		contextRef->PSSetConstantBuffers(index, 1, bufferRef.GetAddress());
	}
}

// 生成
Renderings::PixelShader Renderings::PixelShader::Create(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef)
{
	PixelShader shader;
	shader.Initialize(filePath, deviceRef);
	return shader;
}
