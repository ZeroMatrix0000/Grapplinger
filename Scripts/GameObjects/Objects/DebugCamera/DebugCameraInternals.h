/*
 * FileName:     DebugCameraInternals.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * デバッグ用カメラの内部データ
 */

#pragma once

#include "Scripts/Commons/Renderings/CameraScreen.h"

// デバッグ用カメラの内部データ
struct DebugCameraInternals
{

	/* メンバ関数 */

	// コンストラクタ
	DebugCameraInternals(Ref<GameObject> owner);


	/* メンバ変数 */

	// マウスの移動量
	Math::Vector2Int mouseMovement;
	// 右クリックの状態
	bool mouseButtonRight;
	// 中クリックの状態
	bool mouseButtonMiddle;
	// マウスホイールの回転量
	int mouseWheelDelta;

	// カメラ画面の参照
	Ref<Renderings::CameraScreen<Camera::EulerTargetCamera>> cameraScreenRef;

};
