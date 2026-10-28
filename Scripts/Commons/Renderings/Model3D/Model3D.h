/*
 * FileName:     Model3D.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * 3Dモデル
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Systems
{
	class IResources;
}

namespace Renderings
{
	class Model3DSource;
	class ICameraScreen;
	class IModel3DRenderer;

	// 3Dモデル
	class Model3D : public Component
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Model3D(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRenderer, Ref<Systems::IResources> iResources);
		// デストラクタ
		~Model3D();

		// 初期化処理
		void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

		// 色を設定
		void SetColor(const Math::Color& color) { m_color = color; }
		// 色を設定
		void SetColor(const DirectX::XMVECTORF32& color) { m_color = color; }

		// モデルソースを設定
		void SetModelSource(const std::string& modelSourceName);

		// 映るカメラ画面の参照を追加
		void AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);
		// 映るカメラ画面参照を削除
		void RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);

		// 色を取得
		const Math::Color& GetColor() const { return m_color; }

		// モデルソースを取得
		Ref<const Model3DSource> GetModelSource() const { return m_modelSource; }


	private:


		/* メンバ変数 */

		// 色
		Math::Color m_color;

		// モデルソースの参照
		Ref<const Model3DSource> m_modelSource;

		// 映るカメラ画面の参照リスト
		std::vector<Ref<const ICameraScreen>> m_iCameraScreenRefs;

		// モデル描画インタフェースの参照
		Ref<IModel3DRenderer> m_iModelRendererRef;

		// リソース管理の参照
		Ref<Systems::IResources> m_iResources;

	};
}
