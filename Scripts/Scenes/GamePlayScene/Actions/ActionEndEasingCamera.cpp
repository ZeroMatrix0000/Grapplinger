/*
 * FileName:     ActionEndEasingCamera.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメライージング終了アクション
 */

#include "Pch.h"
#include "ActionEndEasingCamera.h"

#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

 // コンストラクタ
ActionEndEasingCamera::ActionEndEasingCamera(const ComponentDesc& desc)
	: Action{ desc }
	, m_playerCameraRef{ GetOwner()->GetNullComponent<PlayerCamera>() }
{
}

// 初期値を設定
void ActionEndEasingCamera::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_playerCameraRef.Nest(), "PlayerCamera");
	serializer.Load(json);
}

// 初期化処理
void ActionEndEasingCamera::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// パラメータを設定
void ActionEndEasingCamera::SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
}

// 開始処理
void ActionEndEasingCamera::Enter()
{
	// イージング開始
	m_playerCameraRef->EndEasing();
	GoNext();
}
