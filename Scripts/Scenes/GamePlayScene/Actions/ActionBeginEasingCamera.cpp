/*
 * FileName:     ActionBeginEasingCamera.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメライージング開始アクション
 */

#include "Pch.h"
#include "ActionBeginEasingCamera.h"

#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
ActionBeginEasingCamera::ActionBeginEasingCamera(const ComponentDesc& desc)
	: Action{ desc }
	, m_value{}
	, m_time{}
	, m_type{}
	, m_inOut{}
	, m_playerCameraRef{ GetOwner()->GetNullComponent<PlayerCamera>() }
{
}

// 初期値を設定
void ActionBeginEasingCamera::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_playerCameraRef.Nest(), "PlayerCamera");
	serializer.Load(json);
}

// 初期化処理
void ActionBeginEasingCamera::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// パラメータを設定
void ActionBeginEasingCamera::SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_value }, "Value");
	serializer.AddParameter(Ref{ m_time }, "Time");
	serializer.AddParameter(Ref{ m_type }, "Type");
	serializer.AddParameter(Ref{ m_inOut }, "InOut");
	serializer.Load(json);
}

// 開始処理
void ActionBeginEasingCamera::Enter()
{
	// イージング開始
	m_playerCameraRef->BeginEasing(m_value, m_time, m_type, m_inOut);
	GoNext();
}
