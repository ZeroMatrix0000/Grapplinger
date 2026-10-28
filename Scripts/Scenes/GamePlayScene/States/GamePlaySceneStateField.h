/*
 * FileName:     GamePlaySceneStateField.h
 * Author:       Takao Hayata
 * Last Updated: 2026/09/06
 *
 * ゲームプレイシーンのフィールド状態
 */

#pragma once

#include "Scripts/Commons/Systems/State.h"

struct GamePlaySceneInternals;

// ゲームプレイシーンのフィールド状態
class GamePlaySceneStateField : public Systems::State<GamePlaySceneInternals>
{

public:


	/* メンバ関数 */

	// コンストラクタ
	GamePlaySceneStateField();

	// 開始処理
	void Enter(Ref<GamePlaySceneInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<GamePlaySceneInternals> internalsRef) override;

};
