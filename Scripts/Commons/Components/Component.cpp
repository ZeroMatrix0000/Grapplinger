/*
 * FileName:     Component.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * コンポーネント
 */

#include "Pch.h"
#include "Component.h"

#include "ComponentDesc.h"
#include "../GameObjects/IGameObjectInstantiator.h"

// コンストラクタ
Components::Component::Component(const ComponentDesc& desc)
	: m_owner{ desc.owner }
	, m_iGameObjectInstantiator{ desc.iGameObjectInstantiator }
{
}

// ゲームオブジェクトを生成
Ref<GameObject> Components::Component::Instantiate(const std::string& name) const
{
	return m_iGameObjectInstantiator->Instantiate(name);
}

// 未参照ゲームオブジェクトを取得
Ref<GameObject> Components::Component::GetNullGameObject() const
{
	return m_iGameObjectInstantiator->GetNullGameObject();
}

// ゲームオブジェクトを破棄
void Components::Component::Destroy(Ref<GameObject> owner) const
{
	m_iGameObjectInstantiator->Destroy(owner);
}
