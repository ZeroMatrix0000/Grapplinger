/*
 * FileName:     DebugCameraStateIdle.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * デバッグ用カメラの待機状態
 */

#include "Pch.h"
#include "DebugCameraStateIdle.h"

#include "DebugCameraStateRotate.h"
#include "DebugCameraStateMove.h"

// コンストラクタ
DebugCameraStateIdle::DebugCameraStateIdle()
{
}

// 開始処理
void DebugCameraStateIdle::Enter(Ref<DebugCameraInternals> internalsRef)
{
}

// 更新処理
void DebugCameraStateIdle::Update(Ref<DebugCameraInternals> internalsRef, float elapsedTime)
{
	// カメラ
	Camera::EulerTargetCamera camera = internalsRef->cameraScreenRef->GetCamera();

	// マウスホイールでカメラの距離を変更
	float ratio = Math::Pow(WHEEL_COEFFICIENT, static_cast<float>(-internalsRef->mouseWheelDelta));
	camera.distance *= ratio;
	if (camera.distance < Math::EPSILON)
	{
		camera.distance = Math::EPSILON;
	}

	internalsRef->cameraScreenRef->SetCamera(camera);

	// 右クリックで回転状態に遷移
	if (internalsRef->mouseButtonRight)
	{
		SetNextState(std::make_unique<DebugCameraStateRotate>());
	}
	// 中クリックで移動状態に遷移
	if (internalsRef->mouseButtonMiddle)
	{
		SetNextState(std::make_unique<DebugCameraStateMove>());
	}
}

// 終了処理
void DebugCameraStateIdle::Exit(Ref<DebugCameraInternals> internalsRef)
{
}
