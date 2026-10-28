/*
 * FileName:     DebugCameraStateRotate.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/07/08
 *
 * デバッグ用カメラの待機状態
 */

#include "Pch.h"
#include "DebugCameraStateRotate.h"

#include "DebugCameraStateIdle.h"

 // コンストラクタ
DebugCameraStateRotate::DebugCameraStateRotate()
{
}

// 開始処理
void DebugCameraStateRotate::Enter(Ref<DebugCameraInternals> internalsRef)
{
}

// 更新処理
void DebugCameraStateRotate::Update(Ref<DebugCameraInternals> internalsRef, float elapsedTime)
{
	// カメラ
	Camera::EulerTargetCamera camera = internalsRef->cameraScreenRef->GetCamera();

	// 回転量
	Math::Vector2 rotate = internalsRef->mouseMovement * MOUSE_COEFFICIENT;

	camera.rotation.y -= rotate.x;
	camera.rotation.x -= rotate.y;

	internalsRef->cameraScreenRef->SetCamera(camera);

	// 右クリックを離したら待機状態に戻る
	if (!internalsRef->mouseButtonRight)
	{
		SetNextState(std::make_unique<DebugCameraStateIdle>());
	}
}

// 終了処理
void DebugCameraStateRotate::Exit(Ref<DebugCameraInternals> internalsRef)
{
}
