/*
 * FileName:     ICameraScreen.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメラ画面のインタフェース
 */

#pragma once

#include "../Components/Component.h"
#include "Particle/Particle.h"

namespace Renderings
{
	class Model3D;
	class Model3DSource;
	class IModel3DRenderer;

	// カメラ画面のインタフェース
	class ICameraScreen : public Component
	{

	public:


		/* 列挙型 */

		// 描画オブジェクト
		struct RenderObjects
		{
			// モデルの参照リスト
			std::vector<Ref<const Model3D>> modelRefs{};
			// パーティクルの参照リスト
			std::vector<Ref<const Particle::RenderElement>> particleElementRefs{};
		};


		/* メンバ関数 */

		// コンストラクタ
		ICameraScreen(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRenderer);
		// デストラクタ
		virtual ~ICameraScreen();

		// ビュー行列を更新
		virtual void UpdateViewMatrix() = 0;

		// 座標を設定
		virtual void SetPosition(const Math::Vector3& position) = 0;

		// プロジェクション行列を設定
		virtual void SetProjectionMatrix(const Math::Vector2& outputSize) = 0;

		// ビュー行列を取得
		virtual const Math::Matrix& GetViewMatrix() const = 0;
		// プロジェクション行列を取得
		virtual const Math::Matrix& GetProjectionMatrix() const = 0;

		// 目の座標を取得
		virtual const Math::Vector3 GetEyePosition() const = 0;
		// 回転を取得
		virtual const Math::Vector3 GetEyeDirection() const = 0;

		// 出力サイズを取得
		virtual const Math::Vector2& GetOutputSize() const = 0;

		// モデルの参照を追加
		void AddModelRef(Ref<const Model3D> modelRef) const;
		// モデルの参照を削除
		void RemoveModelRef(Ref<const Model3D> modelRef) const;

		// パーティクルの参照を追加
		void AddParticleRef(Ref<const Particle::RenderElement> particleElementRef) const;
		// パーティクルの参照を削除
		void RemoveParticleRef(Ref<const Particle::RenderElement> particleElementRef) const;

		// 描画オブジェクトリストを取得
		const auto& GetRenderObjectsList(const IModel3DRenderer&) const { return m_renderObjectsList; }


	private:


		/* メンバ変数 */

		// モデル描画インタフェースの参照
		Ref<IModel3DRenderer> m_iModelRenderer;

		// 描画オブジェクトリスト
		mutable std::unordered_map<Ref<const Model3DSource>, RenderObjects> m_renderObjectsList;

	};
}
