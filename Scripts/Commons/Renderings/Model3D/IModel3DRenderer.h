/*
 * FileName:     IModel3DRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/25
 *
 * 3Dモデル描画のインタフェース
 */

#pragma once

#include "../Particle/Particle.h"
#include "Scripts/Commons/Systems/OnlyOne.h"

namespace Renderings
{
	class Model3D;
	class ICameraScreen;

	// 3Dモデル描画のインタフェース
	class IModel3DRenderer : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		IModel3DRenderer()
			: OnlyOne{ typeid(IModel3DRenderer) }
		{
		}
		// デストラクタ
		virtual ~IModel3DRenderer() = default;

		// モデルの参照を追加
		virtual void AddModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef) = 0;
		// モデルの参照を削除
		virtual void RemoveModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef) = 0;

		// パーティクルの参照を追加
		virtual void AddParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef) = 0;
		// パーティクルの参照を削除
		virtual void RemoveParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef) = 0;

	};
}
