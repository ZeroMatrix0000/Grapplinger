/*
 * FileName:     PlayerStateOnGround.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/23
 *
 * プレイヤーの地面にいる状態
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
PlayerStateOnGround::PlayerStateOnGround()
	: m_isGround{}
{
}

// 開始処理
void PlayerStateOnGround::Enter(Ref<PlayerInternals> internalsRef)
{
	internalsRef->fallSpeed = 0.0f;
	internalsRef->canExpandWire = true;
}

// 更新処理
void PlayerStateOnGround::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	m_isGround = false;

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

	// 当たり判定の更新
	internalsRef->pBoxCollider->ApplyTransform();

	// パーティクルの更新
	internalsRef->pParticleManager->Update(elapsedTime);
}

// 終了処理
void PlayerStateOnGround::Exit(Ref<PlayerInternals> internalsRef)
{
}

// 座標補正
void PlayerStateOnGround::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
	float cos = direction.Dot(Math::Vector3::Up);

	// めり込んでいる、または、距離が近いかつ移動方向が下
	if (distance < 0.0f || (distance < internalsRef->pBoxCollider->GetWorldBox().size.y / 8.0f && Math::Abs(cos) > 1.0f / 1.41421356f))
	{
		// 移動量
		Math::Vector3 displacement = direction * distance;

		// 移動方向の角度が 45° 以上なら真上に移動する
		if (Math::Abs(cos) > 1.0f / 1.41421356f)
		{
			displacement = Math::Vector3::Up / cos * distance;
			// 落下速度を 0 にする
			internalsRef->fallSpeed = 0.0f;
		}

		// 移動
		internalsRef->pTransform->Translate(displacement);
		// トランスフォームを適用
		internalsRef->pBoxCollider->ApplyTransform();

		// 地面についていることを確定させる
		m_isGround = true;
		// 空中にいる状態になっているならリセット
		if (GetNextState())
		{
			ResetNextState();
		}
	}
	else
	{
		// 確定していないなら
		if (!m_isGround)
		{
			// 空中にいる状態にする
			SetNextState(std::make_unique<PlayerStateOnAir>());
		}
	}
}
