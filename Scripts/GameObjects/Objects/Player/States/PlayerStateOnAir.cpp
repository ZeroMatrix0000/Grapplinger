/*
 * FileName:     PlayerStateOnAir.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/23
 *
 * プレイヤーの空中にいる状態
 */

#include "Pch.h"
#include "PlayerStates.h"

#include "../PlayerInternals.h"
#include "../PlayerWire.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"
#include "Scripts/GameObjects/Particles/JumpingParticle.h"

 // コンストラクタ
PlayerStateOnAir::PlayerStateOnAir()
{
}

// 開始処理
void PlayerStateOnAir::Enter(Ref<PlayerInternals> internalsRef)
{
	internalsRef->fallCoyoteTime = internalsRef->fallCoyoteTime.GetMax();
}

// 更新処理
void PlayerStateOnAir::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	// カメラのY軸回転
	Math::Quaternion cameraRotation = Math::Quaternion::CreateFromAxisAngle
	(
		Math::Vector3::UnitY,
		Math::Deg2Rad(internalsRef->P_CAMERA_SCREEN->GetCamera().rotation.y)
	);

	// 最大の早さを調整
	internalsRef->moveMaxSpeed += internalsRef->MOVE_ACCELERATION * elapsedTime * internalsRef->DASH_RATIO * (internalsRef->IS_DASH ? 1.0f : -1.0f);

	// 加速度がない
	if (internalsRef->MOVE == Math::Vector3::Zero)
	{
		// 速さ
		float speed = Math::Clamp
		(
			internalsRef->moveVelocity.Length() - internalsRef->MOVE_ACCELERATION * elapsedTime,
			0.0f,
			static_cast<float>(internalsRef->moveMaxSpeed)
		);
		internalsRef->moveVelocity.Normalize();
		internalsRef->moveVelocity *= speed;
	}
	// 加速度がある
	else
	{
		// 速度に加速度を足す
		internalsRef->moveVelocity += Math::Vector3::Transform(internalsRef->MOVE, cameraRotation) *
			internalsRef->MOVE_ACCELERATION * elapsedTime * (internalsRef->IS_DASH ? internalsRef->DASH_RATIO : 1.0f);
		// 速さを最大に収める
		if (internalsRef->moveVelocity.Length() > internalsRef->moveMaxSpeed)
		{
			internalsRef->moveVelocity.Normalize();
			internalsRef->moveVelocity *= internalsRef->moveMaxSpeed;
		}

		internalsRef->rotation.SetTarget
		(
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::UnitY, Math::Arctan2(internalsRef->moveVelocity.x, internalsRef->moveVelocity.z)),
			200.0f,
			25.0f
		);
	}

	// 落下速度を計算
	internalsRef->fallSpeed += internalsRef->FALL_ACCELERATION * elapsedTime;

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
		internalsRef->fallSpeed = -internalsRef->JUMP_POWER;
		internalsRef->jumpBufferTime = internalsRef->jumpBufferTime.GetMin();
		// 砂煙の再生
		internalsRef->pParticleManager->Play<JumpingParticle>();
	}

	// 糸の更新
	internalsRef->pWire->Update(elapsedTime);

	// 回転の更新
	internalsRef->rotation.Tick(elapsedTime);

	// 位置に速度を足す
	internalsRef->pTransform->Translate((internalsRef->moveVelocity - Math::Vector3::UnitY * internalsRef->fallSpeed) * elapsedTime);
	// 回転させる
	internalsRef->pTransform->SetRotation(internalsRef->rotation.GetCurrent());

	// 落下猶予
	internalsRef->fallCoyoteTime -= elapsedTime;
	if (internalsRef->fallCoyoteTime.IsMin())
	{
		 SetNextState(std::make_unique<PlayerStateFalling>());
	}

	// 当たり判定の更新
	internalsRef->pBoxCollider->ApplyTransform();

	// パーティクルの更新
	internalsRef->pParticleManager->Update(elapsedTime);
}

// 終了処理
void PlayerStateOnAir::Exit(Ref<PlayerInternals> internalsRef)
{
}

// 座標補正
void PlayerStateOnAir::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
	if (distance < 0.0f)
	{
		float cos = direction.Dot(Math::Vector3::Up);

		// 移動量
		Math::Vector3 displacement = direction * distance;
		// 移動方向の角度が 45° 以上なら真上に移動する
		if (Math::Abs(cos) > 1.0f / 1.41421356f)
		{
			displacement = Math::Vector3::Up / cos * distance;
			// 地面にいる状態にする
			SetNextState(std::make_unique<PlayerStateOnGround>());
		}
		// 移動
		internalsRef->pTransform->Translate(displacement);
		// トランスフォームを適用
		internalsRef->pBoxCollider->ApplyTransform();
	}
}
