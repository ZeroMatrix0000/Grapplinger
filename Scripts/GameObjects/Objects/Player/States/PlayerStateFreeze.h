/*
 * FileName:     PlayerStateFreeze.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/05
 *
 * プレイヤーのフリーズ状態
 */

#pragma once

#include "PlayerState.h"

 // プレイヤーのフリーズ状態
class PlayerStateFreeze : public PlayerState
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerStateFreeze();

	// 開始処理
	void Enter(Ref<PlayerInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<PlayerInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<PlayerInternals> internalsRef) override;

	// 座標補正
	void PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction) override;


	/* 定数 */

	static constexpr float FREEZE_TIME = 0.125f;


	/* メンバ変数 */

	// タイマー
	Limited m_timer;

};
