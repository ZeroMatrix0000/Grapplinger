/*
 * FileName:     PlayerInternals.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * プレイヤーの内部データ
 */

#pragma once

#include "Scripts/Commons/Renderings/CameraScreen.h"

namespace Components
{
	class Transform;
}
namespace Colliders
{
	class BoxCollider;
}
namespace Renderings
{
	class ParticleManager;
}

class PlayerModel;
class PlayerWire;

// プレイヤーの内部データ
struct PlayerInternals
{

	/* 列挙型 */

	// 落下状態
	enum class FallState
	{
		OnGround,
		OnAir,
		Falling
	};


	/* 列挙型 */

	// ターゲットの種類
	enum class TargetType
	{
		Object,
		Ground,
		Air
	};


	/* メンバ関数 */

	// コンストラクタ
	PlayerInternals(Ref<GameObject> owner);


	/* メンバ変数 */

	// 移動加速度
	float MOVE_ACCELERATION;
	// ダッシュ時の速度倍率
	float DASH_RATIO;

	// 落下加速度
	float FALL_ACCELERATION;

	// ジャンプ力
	float JUMP_POWER;

	// カメラの速度の参照量
	float CAMERA_VELOCITY_COEFFICIENT;

	// カメラ画面の参照
	Ref<const Renderings::CameraScreen<Camera::EulerTargetCamera>> CAMERA_SCREEN_REF;

	// 移動入力
	Math::Vector3 MOVE;
	// ダッシュ入力
	bool IS_DASH;
	// ジャンプ入力入力
	bool IS_JUMP;

	// 移動速度
	Math::Vector3 moveVelocity;
	// 移動の速さの最大
	Limited moveMaxSpeed;
	// 落下速度
	Limited fallSpeed;

	// プレイヤーの回転
	Spring<Math::Quaternion> rotation;

	// 落下猶予
	Limited fallCoyoteTime;
	// ジャンプの先行入力猶予
	Limited jumpBufferTime;

	// 糸を伸ばせるかどうか
	bool canExpandWire;

	// トランスフォームの参照
	Ref<Transform> transformRef;
	// 長方形の当たり判定の参照
	Ref<Colliders::BoxCollider> boxColliderRef;
	// パーティクル管理の参照
	Ref<Renderings::ParticleManager> particleManagerRef;

	// プレイヤーのモデルの参照
	Ref<PlayerModel> modelRef;

	// プレイヤーの糸の参照
	Ref<PlayerWire> wireRef;

};
