/*
 * FileName:     PlayerStateStick.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/19
 *
 * プレイヤーの張り付き状態
 */

#pragma once

#include "PlayerState.h"

 // プレイヤーの張り付き状態
class PlayerStateStick : public PlayerState
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerStateStick();

	// 開始処理
	void Enter(Ref<PlayerInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<PlayerInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<PlayerInternals> internalsRef) override;

	// 座標補正
	void PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction) override;

};
