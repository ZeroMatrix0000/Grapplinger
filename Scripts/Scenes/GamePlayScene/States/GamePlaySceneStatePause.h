/*
 * FileName:     GamePlaySceneStatePause.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/16
 *
 * ゲームプレイシーンのポーズ状態
 */

#pragma once

#include "Scripts/Commons/Systems/State.h"

struct GamePlaySceneInternals;

// ゲームプレイシーンのポーズ状態
class GamePlaySceneStatePause : public Systems::State<GamePlaySceneInternals>
{

public:


	/* メンバ関数 */

	// コンストラクタ
	GamePlaySceneStatePause();

	// 開始処理
	void Enter(Ref<GamePlaySceneInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<GamePlaySceneInternals> internalsRef) override;

};
