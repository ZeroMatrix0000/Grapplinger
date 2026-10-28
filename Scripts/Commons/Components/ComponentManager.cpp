/*
 * FileName:     ComponentManager.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * コンポーネント管理
 */

#include "Pch.h"
#include "ComponentManager.h"

#include "../Systems/IErrorMessage.h"
#include "ComponentDesc.h"
#include "../GameObjects/IGameObjectInstantiator.h"

// コンストラクタ
Components::ComponentManager::ComponentManager(Ref<IGameObjectInstantiator> iGameObjectInstantiator)
	: IComponentManager{}
	, m_permit{}
	, m_CreateComponentList{}
	, m_nullComponents{}
	, m_iGameObjectInstantiator{ iGameObjectInstantiator }
{
}

// コンポーネントを作成
std::unique_ptr<Component> Components::ComponentManager::Create(const std::type_index& index, Ref<GameObject> owner)
{
	return m_CreateComponentList.at(index)(ComponentDesc{ owner, m_iGameObjectInstantiator, m_permit });
}

// 未参照コンポーネントを取得
Ref<Component> Components::ComponentManager::GetNullComponent(const std::type_index& index, bool addErrorMessage)
{
	if (addErrorMessage)
	{
		// エラーメッセージを追加
		Systems::IErrorMessage::GetInstance()->AddMessage(Utility::FormatWString
		(
			L"コンポーネントが見つかりません。 | %s",
			Utility::string2wstring(index.name()).c_str()
		));
	}

	auto it = m_nullComponents.find(index);
	if (it == m_nullComponents.end())
	{
		// コンポーネントを追加
		m_nullComponents.emplace(index, m_CreateComponentList.at(index)(ComponentDesc
			{
				m_iGameObjectInstantiator->GetNullGameObject(),
				Ref{ m_iGameObjectInstantiator },
				m_permit
			}));
		return Ref{ *m_nullComponents.at(index) };
	}
	else
	{
		return Ref{ *it->second };
	}
}
