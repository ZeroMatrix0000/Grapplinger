/*
 * FileName:     CheckPointManager.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * チェックポイント管理
 */

#include "Pch.h"
#include "CheckPointManager.h"

#include "CheckPoint.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
CheckPointManager::CheckPointManager(const ComponentDesc& desc)
	: Component{ desc }
	, m_iCameraScreenRef{ GetOwner()->GetNullComponent<Renderings::ICameraScreen>().Const() }
	, m_checkPointRefs{}
{
}

// 初期値を設定
void CheckPointManager::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_iCameraScreenRef.Nest(), "CameraScreen");
	serializer.Load(json);
}

// 初期化処理
void CheckPointManager::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// チェック
int CheckPointManager::Check(const Math::Box& player)
{
	int i = 0;
	for (Ref checkPointRef : m_checkPointRefs)
	{
		if (Math::Geometry::IsCollide(player, checkPointRef->GetBox()))
		{
			return i;
		}
		i++;
	}
	return -1;
}

// 座標を取得
const Math::Vector3& CheckPointManager::GetPosition(int index)
{
	return m_checkPointRefs.at(index)->GetPosition();
}

// メッセージを受け取る
void CheckPointManager::AcceptMessage(const std::string& message, const std::any& value)
{
	if (message == "AddCheckPoint")
	{
		Ref checkPointRef = std::any_cast<Ref<const CheckPoint>>(value);
		checkPointRef->AddICameraScreenRef(m_iCameraScreenRef);
		m_checkPointRefs.push_back(checkPointRef);
		std::ranges::sort(m_checkPointRefs, [](Ref<const CheckPoint> a, Ref<const CheckPoint> b) {return a->GetOrder() < b->GetOrder(); });
	}
}
