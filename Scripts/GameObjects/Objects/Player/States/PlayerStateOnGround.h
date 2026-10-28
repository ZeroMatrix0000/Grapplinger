/*
 * FileName:     PlayerStateOnGround.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/15
 *
 * プレイヤーの地面にいる状態
 */

#pragma once

#include "PlayerState.h"

// プレイヤーの地面にいる状態
class PlayerStateOnGround : public PlayerState
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerStateOnGround();

	// 開始処理
	void Enter(Ref<PlayerInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<PlayerInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<PlayerInternals> internalsRef) override;

	// 座標補正
	void PositionCorrect(Ref<PlayerInternals> internalsRef, float distance, const Math::Vector3& direction) override;


private:


	/* メンバ変数 */

	// 地面についているかどうか
	bool m_isGround;

};
