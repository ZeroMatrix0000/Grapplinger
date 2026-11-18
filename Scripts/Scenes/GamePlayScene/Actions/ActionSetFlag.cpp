/*
 * FileName:     ActionSetFlag.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * イベント分岐アクション
 */

#include "Pch.h"
#include "ActionSetFlag.h"

#include "ActionManager.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IFlagManager.h"

 // コンストラクタ
ActionSetFlag::ActionSetFlag(const ComponentDesc& desc, Ref<Systems::IFlagManager> iFlagManager)
	: Action{ desc }
	, m_flagName{}
	, m_value{}
	, m_iFlagManager{ iFlagManager }
{
}

// 初期値を設定
void ActionSetFlag::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// 初期化処理
void ActionSetFlag::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// パラメータを設定
void ActionSetFlag::SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_flagName }, "FlagName");
	serializer.AddParameter(Ref{ m_value }, "Value");
	serializer.Load(json);
}

// 開始処理
void ActionSetFlag::Enter()
{
	m_iFlagManager->GetFlagRef<bool>(m_flagName).GetValue() = m_value;
	GoNext();
}
