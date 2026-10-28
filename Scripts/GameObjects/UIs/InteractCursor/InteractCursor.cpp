/*
 * FileName:     InteractCursor.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * 話しかける・調べる際のカーソル
 */

#include "Pch.h"
#include "InteractCursor.h"

#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/Renderings/Text/Text.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
InteractCursor::InteractCursor(const ComponentDesc& desc)
	: Component{ desc }
	, m_alphaVelocity{}
	, m_alpha{}
	, m_textTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_textRef{ GetOwner()->GetNullComponent<Renderings::Text>() }
	, m_imageTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_imageRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
{
}

// 初期化処理
void InteractCursor::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_alphaVelocity }, "AlphaVelocity");
	serializer.Load(json);

	m_alpha.Initialize(0.0f, 0.0f, 1.0f);

	if (m_textTransformRef == GetOwner()->GetNullComponent<RectTransform>())
	{
		Ref<GameObject> objRef = Instantiate("Prefab_InteractCursorText");
		m_textTransformRef = objRef->GetComponent<RectTransform>();
		m_textRef = objRef->GetComponent<Renderings::Text>();
		objRef = Instantiate("Prefab_InteractCursorImage");
		m_imageTransformRef = objRef->GetComponent<RectTransform>();
		m_imageRef = objRef->GetComponent<Renderings::Image>();
	}
}

// 更新処理
void InteractCursor::Update(float elapsedTime, bool isFocused)
{
	// 不透明度を設定
	m_alpha += m_alphaVelocity * elapsedTime * (isFocused ? 1.0f : -1.0f);

	Math::Color textColor = m_textRef->GetFontColor();
	textColor.w = m_alpha;
	m_textRef->SetFontColor(textColor);

	Math::Color imageColor = m_imageRef->GetColor();
	imageColor.w = m_alpha;
	m_imageRef->SetColor(imageColor);
}

// 名前を設定
void InteractCursor::SetName(const std::wstring& name)
{
	m_textRef->SetStr(name);
}

// キャンバスの参照を設定
void InteractCursor::SetCanvasRef(Ref<const Renderings::Canvas> canvasRef)
{
	m_textRef->SetCanvasRef(canvasRef);
	m_imageRef->SetCanvasRef(canvasRef);
}

// 座標を設定
void InteractCursor::SetPosition(const Math::Vector2& position)
{
	// 透明なら何もしない
	if (m_alpha == 0.0f)
	{
		return;
	}

	m_textTransformRef->SetPosition(position);
	m_imageTransformRef->SetPosition(position);
}

// キャンバスを取得
const Renderings::Canvas& InteractCursor::GetCanvas() const
{
	return m_textRef->GetCanvasRef().GetValue();
}
