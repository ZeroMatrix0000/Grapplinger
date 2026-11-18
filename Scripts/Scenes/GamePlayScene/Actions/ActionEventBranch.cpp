/*
 * FileName:     ActionEventBranch.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * イベント分岐アクション
 */

#include "Pch.h"
#include "ActionEventBranch.h"

#include "ActionManager.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IFlagManager.h"

 // コンストラクタ
ActionEventBranch::ActionEventBranch(const ComponentDesc& desc, Ref<Systems::IFlagManager> iFlagManager)
	: Action{ desc }
	, m_flagName{}
	, m_falseEventName{}
	, m_trueEventName{}
	, m_iFlagManager{ iFlagManager }
	, m_actionManager{ GetOwner()->GetNullComponent<ActionManager>() }
{
}

// 初期値を設定
void ActionEventBranch::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// 初期化処理
void ActionEventBranch::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_actionManager = GetOwner()->GetComponent<ActionManager>();
}

// パラメータを設定
void ActionEventBranch::SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_flagName }, "FlagName");
	serializer.AddParameter(Ref{ m_falseEventName }, "FalseEvent");
	serializer.AddParameter(Ref{ m_trueEventName }, "TrueEvent");
	serializer.Load(json);
}

// 開始処理
void ActionEventBranch::Enter()
{
	if (m_iFlagManager->GetFlag<bool>(m_flagName))
	{
		m_actionManager->SetNextEvent(m_trueEventName);
	}
	else
	{
		m_actionManager->SetNextEvent(m_falseEventName);
	}
}
