/*
 * FileName:     VertexShader.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 頂点データ
 */

#pragma once

#include "ConstantBuffer.h"

namespace Renderings
{
	class VertexShader
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		VertexShader();

		// 初期化処理
		void Initialize(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef);

		// ブロブデータを取得
		Ref<ID3DBlob> GetBlobRef() const { return Ref{ *m_blob.Get() }; }
		// シェーダを取得
		Ref<ID3D11VertexShader> GetD3DShaderRef() const { return Ref{ *m_d3dShader.Get() }; }
		// 定数バッファを取得
		Ref<ConstantBuffer> GetConstantBufferRef() const { return Ref{ *m_constantBuffer.get() }; }

		// 定数バッファを GPU に送信
		void SetConstantBuffer(UINT index, Ref<ID3D11DeviceContext4> contextRef) const;

		// 初期化済みかどうかを取得
		bool IsInitialized() const { return m_blob.Get() && m_d3dShader.Get(); }


		/* 静的関数 */

		// 生成
		static VertexShader Create(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef);


	private:


		/* メンバ変数 */

		// ブロブデータ
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;

		// シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_d3dShader;

		// 定数バッファ
		std::unique_ptr<ConstantBuffer> m_constantBuffer;

	};
}