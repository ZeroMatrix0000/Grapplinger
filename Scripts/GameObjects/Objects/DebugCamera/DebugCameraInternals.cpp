/*
 * FileName:     DebugCameraInternals.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * デバッグ用カメラの内部データ
 */

#include "Pch.h"
#include "DebugCameraInternals.h"

#include "Scripts/Main/IsDerivedRegister.h"

// コンストラクタ
DebugCameraInternals::DebugCameraInternals(Ref<GameObject> owner)
	: mouseMovement{}
	, mouseButtonRight{}
	, mouseButtonMiddle{}
	, mouseWheelDelta{}
	, cameraScreenRef{ owner->GetNullComponent<Renderings::CameraScreen<Camera::EulerTargetCamera>>() }
{
}
