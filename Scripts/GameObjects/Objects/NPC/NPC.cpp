/*
 * FileName:     NPC.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * NPC
 */

#include "Pch.h"
#include "NPC.h"

#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Colliders/SphereCollider.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
NPC::NPC(const ComponentDesc& desc)
	: Component{ desc }
	, m_name{}
	, m_eventName{}
	, m_focusDistance{}
	, m_baseRotation{}
	, m_rotation{}
	, m_spinAngle{}
	, m_transformRef{ GetOwner()->GetNullComponent<Transform>() }
	, m_boxColliderRef{ GetOwner()->GetNullComponent<Colliders::BoxCollider>() }
	, m_sphereColliderRef{ GetOwner()->GetNullComponent<Colliders::SphereCollider>() }
{
}

// 初期化処理
void NPC::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_transformRef = GetOwner()->GetComponent<Transform>();
	m_boxColliderRef = GetOwner()->GetComponent<Colliders::BoxCollider>();
	m_sphereColliderRef = GetOwner()->GetComponent<Colliders::SphereCollider>();
	m_boxColliderRef->ApplyTransform();
	m_sphereColliderRef->ApplyTransform();

	m_baseRotation = m_transformRef->GetRotation();
	m_rotation.SetValue(m_baseRotation);
	m_spinAngle.SetMovement(0.0f, 0.0f);

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_name }, "Name");
	serializer.AddParameter(Ref{ m_eventName }, "EventName");
	serializer.AddParameter(Ref{ m_focusDistance }, "FocusDistance");
	serializer.Load(json);
}

// 更新処理
void NPC::Update(float elapsedTime)
{
	if (m_spinAngle.IsMoving())
	{
		m_spinAngle.Tick(elapsedTime);
	}
	else
	{
		m_rotation.Tick(elapsedTime);
	}

	m_transformRef->SetRotation(m_rotation.GetCurrent() * Math::Euler{ 0.0f, m_spinAngle.GetMovement(), 0.0f }.CreateQuaternion());
	m_boxColliderRef->ApplyTransform();
	m_sphereColliderRef->ApplyTransform();
}

// 回転を設定
void NPC::SetRotation(const Math::Quaternion* rotation)
{

	// 回転を受け取らなかったら回転を基準に戻す
	if (!rotation)
	{
		m_rotation.SetTarget(m_baseRotation);
	}
	else
	{
		m_rotation.SetTarget(*rotation);
	}
}

// スピン
void NPC::Spin(bool isClockwise)
{
	m_spinAngle.SetMovement(0.0f, isClockwise ? 720.0f : -720.0f, 1.0f, Easing::Type::Back, Easing::InOut::Out);
}

// 中心座標を取得
const Math::Vector3& NPC::GetPosition() const
{
	return m_boxColliderRef->GetWorldBox().position;
}

// 当たり判定を取得
const Math::Box& NPC::GetBox() const
{
	return m_boxColliderRef->GetWorldBox();
}

// 当たり判定を取得
const Math::Sphere& NPC::GetSphere() const
{
	return m_sphereColliderRef->GetWorldSphere();
}
