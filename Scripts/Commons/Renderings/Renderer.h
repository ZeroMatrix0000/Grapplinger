/*
 * FileName:     Renderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
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
			Ref<ID3D11Device5>               deviceRef,
			Ref<ID3D11DeviceContext4>        contextRef,
			Ref<const DirectX::CommonStates> commonStatesRef
		);
		// 描画処理
		void Render();

		// 描画モードを返る
		void ChangeRenderMode();

		// モデル描画インタフェースを取得
		Ref<Renderings::Model3DRenderer>  GetModelRenderer()    { return Ref{ m_modelRenderer }; }
		// 画像描画インタフェースを取得
		Ref<Renderings::ImageRenderer>    GetImageRenderer()    { return Ref{ m_imageRenderer }; }
		// テキスト描画インタフェースを取得
		Ref<Renderings::TextRenderer>     GetTextRenderer()     { return Ref{ m_textRenderer }; }
		// 当たり判定描画インタフェースを取得
		Ref<Renderings::ColliderRenderer> GetColliderRenderer() { return Ref{ m_colliderRenderer }; }


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
