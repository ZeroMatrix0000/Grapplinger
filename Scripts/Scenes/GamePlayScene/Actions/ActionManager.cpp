/*
 * FileName:     ActionManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * アクション管理
 */

#include "Pch.h"
#include "ActionManager.h"

#include "Actions.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Systems/IResources.h"

// コンストラクタ
ActionManager::ActionManager(const ComponentDesc& desc, Ref<Systems::IResources> iResources)
	: Component{ desc }
	, m_eventCount{}
	, m_eventIndex{}
	, m_event{ iResources->GetNullJson() }
	, m_currentActionRef{ GetOwner()->GetNullComponent<Action>() }
	, m_pIGameObjectFinder{}
	, m_iResources{ iResources }
{
}

// 初期化処理
void ActionManager::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_pIGameObjectFinder = iGameObjectFinder.Get();
}

// 更新処理
void ActionManager::Update(float elapsedTime, Ref<const IGameInput> iGameInput)
{
	// アクションがなければ何もしない
	if (IsCompleted())
	{
		return;
	}

	// アクションを更新
	m_currentActionRef->Update(elapsedTime, iGameInput);

	// 次のアクションに移動
	if (m_currentActionRef->IsNext())
	{
		NextAction();
	}
}

// イベントを設定
void ActionManager::LoadEvent(const std::string& eventName)
{
	m_event = m_iResources->GetJson(eventName);

	m_eventCount = m_event->size();
	m_eventIndex = 0;

	if (!IsCompleted())
	{
		InitializeCurrentAction();
	}
}

// アクション切り替え
void ActionManager::NextAction()
{
	m_currentActionRef->Exit();
	m_eventIndex++;
	if (!IsCompleted())
	{
		InitializeCurrentAction();
	}
}

// 現在のアクションを初期化
void ActionManager::InitializeCurrentAction()
{
	const auto& eventList = m_event->at(m_eventIndex);

	auto it = eventList.find("ActionType");
	if (it == eventList.end())
	{
		m_currentActionRef = GetOwner()->GetNullComponent<Action>();
	}
	else
	{
		if (it.value() == "OpenMessageWindow")
		{
			m_currentActionRef = GetOwner()->GetComponent<ActionOpenMessageWindow>().GetValue();
		}
		else if (it.value() == "CloseMessageWindow")
		{
			m_currentActionRef = GetOwner()->GetComponent<ActionCloseMessageWindow>().GetValue();
		}
		else if (it.value() == "SetMessageWindowText")
		{
			m_currentActionRef = GetOwner()->GetComponent<ActionSetMessageWindowText>().GetValue();
		}
		else
		{
			m_currentActionRef = GetOwner()->GetNullComponent<Action>();
		}
	}

	it = eventList.find("Params");
	if (it != eventList.end())
	{
		m_currentActionRef->SetParams(*it, Ref{ *m_pIGameObjectFinder });
	}

	m_currentActionRef->Enter();
}
