/*
 * FileName:     SlimeInternals.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライムの内部データ
 */

#include "Pch.h"
#include "SlimeInternals.h"

#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
SlimeInternals::SlimeInternals(Ref<GameObject> owner)
	: transformRef{ owner->GetNullComponent<Transform>() }
	, boxColliderRef{ owner->GetNullComponent<Colliders::BoxCollider>() }
	, sphereColliderRef{ owner->GetNullComponent<Colliders::SphereCollider>() }
{
}
