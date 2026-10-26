/*
 * FileName:     Text.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * テキスト
 */

#include "Pch.h"
#include "Text.h"

#include "ITextRenderer.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
Renderings::Text::Text(const ComponentDesc& desc, Ref<ITextRenderer> iTextRendererRef)
	: Component{ desc }
	, m_str{}
	, m_fontName{}
	, m_fontSize{ 24.0f }
	, m_fontColor{ D2D1::ColorF::White }
	, m_textAlignment{}
	, m_paragraphAlignment{}
	, m_lineSpace{ 1.5f }
	, m_outlineWidth{ 0.0f }
	, m_outlineColor{ D2D1::ColorF::Black }
	, m_orderInLayer{}
	, m_canvasRef{ GetOwner()->GetNullComponent<Canvas>().Const() }
	, m_iTextRendererRef{ iTextRendererRef }
{
	// テキスト描画に参照を追加
	m_iTextRendererRef->AddTextRef(Ref{ *this }.Const());
}

// デストラクタ
Renderings::Text::~Text()
{
	// 画像描画から参照を削除
	m_iTextRendererRef->RemoveTextRef(Ref{ *this }.Const());
}

// 初期化処理
void Renderings::Text::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinderRef)
{
	std::wstring str = m_str.GetSourceStr();
	bool isDecoration = m_str.IsDecoration();

	Systems::JsonSerializer serializer{ iGameObjectFinderRef };
	serializer.AddParameter(&str, "Str");
	serializer.AddParameter(&isDecoration, "IsDecoration");
	serializer.AddParameter(&m_fontName, "FontName");
	serializer.AddParameter(&m_fontSize, "FontSize");
	serializer.AddParameter(&m_fontColor, "FontColor");
	serializer.AddParameter(&m_textAlignment, "TextAlignment");
	serializer.AddParameter(&m_paragraphAlignment, "ParagraphAlignment");
	serializer.AddParameter(&m_lineSpace, "LineSpace");
	serializer.AddParameter(&m_outlineWidth, "OutlineWidth");
	serializer.AddParameter(&m_outlineColor, "OutlineColor");
	serializer.AddParameter(&m_orderInLayer, "OrderInLayer");
	serializer.AddParameter(&m_canvasRef, "Canvas");
	serializer.Load(json);

	m_str.SetIsDecoration(isDecoration);
	m_str.SetStr(str);
}
