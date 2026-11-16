/*
 * FileName:     PauseScreen.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/16
 *
 * ポーズ画面
 */

#include "Pch.h"
#include "PauseScreen.h"

#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
PauseScreen::PauseScreen(const ComponentDesc& desc)
	: Component{ desc }
	, m_isPausing{}
	, m_sideBeltWidth{}
	, m_backGround{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_sideBelt{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_sideBeltImage{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>() }
{
}

// 初期値を設定
void PauseScreen::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_backGround.Nest(), "BackGround");
	serializer.AddParameter(m_canvasRef.Nest(), "Canvas");
	serializer.Load(json);
}

// 初期化処理
void PauseScreen::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_backGround->SetPivot(Utility::AlignmentPoint::MiddleLeft);
	m_backGround->SetAnchor(Utility::AlignmentPoint::MiddleRight);

	// 左帯を作成
	Ref obj = Instantiate("Prefab_PauseScreenSideBelt");
	m_sideBelt = obj->GetComponent<RectTransform>();
	m_sideBeltImage = obj->GetComponent<Renderings::Image>();
	m_sideBeltImage->SetCanvasRef(m_canvasRef.Const());

	m_sideBeltWidth = m_sideBelt->GetRect().size.x;
	m_sideBeltPosition.SetMovement(Math::Vector2{ -m_sideBeltWidth, 0.0f }, 0.0f);
	m_sideBelt->SetPosition(Math::Vector2{ -m_sideBeltWidth, 0.0f });
}

// 更新処理
void PauseScreen::Update(float elapsedTime)
{
	m_sideBeltPosition.Tick(elapsedTime);
	m_sideBelt->SetPosition(m_sideBeltPosition.GetMovement());

	Ref bufferRef = m_sideBeltImage->GetPixelShader()->GetConstantBufferRef();
	bufferRef->SetVariable("Time", bufferRef->GetVariable<float>("Time") + elapsedTime);
	bufferRef->SetVariable("TextureSize", m_sideBelt->GetRect().size);
}

// キャンバスサイズを設定
void PauseScreen::SetCanvasSize(const Math::Vector2& outputSize)
{
	m_backGround->SetSize(m_canvasRef->GetSize());
	m_sideBelt->SetSize(Math::Vector2{ m_sideBeltWidth, m_canvasRef->GetSize().y });
}

// ポーズする
void PauseScreen::Pause(bool isPause)
{
	m_isPausing = isPause;
	m_sideBeltPosition.SetMovement(Math::Vector2{ isPause ? 0.0f : -m_sideBeltWidth, 0.0f }, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_backGround->SetPivot(isPause ? Utility::AlignmentPoint::MiddleCenter : Utility::AlignmentPoint::MiddleLeft);
	m_backGround->SetAnchor(isPause ? Utility::AlignmentPoint::MiddleCenter : Utility::AlignmentPoint::MiddleRight);
}
