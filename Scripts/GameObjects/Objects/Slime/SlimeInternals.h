/*
 * FileName:     SlimeInternals.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライムの内部データ
 */

#pragma once

namespace Components
{
	class Transform;
}
namespace GameObjects
{
	class GameObject;
}
namespace Colliders
{
	class BoxCollider;
	class SphereCollider;
}

// スライムの内部データ
struct SlimeInternals
{

	/* メンバ関数 */

	// コンストラクタ
	SlimeInternals(Ref<GameObject> owner);


	/* メンバ変数 */

	// トランスフォームの参照
	Ref<Transform> transformRef;
	// 長方形の当たり判定の参照
	Ref<Colliders::BoxCollider> boxColliderRef;
	// 球の当たり判定の参照
	Ref<Colliders::SphereCollider> sphereColliderRef;

};
