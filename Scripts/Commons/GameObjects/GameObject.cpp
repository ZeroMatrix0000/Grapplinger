/*
 * FileName:     GameObject.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * ゲームオブジェクト
 */

#include "Pch.h"
#include "GameObject.h"

#include "../Components/Component.h"

// コンストラクタ
GameObjects::GameObject::GameObject(Ref<IComponentManager> iComponentManager)
	: m_name{}
	, m_components{}
	, m_componentRefs{}
	, m_iComponentManager{ iComponentManager }
{
}

// 初期化処理
void GameObjects::GameObject::Initialize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	for (const auto& component : m_components)
	{
		component.first->Initalize(iGameObjectFinder);
	}
}

// メッセージを受け取る
void GameObjects::GameObject::AcceptMessage(const std::string& message) const
{
	for (const auto& component : m_components)
	{
		component.first->AcceptMessage(message);
	}
}

// ゲームオブジェクトを破棄
void GameObjects::GameObject::Destroy() const
{
	for (const auto& Method : m_MethodsWhenDestroy)
	{
		Method();
	}
}

// 破棄時の処理を追加
void GameObjects::GameObject::AddMethodWhenDestroy(const std::function<void()>& Method)
{
	m_MethodsWhenDestroy.emplace_back(Method);
}
