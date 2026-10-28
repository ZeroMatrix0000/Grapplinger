/*
 * FileName:     PlayerStateStick.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/23
 *
 * プレイヤーの張り付き状態
 */

#include "Pch.h"
#include "PlayerStateStick.h"

#include "PlayerStates.h"
#include "../PlayerWire.h"
#include "../PlayerInternals.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Particles/LandingParticle.h"
#include "Scripts/GameObjects/Particles/JumpingParticle.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"

// コンストラクタ
PlayerStateStick::PlayerStateStick()
{
}

// 開始処理
void PlayerStateStick::Enter(Ref<PlayerInternals> internalsRef)
{
}

// 更新処理
void PlayerStateStick::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	// カメラのY軸回転
	Math::Quaternion cameraRotation = Math::Quaternion::CreateFromAxisAngle
	(
		Math::Vector3::UnitY,
		Math::Deg2Rad(internalsRef->P_CAMERA_SCREEN->GetCamera().rotation.y)
	);

	// 加速度がある
	if (internalsRef->MOVE != Math::Vector3::Zero)
	{
		Math::Vector3 direction = Math::Vector3::Transform(internalsRef->MOVE, cameraRotation);
		internalsRef->rotation.SetTarget
		(
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::UnitY, Math::Arctan2(direction.x, direction.z)),
			200.0f,
			25.0f
		);
	}

	// ジャンプ入力
	if (internalsRef->IS_JUMP)
	{
		internalsRef->jumpBufferTime = internalsRef->jumpBufferTime.GetMax();
	}
	internalsRef->jumpBufferTime -= elapsedTime;

	// ジャンプ
	if (internalsRef->jumpBufferTime != 0.0f)
	{
		SetNextState(std::make_unique<PlayerStateFalling>());
		internalsRef->fallSpeed = internalsRef->fallSpeed.GetMin();
		internalsRef->jumpBufferTime = internalsRef->jumpBufferTime.GetMin();
		// 砂煙の再生
		internalsRef->pParticleManager->Play<JumpingParticle>();
	}

	// 糸の更新
	internalsRef->pWire->Update(elapsedTime);

	// 回転の更新
	internalsRef->rotation.Tick(elapsedTime);

	// 回転させる
	internalsRef->pTransform->SetRotation(internalsRef->rotation.GetCurrent());

	// 当たり判定の更新
	internalsRef->pBoxCollider->ApplyTransform();

	// パーティクルの更新
	internalsRef->pParticleManager->Update(elapsedTime);
}

// 終了処理
void PlayerStateStick::Exit(Ref<PlayerInternals> internalsRef)
{
	internalsRef->pWire->UnDisplay();
}

// 座標補正
void PlayerStateStick::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
	float cos = direction.Dot(Math::Vector3::Up);

	// 距離が近い
	if (distance < 0.0f || (distance < internalsRef->pBoxCollider->GetWorldBox().size.y / 8.0f && Math::Abs(cos) > 1.0f / 1.41421356f))
	{
		// 移動量
		Math::Vector3 displacement = direction * distance;
		// 移動方向の角度が 45° 以上なら真上に移動する
		if (cos < -1.0f / 1.41421356f)
		{
			// 移動
			internalsRef->pTransform->Translate(displacement);
			// トランスフォームを適用
			internalsRef->pBoxCollider->ApplyTransform();
			// 地面にいる状態にする
			SetNextState(std::make_unique<PlayerStateOnGround>());
			// 砂煙の再生
			internalsRef->pParticleManager->Play<LandingParticle>();
			return;
		}
		// 移動
		internalsRef->pTransform->Translate(displacement);
		// トランスフォームを適用
		internalsRef->pBoxCollider->ApplyTransform();

		internalsRef->pWire->SetPlayerPosition(internalsRef->pBoxCollider->GetWorldBox().position);
	}
}
