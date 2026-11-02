/*
 * FileName:     SlimeDefeatParticle.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライム倒しパーティクル
 */

#include "Pch.h"
#include "SlimeDefeatParticle.h"

#include "Scripts/Commons/Components/Transform.h"

// コンストラクタ
SlimeDefeatParticle::SlimeDefeatParticle(Ref<Renderings::IModel3DRenderer> iModelRenderer, Ref<Systems::IResources> iResources)
	: Renderings::Particle{ iModelRenderer, iResources }
{
}

// 初期化処理
void SlimeDefeatParticle::Initalize(Ref<const Transform> transformRef)
{
	for (int i = 0; i < ELEMENT_COUNT; i++)
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
		pUpdateElement->scale = Math::Vector3::One * 0.25f;

		// モデルを設定
		SetModelSource("Box", Ref{ *pRenderElement });
		pRenderElement->color = Math::Color{ 0.0f, 0.760694f, 0.025336f, 1.0f };

		// 方向
		Math::Vector3 direction = Math::Vector3::Transform
		(
			Math::Vector3::Forward * Random::GetFloatRange(0.8f, 1.0f),
			Math::Quaternion::CreateFromAxisAngle
			(
				Math::Vector3::Up,
				Math::Deg2Rad(i * 360.0f / ELEMENT_COUNT + Random::GetFloatRange(-10.0f, 10.0f))
			)
		);
		direction *= 8.0f;
		direction.y = -FALL_SPEED;
		m_directions.push_back(direction);

		// 回転方向
		m_rotateDirections.push_back(Math::Vector3
			{
				Random::GetFloatRange(-1.0f, 1.0f),
				Random::GetFloatRange(-1.0f, 1.0f),
				Random::GetFloatRange(-1.0f, 1.0f)
			});
	}
	ApplyTransform();

	m_scale.SetMovement(Math::Vector3::One * 0.5f, Math::Vector3::Zero, 0.75f, Easing::Type::Sine, Easing::InOut::In);
}

// 更新処理
void SlimeDefeatParticle::Update(float elapsedTime)
{
	// 移動が終わったら消す
	if (!m_scale.IsMoving())
	{
		Destroy();
		return;
	}

	// 大きさの更新
	m_scale.Tick(elapsedTime);

	for (int i = 0; i < ELEMENT_COUNT; i++)
	{
		// 移動
		m_directions.at(i).y -= GRAVITY * elapsedTime;
		GetUpdateElementRef(i)->position += m_directions.at(i) * elapsedTime;
		// 回転
		GetUpdateElementRef(i)->rotation *= Math::Quaternion::CreateFromAxisAngle
		(
			m_rotateDirections.at(i),
			m_rotateDirections.at(i).Length() * Math::PI * 2.0f * elapsedTime
		);
		// 拡大
		GetUpdateElementRef(i)->scale = m_scale.GetMovement();
	}

	ApplyTransform();
}
