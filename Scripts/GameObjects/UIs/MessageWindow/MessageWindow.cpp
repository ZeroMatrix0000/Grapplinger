/*
 * FileName:     MessageWindow.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * メッセージウィンドウ
 */

#include "Pch.h"
#include "MessageWindow.h"

#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Text/Text.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
MessageWindow::MessageWindow(const ComponentDesc& desc)
	: Component{ desc }
	, m_moveTime{}
	, m_textVelocity{}
	, m_backgroundSize{}
	, m_characterSize{}
	, m_textSize{}
	, m_characterFontSize{}
	, m_textFontSize{}
	, m_moveRate{}
	, m_text{}
	, m_textCount{}
	, m_backgroundTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_backgroundRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_textTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_textRef{ GetOwner()->GetNullComponent<Renderings::Text>() }
	, m_characterTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_characterRef{ GetOwner()->GetNullComponent<Renderings::Text>() }
{
}

// 初期化処理
void MessageWindow::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Ref canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>().Const() };

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_moveTime }, "MoveTime");
	serializer.AddParameter(Ref{ m_textVelocity }, "TextVelocity");
	serializer.AddParameter(canvasRef.Nest(), "Canvas");
	serializer.Load(json);

	// メッセージウィンドウを生成
	if (m_backgroundTransformRef == GetOwner()->GetNullComponent<RectTransform>())
	{
		Ref<GameObject> refObj = Instantiate("Prefab_MessageWindowBackGround");
		m_backgroundTransformRef = refObj->GetComponent<RectTransform>();
		m_backgroundRef = refObj->GetComponent<Renderings::Image>();
		refObj = Instantiate("Prefab_MessageWindowText");
		m_textTransformRef = refObj->GetComponent<RectTransform>();
		m_textRef = refObj->GetComponent<Renderings::Text>();
		refObj = Instantiate("Prefab_MessageWindowCharacter");
		m_characterTransformRef = refObj->GetComponent<RectTransform>();
		m_characterRef = refObj->GetComponent<Renderings::Text>();
	}

	if (canvasRef != GetOwner()->GetNullComponent<Renderings::Canvas>().Const())
	{
		// キャンバスを設定
		m_backgroundRef->SetCanvasRef(canvasRef);
		m_characterRef->SetCanvasRef(canvasRef);
		m_textRef->SetCanvasRef(canvasRef);
	}

	// 移動率を初期化
	m_moveRate.SetMovement(0.0f, 0.0f);

	// サイズを取得
	m_backgroundSize = m_backgroundTransformRef->GetRect().size;
	m_characterSize = m_characterTransformRef->GetRect().size;
	m_textSize = m_textTransformRef->GetRect().size;
	m_characterFontSize = m_characterRef->GetFontSize();
	m_textFontSize = m_textRef->GetFontSize();

	// 非表示
	m_backgroundTransformRef->SetSize(Math::Vector2::Zero);
	m_characterTransformRef->SetSize(Math::Vector2::Zero);
	m_textTransformRef->SetSize(Math::Vector2::Zero);
	m_characterRef->SetFontSize(0.0f);
	m_textRef->SetFontSize(0.0f);

}

// 更新処理
void MessageWindow::Update(float elapsedTime)
{
	// 移動率を更新
	m_moveRate.Tick(elapsedTime);
	// 移動率
	float moveRate = m_moveRate.GetMovement();

	// 透明度を変更
	Math::Color color = m_backgroundRef->GetColor();
	color.w = moveRate;
	m_backgroundRef->SetColor(color);
	color = m_characterRef->GetFontColor();
	color.w = moveRate;
	m_characterRef->SetFontColor(color);
	color = m_textRef->GetFontColor();
	color.w = moveRate;
	m_textRef->SetFontColor(color);

	// 古い文字数
	size_t oldTextCount = Math::RoundInt(m_textCount);
	// 文字数を計算
	m_textCount += m_textVelocity * elapsedTime;
	// 新しく表示する文字数
	size_t newCharCount = Math::RoundInt(m_textCount) - oldTextCount;

	// 検索番号
	size_t index = oldTextCount;

	// <> の間か
	bool isAngleBracket = false;
	// 文字列に変更があったか
	bool isChanged = false;

	while (newCharCount != 0 && !m_textCount.IsMax())
	{
		// 文字
		wchar_t c = m_text.at(index);

		if (isAngleBracket)
		{
			m_textCount += 1.0f;
			if (c == L'>')
			{
				isAngleBracket = false;
			}
		}
		else
		{
			if (c == L'<')
			{
				isAngleBracket = true;
				m_textCount += 1.0f;
			}
			else
			{
				isChanged = true;
				newCharCount--;
			}
		}

		index++;
	}

	// 移動率が 0 なら以降何もしない
	if (moveRate == 0.0f)
	{
		return;
	}

	// サイズを変更
	m_backgroundTransformRef->SetSize(Math::Vector2::Lerp(Math::Vector2::Zero, m_backgroundSize, moveRate));
	m_characterTransformRef->SetSize(Math::Vector2::Lerp(Math::Vector2::Zero, m_characterSize, moveRate));
	m_textTransformRef->SetSize(Math::Vector2::Lerp(Math::Vector2::Zero, m_textSize, moveRate));

	// フォントサイズを変更
	m_characterRef->SetFontSize(m_characterFontSize * moveRate);
	m_textRef->SetFontSize(m_textFontSize * moveRate);

	// 文字列を変更
	if (isChanged)
	{
		std::wstring rubyStr = m_text.substr(Math::RoundInt(m_textCount));
		size_t rubyIndex = rubyStr.find(L"</ruby>");
		if (rubyIndex != std::wstring::npos)
		{
			rubyStr = rubyStr.substr(0, rubyIndex);
		}
		Renderings::DecorationString::DeleteTag(Ref{ rubyStr });

		std::wstring str = m_text.substr(0, Math::RoundInt(m_textCount));
		str.append(L"<color=#00000000>");
		str.append(rubyStr);

		m_textRef->SetStr(str);
	}
}

// 出現
void MessageWindow::Appear(bool isAppear)
{
	m_moveRate.SetMovement
	(
		isAppear ? 0.0f : 1.0f,
		isAppear ? 1.0f : 0.0f,
		m_moveTime,
		Easing::Type::Back,
		isAppear ? Easing::InOut::Out : Easing::InOut::In
	);
}

// テキストを設定
void MessageWindow::SetText(const std::wstring& text)
{
	m_text = text;
	m_textCount.Initialize(0.0f, 0.0f, static_cast<float>(text.size()));
	m_textRef->SetStr(L"");
}

// テキストを全て表示
void MessageWindow::DisplayAllText()
{
	m_textCount = static_cast<float>(m_text.size());
	m_textRef->SetStr(m_text);
}

// 話者を設定
void MessageWindow::SetCharacter(const std::wstring& text)
{
	// 空文字でなければ
	if (!text.empty())
	{
		m_characterRef->SetStr(text);
	}
}
