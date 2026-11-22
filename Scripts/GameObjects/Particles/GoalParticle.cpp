/*
 * FileName:     GoalParticle.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * ゴールパーティクル
 */

#include "Pch.h"
#include "GoalParticle.h"

#include "Scripts/Commons/Components/Transform.h"

 // コンストラクタ
GoalParticle::GoalParticle(Ref<Systems::IResources> iResources)
	: Particle{ iResources }
	, m_position{}
	, m_scale{}
{
}

// 初期化処理
void GoalParticle::Initalize(Ref<const Transform> transformRef)
{
	// 更新情報
	UpdateElement* pUpdateElement{};
	// 描画情報
	RenderElement* pRenderElement{};
	AddElement(&pUpdateElement, &pRenderElement);

	// 管理者の位置に移動
	pUpdateElement->position = transformRef->GetPosition();
	// ランダムに回転
	pUpdateElement->rotation = Math::Euler
	{
		Random::GetFloatRange(0.0f, 360.0f),
		Random::GetFloatRange(0.0f, 360.0f),
		Random::GetFloatRange(0.0f, 360.0f)
	}.CreateQuaternion();
	// 拡大
	pUpdateElement->scale = Math::Vector3::One * 0.2f;

	// モデルを設定
	SetModelSource("Box", Ref{ *pRenderElement });

	// 方向
	Math::Vector3 direction
	{
		Random::GetFloatRange(-1.0f, 1.0f),
		Random::GetFloatRange(-1.0f, 1.0f),
		Random::GetFloatRange(-1.0f, 1.0f)
	};
	direction.Normalize();
	direction *= 2.0f;

	ApplyTransform();

	m_position.SetMovement(pUpdateElement->position, pUpdateElement->position + direction, 0.75f, Easing::Type::Sine, Easing::InOut::Out);
	m_scale.SetMovement(Math::Vector3::One * 0.2f, Math::Vector3::Zero, 0.75f, Easing::Type::Sine, Easing::InOut::In);
}

// 更新処理
void GoalParticle::Update(float elapsedTime)
{
	// 移動が終わったら消す
	if (!m_position.IsMoving())
	{
		Destroy();
		return;
	}

	// 位置の更新
	m_position.Tick(elapsedTime);
	// 大きさの更新
	m_scale.Tick(elapsedTime);

	// 移動
	GetUpdateElementRef(0)->position = m_position.GetMovement();
	// 拡大
	GetUpdateElementRef(0)->scale = m_scale.GetMovement();

	ApplyTransform();
}
