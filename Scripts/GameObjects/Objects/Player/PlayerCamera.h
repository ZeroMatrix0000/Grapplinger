/*
 * FileName:     PlayerCamera.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * プレイヤーカメラ
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"
#include "Scripts/Commons/Renderings/CameraScreen.h"

 // デバッグ用カメラ
class PlayerCamera : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerCamera(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 回転
	void Rotate(const Math::Vector2& rotate);
	// メッシュによる座標補正
	void MeshCorrect(const Mesh& mesh, const Math::Vector3& centerPosition);
	// 直方体による座標補正
	void BoxCorrect(const Math::Box& box, const Math::Vector3& centerPosition);

	// ターゲットの位置を変更
	void SetTarget(const Math::Vector3& target);

	// カメラを移動
	void SetPosition(const Math::Vector3& position);

	// カメラを固定
	void FixEyePosition();


private:


	/* メンバ変数 */

	// ターゲットの位置
	Math::Vector3 m_target;
	// Y軸回転
	Cycled m_rotationYDeg;
	// X軸回転
	Limited m_rotationXDeg;
	// カメラの距離
	float m_distance;
	// カメラの最大距離
	float m_maxDistance;

	// 目線を固定しているか
	bool m_isFixed;
	// 目線の位置
	Math::Vector3 m_eyePosition;

	// カメラ
	Spring<Camera::EulerTargetCamera> m_camera;

	// カメラ画面の参照
	Ref<Renderings::CameraScreen<Camera::EulerTargetCamera>> m_cameraScreenRef;

};
