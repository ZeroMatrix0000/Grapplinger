/*
 * FileName:     GoalParticle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * ゴールパーティクル
 */

#pragma once

#include "Scripts/Commons/Renderings/Particle/Particle.h"

// ゴールパーティクル
class GoalParticle : public Renderings::Particle
{

public:


	/* メンバ関数 */

	// コンストラクタ
	GoalParticle(Ref<Systems::IResources> iResources);

	// 初期化処理
	void Initalize(Ref<const Transform> transformRef) override;

	// 更新処理
	void Update(float elapsedTime) override;


private:


	/* メンバ変数 */

	// 速さ
	Easing::Value<Math::Vector3> m_position;
	// 大きさ
	Easing::Value<Math::Vector3> m_scale;

};
