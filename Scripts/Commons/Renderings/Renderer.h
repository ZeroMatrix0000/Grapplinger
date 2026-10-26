/*
 * FileName:     Renderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * 描画
 */

#pragma once

#include "Model3D/Model3DRenderer.h"
#include "Image/ImageRenderer.h"
#include "Text/TextRenderer.h"
#include "ColliderRenderer.h"
#include "../Systems/OnlyOne.h"

namespace Systems
{
	class IResources;
}

namespace Renderings
{
	class PixelShader;

	// 描画
	class Renderer : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Renderer();

		// 初期化処理
		void Initialize
		(
			ID3D11Device5*               pDevice,
			ID3D11DeviceContext4*        pContext,
			IDXGISwapChain4*             pSwapChain,
			const DirectX::CommonStates& commonStates
		);
		// 描画処理
		void Render();

		// 描画モードを返る
		void ChangeRenderMode();

		// モデル描画インタフェースの参照を取得
		Ref<Renderings::Model3DRenderer>  GetModelRendererRef()    { return Ref{ m_modelRenderer }; }
		// 画像描画インタフェースの参照を取得
		Ref<Renderings::ImageRenderer>    GetImageRendererRef()    { return Ref{ m_imageRenderer }; }
		// テキスト描画インタフェースの参照を取得
		Ref<Renderings::TextRenderer>     GetTextRendererRef()     { return Ref{ m_textRenderer }; }
		// 当たり判定描画インタフェースの参照を取得
		Ref<Renderings::ColliderRenderer> GetColliderRendererRef() { return Ref{ m_colliderRenderer }; }


	private:


		/* 列挙型 */

		// 描画モード
		enum class RenderMode
		{
			Model,
			Both,
			Collider,
			Length
		};


		/* メンバ変数 */

		// モデル描画
		Renderings::Model3DRenderer  m_modelRenderer;
		// 画像描画
		Renderings::ImageRenderer    m_imageRenderer;
		// テキスト描画
		Renderings::TextRenderer     m_textRenderer;
		// 当たり判定描画
		Renderings::ColliderRenderer m_colliderRenderer;

		// 描画モード
		RenderMode m_renderMode;

	};
}
