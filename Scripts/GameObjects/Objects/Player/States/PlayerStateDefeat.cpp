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
PlayerStateDefeat::PlayerStateDefeat()
{
}

// 開始処理
void PlayerStateDefeat::Enter(Ref<PlayerInternals> internalsRef)
{
	internalsRef->moveVelocity = Math::Vector3::Zero;
	internalsRef->particleManagerRef->Play<PlayerDefeatParticle>();
}

// 更新処理
void PlayerStateDefeat::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
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
