/*
 * FileName:     ActionWaitUntilEasingCamera.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメライージング終了アクション
 */

#include "Pch.h"
#include "ActionWaitUntilEasingCamera.h"

#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

 // コンストラクタ
ActionWaitUntilEasingCamera::ActionWaitUntilEasingCamera(const ComponentDesc& desc)
	: Action{ desc }
	, m_playerCameraRef{ GetOwner()->GetNullComponent<PlayerCamera>() }
{
}

// 初期値を設定
void ActionWaitUntilEasingCamera::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_playerCameraRef.Nest(), "PlayerCamera");
	serializer.Load(json);
}

// 初期化処理
void ActionWaitUntilEasingCamera::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// パラメータを設定
void ActionWaitUntilEasingCamera::SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// 開始処理
void ActionWaitUntilEasingCamera::Enter()
{
}

// 更新処理
void ActionWaitUntilEasingCamera::Update(float elapsedTime, Ref<const IGameInput> iGameInput)
{
	if (!m_playerCameraRef->IsEasing())
	{
		GoNext();
	}
}
