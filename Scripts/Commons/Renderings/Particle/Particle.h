/*
 * FileName:     Particle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * パーティクル
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class Transform;
}

namespace Systems
{
	class IResources;
}

namespace Renderings
{
	class Model3DSource;
	class ICameraScreen;
	class IModel3DRenderer;

	// パーティクル
	class Particle
	{

	public:


		/* 構造体 */

		// 更新情報
		struct UpdateElement
		{
			// 座標
			Math::Vector3 position{ Math::Vector3::Zero };
			// 回転
			Math::Quaternion rotation{ Math::Quaternion::Identity };
			// 拡大
			Math::Vector3 scale{ Math::Vector3::One };
		};

		// 描画情報
		struct RenderElement
		{
			// コンストラクタ
			RenderElement(Ref<Systems::IResources> iResourcesRef);

			// ワールド行列
			Math::Matrix matrix;
			// 色
			Math::Color color;
			// モデルソース
			Ref<const Model3DSource> modelSource;
		};


		/* メンバ関数 */

		// コンストラクタ
		Particle(Ref<IModel3DRenderer> iModelRendererRef, Ref<Systems::IResources> iResourcesRef);
		// デストラクタ
		virtual ~Particle();

		// 初期化処理
		virtual void Initalize(Ref<const Transform> transformRef) = 0;

		// 更新処理
		virtual void Update(float elapsedTime) = 0;

		// ワールド行列を適用
		void ApplyTransform();

		// モデルソースを設定
		void SetModelSource(const std::string& modelSourceName, Ref<RenderElement> renderElementRef);

		// 映るカメラ画面の参照を追加
		void AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);
		// 映るカメラ画面参照を削除
		void RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);

		// アクティブフラグを取得
		bool IsActive() const { return m_isActive; }


	protected:


		/* メンバ関数 */

		// 要素を追加
		void AddElement(UpdateElement** ppUpdateElement, RenderElement** ppRenderElement);

		// 要素の参照を取得
		Ref<UpdateElement> GetUpdateElementRef(size_t index) { return Ref{ m_updateElements.at(index) }; }

		// 非アクティブ化
		void Destroy() { m_isActive = false; }


	private:


		/* メンバ変数 */

		// 更新情報リスト
		std::vector<UpdateElement> m_updateElements;
		// 描画情報リストリスト
		std::vector<std::unique_ptr<RenderElement>> m_renderElements;

		// アクティブフラグ
		bool m_isActive;

		// 映るカメラ画面の参照リスト
		std::vector<Ref<const ICameraScreen>> m_iCameraScreenRefs;

		// モデル描画インタフェースの参照
		Ref<IModel3DRenderer> m_iModelRendererRef;

		// リソース管理の参照
		Ref<Systems::IResources> m_iResourcesRef;

	};
}
