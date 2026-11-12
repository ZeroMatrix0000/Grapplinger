/*
 * FileName:     CheckPoint.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * チェックポイント
 */

#include "Pch.h"
#include "CheckPoint.h"

#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
CheckPoint::CheckPoint(const ComponentDesc& desc)
	: Component{ desc }
	, m_order{}
	, m_boxColliderRef{ GetOwner()->GetNullComponent<Colliders::BoxCollider>() }
{
}

// 初期値を設定
void CheckPoint::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_order }, "Order");
	serializer.Load(json);
}

// 初期化処理
void CheckPoint::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_boxColliderRef = GetOwner()->GetComponent<Colliders::BoxCollider>();
	m_boxColliderRef->ApplyTransform();

	// ターゲット管理管理に追加
	iGameObjectFinder->Find("CheckPointManager")->AcceptMessage("AddCheckPoint", Ref{ *this }.Const());
}

// カメラ画面の参照を追加
void CheckPoint::AddICameraScreenRef(Ref<const Renderings::ICameraScreen> iCameraScreenRef) const
{
	m_boxColliderRef->AddICameraScreenRef(iCameraScreenRef);
}

// 当たり判定を取得
const Math::Box& CheckPoint::GetBox() const
{
	return m_boxColliderRef->GetWorldBox();
}

// 座標を取得
const Math::Vector3& CheckPoint::GetPosition() const
{
	return GetConstOwner()->GetConstComponent<Transform>()->GetPosition();
}
