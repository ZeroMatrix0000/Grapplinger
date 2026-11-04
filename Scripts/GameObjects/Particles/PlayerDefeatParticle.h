/*
 * FileName:     PlayerDefeatParticle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/04
 *
 * プレイヤー敗北パーティクル
 */

#pragma once

#include "Scripts/Commons/Renderings/Particle/Particle.h"

 // プレイヤー敗北パーティクル
class PlayerDefeatParticle : public Renderings::Particle
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerDefeatParticle(Ref<Renderings::IModel3DRenderer> iModelRenderer, Ref<Systems::IResources> iResources);

	// 初期化処理
	void Initalize(Ref<const Transform> transformRef) override;

	// 更新処理
	void Update(float elapsedTime) override;


private:


	/* 定数 */

	// 要素数
	static constexpr int ELEMENT_COUNT = 8;

	// 落下速度の初期値
	static constexpr float FALL_SPEED = -6.0f;
	// 重力加速度
	static constexpr float GRAVITY = 24.0f;


	/* メンバ変数 */

	// 方向
	std::vector<Math::Vector3> m_directions;
	// 回転方向
	std::vector<Math::Vector3> m_rotateDirections;
	// 回転速度
	std::vector<float> m_rotateVelocities;
	// 大きさ
	Easing::Value<Math::Vector3> m_scale;

};
