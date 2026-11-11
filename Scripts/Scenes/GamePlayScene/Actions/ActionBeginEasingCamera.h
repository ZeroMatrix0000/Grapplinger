/*
 * FileName:     ActionBeginEasingCamera.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメライージング開始アクション
 */

#pragma once

#include "Action.h"

class PlayerCamera;

// メッセージウィンドウ閉アクション
class ActionBeginEasingCamera : public Action
{

public:


	/* メンバ関数 */

	// コンストラクタ
	ActionBeginEasingCamera(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// パラメータを設定
	void SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 開始処理
	void Enter() override;
	// 更新処理
	void Update(float elapsedTime, Ref<const IGameInput> iGameInput) override {}


private:


	/* メンバ変数 */

	Camera::EulerTargetCamera m_value;
	float m_time;
	Easing::Type m_type;
	Easing::InOut m_inOut;

	// プレイヤーカメラの参照
	Ref<PlayerCamera> m_playerCameraRef;

};