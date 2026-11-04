/*
 * FileName:     PlayerStateDefeat.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/04
 *
 * プレイヤーの敗北状態
 */

#include "Pch.h"
#include "PlayerStateDefeat.h"

#include "../PlayerInternals.h"
#include "Scripts/GameObjects/Particles/PlayerDefeatParticle.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"

// コンストラクタ
PlayerStateDefeat::PlayerStateDefeat(bool isFalling)
	: PlayerState{}
	, m_isFalling{ isFalling }
{
}

// 開始処理
void PlayerStateDefeat::Enter(Ref<PlayerInternals> internalsRef)
{
	if (!m_isFalling)
	{
		internalsRef->moveVelocity = Math::Vector3::Zero;
		internalsRef->fallSpeed = 0.0f;
		internalsRef->transformRef->SetScale(Math::Vector3::Zero);
		internalsRef->particleManagerRef->Play<PlayerDefeatParticle>();
	}
}

// 更新処理
void PlayerStateDefeat::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	// 移動
	internalsRef->transformRef->Translate((internalsRef->moveVelocity - Math::Vector3::UnitY * internalsRef->fallSpeed) * elapsedTime);

	// 当たり判定の更新
	internalsRef->boxColliderRef->ApplyTransform();

	// パーティクル管理を更新
	internalsRef->particleManagerRef->Update(elapsedTime);
}

// 終了処理
void PlayerStateDefeat::Exit(Ref<PlayerInternals> internalsRef)
{
}

// 座標補正
void PlayerStateDefeat::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
}
