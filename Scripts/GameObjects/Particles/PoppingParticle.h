/*
 * FileName:     PoppingParticle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * 弾けパーティクル
 */

#pragma once

#include "Scripts/Commons/Renderings/Particle/Particle.h"

 // 弾けパーティクル
class PoppingParticle : public Renderings::Particle
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PoppingParticle(Ref<Renderings::IModel3DRenderer> iModelRendererRef, Ref<Systems::IResources> iResourcesRef);

	// 初期化処理
	void Initalize(Ref<const Transform> transformRef) override;

	// 更新処理
	void Update(float elapsedTime) override;


private:


	/* 定数 */

	// 要素数
	static constexpr int ELEMENT_COUNT = 24;


	/* メンバ変数 */

	// 方向
	std::vector<Math::Vector3> m_directions;
	// 速さ
	Easing::Value<float> m_speed;
	// 大きさ
	Easing::Value<Math::Vector3> m_scale;

};
