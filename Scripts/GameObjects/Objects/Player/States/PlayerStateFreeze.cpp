/*
 * FileName:     PlayerStateFreeze.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/05
 *
 * プレイヤーのフリーズ状態
 */

#include "Pch.h"
#include "PlayerStateFreeze.h"

#include "PlayerStates.h"

// コンストラクタ
PlayerStateFreeze::PlayerStateFreeze()
	: PlayerState{}
	, m_timer{}
{
}

// 開始処理
void PlayerStateFreeze::Enter(Ref<PlayerInternals> internalsRef)
{
	m_timer.Initialize(0.0f, 0.0f, FREEZE_TIME);
}

// 更新処理
void PlayerStateFreeze::Update(Ref<PlayerInternals> internalsRef, float elapsedTime)
{
	m_timer += elapsedTime;
	if (m_timer.IsMax())
	{
		SetNextState(std::make_unique<PlayerStateDefeat>(false));
	}
}

// 終了処理
void PlayerStateFreeze::Exit(Ref<PlayerInternals> internalsRef)
{
}

// 座標補正
void PlayerStateFreeze::PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction)
{
}
