/*
 * FileName:     PauseScreen.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/17
 *
 * ポーズ画面
 */

#include "Pch.h"
#include "PauseScreen.h"

#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Text/Text.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
PauseScreen::PauseScreen(const ComponentDesc& desc)
	: Component{ desc }
	, m_menuInterval{}
	, m_menuBasePosition{}
	, m_menuBarBaseSize{}
	, m_menuBaseFontSize{}
	, m_isPausing{}
	, m_selectNumber{}
	, m_canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>() }
	, m_backGround{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_sideBelt{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_sideBeltImage{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_sideBeltWidth{}
	, m_sideBeltPosition{}
	, m_title{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_menuList{}
	, m_menuBasePositionList{}
	, m_menuBarList{}
	, m_menuBarSizeList{}
	, m_menuBarBasePositionList{}
	, m_menuTextList{}
	, m_menuFontSizeList{}
	, m_menuOnDecideList{}
{
}

// 初期値を設定
void PauseScreen::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_menuInterval }, "MenuInterval");
	serializer.AddParameter(Ref{ m_menuBasePosition }, "MenuBasePosition");
	serializer.AddParameter(Ref{ m_menuBarBaseSize }, "MenuBarBaseSize");
	serializer.AddParameter(Ref{ m_menuBaseFontSize }, "MenuBaseFontSize");
	serializer.AddParameter(m_canvasRef.Nest(), "Canvas");
	serializer.AddParameter(m_backGround.Nest(), "BackGround");
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

	// タイトルを作成
	obj = Instantiate("Prefab_PauseScreenTitle");
	m_title = obj->GetComponent<RectTransform>();
	obj->GetComponent<Renderings::Text>()->SetCanvasRef(m_canvasRef.Const());
	m_title->SetPosition(Math::Vector2{ -m_sideBeltWidth, 0.0f });
}

// 更新処理
void PauseScreen::Update(float elapsedTime)
{
	m_sideBeltPosition.Tick(elapsedTime);

	Math::Vector2 position = m_sideBeltPosition.GetMovement();

	m_sideBelt->SetPosition(position);
	m_title->SetPosition(position);

	for (int i = 0; i < m_menuList.size(); i++)
	{
		m_menuList.at(i)->SetPosition(position + m_menuBasePositionList.at(i));
		m_menuBarList.at(i)->SetPosition(position + m_menuBarBasePositionList.at(i));
		m_menuBarSizeList.at(i).Tick(elapsedTime);
		m_menuBarList.at(i)->SetSize(m_menuBarSizeList.at(i).GetMovement());
		m_menuFontSizeList.at(i).Tick(elapsedTime);
		m_menuTextList.at(i)->SetFontSize(m_menuFontSizeList.at(i).GetMovement());
	}

	Ref bufferRef = m_sideBeltImage->GetPixelShader()->GetConstantBufferRef();
	bufferRef->SetVariable("Time", bufferRef->GetVariable<float>("Time") + elapsedTime);
	bufferRef->SetVariable("TextureSize", m_sideBelt->GetRect().size);
}

// キャンバスサイズを設定
void PauseScreen::SetCanvasSize(const Math::Vector2& outputSize)
{
	m_backGround->SetSize(m_canvasRef->GetSize());
}

// ポーズする
void PauseScreen::Pause(bool isPause)
{
	if (m_isPausing == isPause)
	{
		return;
	}

	m_isPausing = isPause;
	m_sideBeltPosition.SetMovement(Math::Vector2{ isPause ? 0.0f : -m_sideBeltWidth, 0.0f }, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_backGround->SetPivot(isPause ? Utility::AlignmentPoint::MiddleCenter : Utility::AlignmentPoint::MiddleLeft);
	m_backGround->SetAnchor(isPause ? Utility::AlignmentPoint::MiddleCenter : Utility::AlignmentPoint::MiddleRight);
}

// メニューの追加
void PauseScreen::AddMenu(const std::wstring& str, const std::function<void()>& onDecide)
{
	int size = static_cast<int>(m_menuList.size());

	Ref obj = Instantiate("Prefab_PauseScreenMenu");
	Ref menu = m_menuList.emplace_back(obj->GetComponent<RectTransform>());
	menu->SetPosition(m_menuBasePosition + Math::Vector2::UnitY * m_menuInterval * static_cast<float>(size));
	Ref text = m_menuTextList.emplace_back(obj->GetComponent<Renderings::Text>());
	text->SetStr(str);
	text->SetCanvasRef(m_canvasRef.Const());
	auto& fontSize = m_menuFontSizeList.emplace_back();
	fontSize.SetMovement(size == 0 ? m_menuBaseFontSize : m_menuBaseFontSize * 0.9f, 0.0f);
	m_menuBasePositionList.emplace_back(menu->GetRect().position);

	obj = Instantiate("Prefab_PauseScreenMenuBar");
	Ref bar = m_menuBarList.emplace_back(obj->GetComponent<RectTransform>());
	bar->SetPosition(m_menuBasePosition + Math::Vector2::UnitY * (m_menuInterval * static_cast<float>(size) - 5.0f));
	Ref image = obj->GetComponent<Renderings::Image>();
	image->SetCanvasRef(m_canvasRef.Const());
	auto& barSize = m_menuBarSizeList.emplace_back();
	barSize.SetMovement(size == 0 ? m_menuBarBaseSize : m_menuBarBaseSize * Math::Vector2::UnitY, 0.0f);
	m_menuBarBasePositionList.emplace_back(bar->GetRect().position);

	m_menuOnDecideList.emplace_back(onDecide);
}

// 上選択
void PauseScreen::SelectAbove()
{
	m_menuBarSizeList.at(m_selectNumber).SetMovement(m_menuBarBaseSize * Math::Vector2::UnitY, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_menuFontSizeList.at(m_selectNumber).SetMovement(m_menuBaseFontSize * 0.9f, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_selectNumber = Math::Clamp(m_selectNumber - 1, 0, static_cast<int>(m_menuList.size()) - 1);
	m_menuBarSizeList.at(m_selectNumber).SetMovement(m_menuBarBaseSize, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_menuFontSizeList.at(m_selectNumber).SetMovement(m_menuBaseFontSize, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
}

// 下選択
void PauseScreen::SelectBelow()
{
	m_menuBarSizeList.at(m_selectNumber).SetMovement(m_menuBarBaseSize * Math::Vector2::UnitY, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_menuFontSizeList.at(m_selectNumber).SetMovement(m_menuBaseFontSize * 0.9f, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_selectNumber = Math::Clamp(m_selectNumber + 1, 0, static_cast<int>(m_menuList.size()) - 1);
	m_menuBarSizeList.at(m_selectNumber).SetMovement(m_menuBarBaseSize, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
	m_menuFontSizeList.at(m_selectNumber).SetMovement(m_menuBaseFontSize, 0.25f, Easing::Type::Quart, Easing::InOut::Out);
}

// 決定
void PauseScreen::Decide()
{
	m_menuOnDecideList.at(m_selectNumber)();
}
