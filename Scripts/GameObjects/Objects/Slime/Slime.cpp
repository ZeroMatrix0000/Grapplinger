/*
 * FileName:     Slime.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * スライム
 */

#include "Pch.h"
#include "Slime.h"

#include "States/SlimeStates.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Colliders/SphereCollider.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
Slime::Slime(const ComponentDesc& desc)
	: Component{ desc }
	, m_internals{ GetOwner() }
	, m_currentState{}
	, m_iCameraScreenRef{ GetOwner()->GetNullComponent<Renderings::ICameraScreen>().Const() }
{
	m_currentState = std::make_unique<SlimeStateIdle>();
}

// 初期値を設定
void Slime::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_iCameraScreenRef.Nest(), "CameraScreen");
	serializer.Load(json);
}

// 初期化処理
void Slime::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_internals.transformRef = GetOwner()->GetComponent<Transform>();
	m_internals.boxColliderRef = GetOwner()->GetComponent<Colliders::BoxCollider>();
	m_internals.sphereColliderRef = GetOwner()->GetComponent<Colliders::SphereCollider>();
	m_internals.boxColliderRef->ApplyTransform();
	m_internals.sphereColliderRef->ApplyTransform();

	// カメラを追加
	GetOwner()->GetComponent<Renderings::Model3D>()->AddICameraScreenRef(m_iCameraScreenRef);
	m_internals.boxColliderRef->AddICameraScreenRef(m_iCameraScreenRef);
	m_internals.sphereColliderRef->AddICameraScreenRef(m_iCameraScreenRef);

	// スライム管理に追加
	iGameObjectFinder->Find("SlimeManager")->AcceptMessage("AddSlime", Ref{ *this });
	// ターゲット管理に追加
	iGameObjectFinder->Find("TargetPoint")->AcceptMessage("AddTargetObject", m_internals.sphereColliderRef);
}

// 更新処理
void Slime::Update(float elapsedTime)
{
	// 状態ごとの処理
	m_currentState->Update(Ref{ m_internals }, elapsedTime);
	// 次の状態に遷移
	std::unique_ptr<Systems::State<SlimeInternals>> nextState{ m_currentState->MoveNextState() };
	if (nextState)
	{
		SetState(std::move(nextState));
	}
}

// 当たり判定を取得
const Math::Box& Slime::GetBox() const
{
	return m_internals.boxColliderRef->GetWorldBox();
}

// 状態を変更
void Slime::SetState(std::unique_ptr<Systems::State<SlimeInternals>> state)
{
	if (m_currentState != nullptr)
	{
		m_currentState->Exit(Ref{ m_internals });
	}
	m_currentState = std::move(state);
	m_currentState->Enter(Ref{ m_internals });
}
