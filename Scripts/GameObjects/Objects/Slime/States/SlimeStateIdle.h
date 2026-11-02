/*
 * FileName:     SlimeStateIdle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライムの待機状態
 */

#pragma once

#include "Scripts/Commons/Systems/State.h"
#include "../SlimeInternals.h"

// スライムの待機状態
class SlimeStateIdle : public Systems::State<SlimeInternals>
{

public:


	/* メンバ関数 */

	// コンストラクタ
	SlimeStateIdle();

	// 開始処理
	void Enter(Ref<SlimeInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<SlimeInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<SlimeInternals> internalsRef) override;

};
