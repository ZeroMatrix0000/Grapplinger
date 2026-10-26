/*
 * FileName:     IColliderRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * 当たり判定描画のインタフェース
 */

#pragma once

#include "../Systems/OnlyOne.h"

namespace Colliders
{
	class BoxCollider;
	class SphereCollider;
	class MeshCollider;
}

namespace Renderings
{
	// 当たり判定描画のインタフェース
	class IColliderRenderer : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		IColliderRenderer()
			: OnlyOne{ typeid(IColliderRenderer) }
		{
		}
		// デストラクタ
		virtual ~IColliderRenderer() = default;

		// 長方形の当たり判定の参照を追加
		virtual void AddPBoxCollider(Ref<const Colliders::BoxCollider> boxColliderRef) = 0;
		// 長方形の当たり判定の参照を削除
		virtual void RemovePBoxCollider(Ref<const Colliders::BoxCollider> boxColliderRef) = 0;

		// 球の当たり判定の参照を追加
		virtual void AddPSphereCollider(Ref<const Colliders::SphereCollider> sphereColliderRef) = 0;
		// 球の当たり判定の参照を削除
		virtual void RemovePSphereCollider(Ref<const Colliders::SphereCollider> sphereColliderRef) = 0;

		// メッシュの当たり判定の参照を追加
		virtual void AddPMeshCollider(Ref<const Colliders::MeshCollider> meshColliderRef) = 0;
		// メッシュの当たり判定の参照を削除
		virtual void RemovePMeshCollider(Ref<const Colliders::MeshCollider> meshColliderRef) = 0;

	};
}
