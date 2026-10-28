/*
 * FileName:     DebugCameraStateMove.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/07/08
 *
 * デバッグ用カメラの待機状態
 */

#include "Pch.h"
#include "DebugCameraStateMove.h"

#include "DebugCameraStateIdle.h"

 // コンストラクタ
DebugCameraStateMove::DebugCameraStateMove()
{
}

// 開始処理
void DebugCameraStateMove::Enter(Ref<DebugCameraInternals> internalsRef)
{
}

// 更新処理
void DebugCameraStateMove::Update(Ref<DebugCameraInternals> internalsRef, float elapsedTime)
{
	// カメラ
	Camera::EulerTargetCamera camera = internalsRef->cameraScreenRef->GetCamera();

	// マウスの移動量の移動量
	Math::Vector2 mouseDelta = internalsRef->mouseMovement * MOUSE_COEFFICIENT * camera.distance;
	// カメラの移動
	Math::Vector3 movement = Math::Vector3::Transform
	(
		Math::Vector3{ -mouseDelta.x, mouseDelta.y, 0.0f },
		camera.rotation.CreateQuaternion()
	);
	camera.position += movement;

	internalsRef->cameraScreenRef->SetCamera(camera);

	// 中クリックを離したら待機状態に戻る
	if (!internalsRef->mouseButtonMiddle)
	{
		SetNextState(std::make_unique<DebugCameraStateIdle>());
	}
}

// 終了処理
void DebugCameraStateMove::Exit(Ref<DebugCameraInternals> internalsRef)
{
}
