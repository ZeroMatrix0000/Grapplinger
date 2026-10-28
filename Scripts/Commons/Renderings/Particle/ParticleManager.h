/*
 * FileName:     ParticleManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * パーティクル管理
 */

#pragma once

#include "Particle.h"
#include "Scripts/Commons/Components/Component.h"

namespace Systems
{
	class IResources;
}

namespace Renderings
{
	class ICameraScreen;
	class IModel3DRenderer;

	// パーティクル管理
	class ParticleManager : public Component
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		ParticleManager(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRenderer, Ref<Systems::IResources> iResources);

		// 初期化処理
		void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

		// 更新処理
		void Update(float elapsedTime);

		// 映るカメラ画面の参照を追加
		void AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);
		// 映るカメラ画面参照を削除
		void RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef);

		// パーティクルを生成
		template<typename TParticle> requires IsDerived<TParticle, Particle>
		void Play()
		{
			// パーティクルを生成
			auto& particle = m_particles.emplace_back();
			particle = std::make_unique<TParticle>(m_iModelRendererRef, m_iResources);
			// カメラを設定
			for (Ref iCameraScreenRef : m_iCameraScreenRefs)
			{
				particle->AddICameraScreenRef(iCameraScreenRef);
			}
			// 初期化処理
			particle->Initalize(m_transformRef);
		}


	private:


		/* メンバ変数 */

		// パーティクルリスト
		std::vector<std::unique_ptr<Particle>> m_particles;

		// トランスフォームの参照
		Ref<const Transform> m_transformRef;

		// 映るカメラ画面の参照リスト
		std::vector<Ref<const ICameraScreen>> m_iCameraScreenRefs;

		// モデル描画インタフェースの参照
		Ref<IModel3DRenderer> m_iModelRendererRef;

		// リソース管理の参照
		Ref<Systems::IResources> m_iResources;

	};
}
