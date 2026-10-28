/*
 * FileName:     PlayerStateExpandWire.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * プレイヤーの地面にいる状態
 */

#include "Pch.h"
#include "PlayerStates.h"

#include "../PlayerWire.h"
#include "../PlayerModel.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"

 // コンストラクタ
PlayerStateExpandWire::PlayerStateExpandWire(const Math::Vector3& targetPosition, PlayerInternals::TargetType targetType)
	: m_targetPosition{ targetPosition }
	, m_targetType{ targetType }
	, m_playerPosition{}
{
}

// 開始処理
void PlayerStateExpandWire::Enter(Ref<PlayerInternals> internalsRef)
{
	m_playerPosition = internalsRef->boxColliderRef->GetWorldBox().position;

	// 速度を 0 にする
	internalsRef->moveVelocity = Math::Vector3::Zero;
	internalsRef->fallSpeed = 0.0f;

	// 向き
	Math::Vector3 direction = m_targetPosition - m_playerPosition;
	direction.Normalize();

	// モデルを回転
	internalsRef->modelRef->SetWireAngle(Math::Rad2Deg(Math::Arcsin(-direction.y / direction.Length())));

	Math::Quaternion wireRotation = Math::Quaternion::CreateFromRotationMatrix
	(
		Math::Matrix::CreateWorld(Math::Vector3::Zero, -direction, Math::Vector3::Up)
	);
	direction.y = 0;

	// ターゲットの方向へ回転
	internalsRef->rotation.SetTarget
	(
		Math::Quaternion::CreateFromRotationMatrix(Math::Matrix::CreateWorld(Math::Vector3::Zero, -direction, Math::Vector3::Up)),
		200.0f,
		25.0f
	);

	// ターゲットを設定
	internalsRef->wireRef->SetTarget
	(
		internalsRef->boxColliderRef->GetWorldBox().position,
		m_targetPosition,
		wireRotation,
		m_targetType == PlayerInternals::TargetType::Air
	);
}

// 更新処理
void PlayerStateExpandWire::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	// 糸の更新
	internalsRef->wireRef->Update(elapsedTime);

	// 回転の更新
	internalsRef->rotation.Tick(elapsedTime);

	// 座標を調整
	m_playerPosition = internalsRef->wireRef->GetPlayerPosition();
	internalsRef->transformRef->SetPosition(m_playerPosition + Math::Vector3::Down * internalsRef->boxColliderRef->GetWorldBox().size.y / 2.0f);

	// 回転させる
	internalsRef->transformRef->SetRotation(internalsRef->rotation.GetCurrent());

	// 当たり判定の更新
	internalsRef->boxColliderRef->ApplyTransform();

	// パーティクルの更新
	internalsRef->particleManagerRef->Update(elapsedTime);

	// 落下状態に戻す
	if (internalsRef->wireRef->GetCurrentState() == PlayerWire::State::None)
	{
		switch (m_targetType)
		{
		case PlayerInternals::TargetType::Object:
			SetNextState(std::make_unique<PlayerStateFalling>());
			internalsRef->fallSpeed = internalsRef->fallSpeed.GetMin();
			internalsRef->canExpandWire = true;
			internalsRef->wireRef->UnDisplay();
			break;
		case PlayerInternals::TargetType::Ground:
			SetNextState(std::make_unique<PlayerStateStick>());
			break;
		case PlayerInternals::TargetType::Air:
			SetNextState(std::make_unique<PlayerStateFalling>());
			internalsRef->wireRef->UnDisplay();
			break;
		default:
			break;
		}
	}
}

// 終了処理
void PlayerStateExpandWire::Exit(Ref<PlayerInternals> internalsRef)
{
}

// 座標補正
void PlayerStateExpandWire::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
}
