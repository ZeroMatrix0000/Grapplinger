/*
 * FileName:     PlayerStateFalling.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/15
 *
 * プレイヤーの落下状態
 */

#pragma once

#include "PlayerState.h"

// プレイヤーの落下状態
class PlayerStateFalling : public PlayerState
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerStateFalling();

	// 開始処理
	void Enter(Ref<PlayerInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<PlayerInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<PlayerInternals> internalsRef) override;

	// 座標補正
	void PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction) override;

};
