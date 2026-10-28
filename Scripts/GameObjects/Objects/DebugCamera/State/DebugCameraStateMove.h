/*
 * FileName:     DebugCameraStateMove.h
 * Author:       Takao Hayata
 * Last Updated: 2026/07/08
 *
 * デバッグ用カメラの待機状態
 */

#pragma once

#include "Scripts/Commons/Systems/State.h"
#include "../DebugCameraInternals.h"

 // デバッグ用カメラの待機状態
class DebugCameraStateMove : public Systems::State<DebugCameraInternals>
{

public:


	/* メンバ関数 */

	// コンストラクタ
	DebugCameraStateMove();

	// 開始処理
	void Enter(Ref<DebugCameraInternals> internalsRef) override;
	// 更新処理
	void Update(Ref<DebugCameraInternals> internalsRef, float elapsedTime) override;
	// 終了処理
	void Exit(Ref<DebugCameraInternals> internalsRef) override;


private:


	/* 定数 */

	// マウスの移動量倍率
	static constexpr float MOUSE_COEFFICIENT = 0.001f;


};
