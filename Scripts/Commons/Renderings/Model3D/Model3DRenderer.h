/*
 * FileName:     Model3DRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 3Dモデル描画
 */

#pragma once

#include "IModel3DRenderer.h"
#include "Model3DSource.h"

namespace Systems
{
	class IResources;
}

namespace Renderings
{
	class VertexShader;
	class PixelShader;

	// 3Dモデル描画
	class Model3DRenderer : public IModel3DRenderer
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Model3DRenderer();

		// 初期化処理
		void Initialize
		(
			Ref<ID3D11Device5> deviceRef,
			Ref<ID3D11DeviceContext4> contextRef,
			Ref<const DirectX::CommonStates> commonStatesRef
		);
		// 描画処理
		void Render(bool isInstance);

		// 深度ステンシルビューを設定
		void SetDepthStencilView(Ref<ID3D11DepthStencilView> depthStencilViewRef) { m_pDepthStencilView = depthStencilViewRef.Get(); };

		// 頂点シェーダを設定
		void SetVertexShader(Ref<const VertexShader> vertexShader);
		// ピクセルシェーダを設定
		void SetPixelShader(Ref<const PixelShader> pixelShader) { m_pPixelShader = pixelShader.Get(); }

		// モデルの参照を追加
		void AddModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef) override;
		// モデルの参照を削除
		void RemoveModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef) override;

		// パーティクルの参照を追加
		void AddParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef) override;
		// パーティクルの参照を削除
		void RemoveParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef) override;


	private:


		/* 構造体 */

		// 描画オブジェクト
		struct RenderObjects
		{
			// モデルの参照リスト
			std::vector<Ref<const Model3D>> modelRefs{};
			// パーティクルの参照リスト
			std::vector<Ref<const Particle::RenderElement>> particleElementRefs{};
		};

		// 入力要素
		struct InputElement
		{
			DirectX::XMFLOAT3X4 instMatrix;
			DirectX::XMFLOAT4 color;
		};


		/* メンバ関数 */

		// 白画像を作成
		void CreateWhiteTexture();

		// 入力レイアウトを作成
		void CreateInputLayout();

		// インスタンスバッファを更新
		void UpdateInstanceBuffer(const std::vector<InputElement>& inputElements);

		// パーツの情報を設定
		void SetPartInfo(const Model3DSource::PartInfo& partInfo);

		// 描画
		void Draw(Ref<const ICameraScreen> iCameraScreenRef, Ref<const DirectX::ModelMeshPart> partRef);
		// インスタンス描画
		void DrawInstanced(Ref<const ICameraScreen> iCameraScreenRef, Ref<const DirectX::ModelMeshPart> partRef, size_t instMatricesCount);


		/* メンバ変数 */

		// インスタンスバッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_instanceBuffer;

		// 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		// 白画像
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_whiteTexture;

		// 描画オブジェクトリスト
		std::unordered_map<Ref<const ICameraScreen>, std::unordered_map<Ref<const Model3DSource>, RenderObjects>> m_renderObjects;

		// デバイスのポインタ
		ID3D11Device5* m_pDevice;
		// コンテキストのポインタ
		ID3D11DeviceContext4* m_pContext;
		// コモンステートのポインタ
		const DirectX::CommonStates* m_pCommonStates;

		// 深度ステンシルビューのポインタ
		ID3D11DepthStencilView* m_pDepthStencilView;

		// 頂点シェーダのポインタ
		const VertexShader* m_pVertexShader;
		// ピクセルシェーダのポインタ
		const PixelShader* m_pPixelShader;

	};
}
