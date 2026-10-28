/*
 * FileName:     PlayerInternals.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * プレイヤーの内部データ
 */

#include "Pch.h"
#include "PlayerInternals.h"

#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/GameObjects/GameObject.h"

// コンストラクタ
PlayerInternals::PlayerInternals(Ref<GameObject> owner)
	: MOVE_ACCELERATION{}
	, DASH_RATIO{}
	, FALL_ACCELERATION{}
	, JUMP_POWER{}
	, CAMERA_VELOCITY_COEFFICIENT{}
	, CAMERA_SCREEN_REF{ owner->GetNullComponent<Renderings::CameraScreen<Camera::EulerTargetCamera>>().Const() }
	, MOVE{}
	, IS_DASH{}
	, IS_JUMP{}
	, moveVelocity{}
	, moveMaxSpeed{}
	, fallSpeed{}
	, rotation{}
	, fallCoyoteTime{}
	, jumpBufferTime{}
	, canExpandWire{}
	, transformRef{ owner->GetNullComponent<Transform>() }
	, boxColliderRef{ owner->GetNullComponent<Colliders::BoxCollider>() }
	, particleManagerRef{ owner->GetNullComponent<Renderings::ParticleManager>() }
	, modelRef{ owner->GetNullComponent<PlayerModel>() }
	, wireRef{ owner->GetNullComponent<PlayerWire>() }
{
}
