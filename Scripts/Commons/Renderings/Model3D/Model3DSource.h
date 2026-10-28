/*
 * FileName:     Model3DSource.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 3Dモデルのソース
 */

#pragma once

namespace Renderings
{
	class InstancingEffectFactory;
	class PixelShader;

	// 3Dモデルのソース
	class Model3DSource
	{

	public:


		/* 構造体 */

		// パーツごとの情報
		struct PartInfo
		{
			// エフェクト情報
			DirectX::IEffectFactory::EffectInfo effectInfo{};
			// テクスチャ
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture{};
			// 法線マップ
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap{};
			// 鏡面反射マップ
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularMap{};
		};


		/* メンバ関数 */

		// コンストラクタ
		Model3DSource(Ref<const PixelShader> pixelShader);

		// 初期化処理
		void Initialize(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef, Ref<InstancingEffectFactory> fxRef);

		// パーツごとの情報を追加
		void AddPartInfo
		(
			const DirectX::IEffectFactory::EffectInfo& effectInfo,
			ID3D11ShaderResourceView* pTexture,
			ID3D11ShaderResourceView* pNormalMap,
			ID3D11ShaderResourceView* pSpecularMap
		);

		// 深度ステンシルステートを設定
		void SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc, Ref<ID3D11Device5> deviceRef);
		// 描画順を設定
		void SetDrawingOrder(int drawingOrder) { m_drawingOrder = drawingOrder; }

		// ピクセルシェーダを設定
		void SetPixelShader(Ref<const PixelShader> pixelShader) { m_pixelShader = pixelShader; }

		// モデルを取得
		const auto& GetModel() const
		{
			return *m_model;
		}

		// パーツごとの情報を取得
		const PartInfo& GetPartInfo(size_t index) const { return m_partInfos.at(index); };

		// 深度ステンシルステートを取得
		auto* GetDepthStencilState() const { return m_depthStencilState.Get(); }
		// 描画順を取得
		int GetDrawingOrder() const { return m_drawingOrder; }

		// ピクセルシェーダのポインタを取得
		Ref<const PixelShader> GetPixelShader() const { return m_pixelShader; }

		// 初期化済みかどうかを取得
		bool IsInitialized() const { return m_model.get(); }

		/* 静的関数 */

		// 生成
		static Model3DSource Create(Ref<const PixelShader> pixelShader, const std::wstring& filePath, Ref<ID3D11Device5> deviceRef, Ref<InstancingEffectFactory> fxRef);


	private:


		/* メンバ変数 */

		// モデル
		std::unique_ptr<DirectX::Model> m_model;

		// パーツごとの情報
		std::vector<PartInfo> m_partInfos;

		// 深度ステンシルステート
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		// 描画順
		int m_drawingOrder;

		// ピクセルシェーダ
		Ref<const PixelShader> m_pixelShader;

	};
}
