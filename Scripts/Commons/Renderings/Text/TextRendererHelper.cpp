/*
 * FileName:     TextRendererHelper.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * テキスト描画のヘルパー関数群
 */

#include "Pch.h"
#include "TextRendererHelper.h"

#include "Text.h"
#include "TextRenderer.h"

// コンストラクタ
Renderings::TextRenderHelper::TextRenderHelper(Ref<TextRenderer> renderer)
	: OnlyOne{ typeid(TextRenderHelper) }
	, m_renderer{ renderer }
{
}

// 長方形をキャンバス情報とピボットとアンカーに沿って移動
Math::Rect Renderings::TextRenderHelper::AdjustRect(const Math::Vector2& canvasSize, float canvasRatio, const Math::Rect& rect, Utility::AlignmentPoint pivot, Utility::AlignmentPoint anchor)
{
	Math::Rect newRect = rect;

	// ピボットに合わせて長方形を移動
	switch (pivot)
	{
	case Utility::AlignmentPoint::TopLeft:
		newRect.position.x += newRect.size.x / 2.0f;
		newRect.position.y += newRect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::TopCenter:
		newRect.position.y += newRect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::TopRight:
		newRect.position.x -= newRect.size.x / 2.0f;
		newRect.position.y += newRect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleLeft:
		newRect.position.x += newRect.size.x / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleCenter:
		break;
	case Utility::AlignmentPoint::MiddleRight:
		newRect.position.x -= newRect.size.x / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomLeft:
		newRect.position.x += newRect.size.x / 2.0f;
		newRect.position.y -= newRect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomCenter:
		newRect.position.y -= newRect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomRight:
		newRect.position.x -= newRect.size.x / 2.0f;
		newRect.position.y -= newRect.size.y / 2.0f;
		break;
	default:
		break;
	}
	// アンカーに合わせて長方形を移動
	switch (anchor)
	{
	case Utility::AlignmentPoint::TopCenter:
		newRect.position.x += canvasSize.x / 2.0f;
		break;
	case Utility::AlignmentPoint::TopRight:
		newRect.position.x += canvasSize.x;
		break;
	case Utility::AlignmentPoint::MiddleLeft:
		newRect.position.y += canvasSize.y / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleCenter:
		newRect.position.x += canvasSize.x / 2.0f;
		newRect.position.y += canvasSize.y / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleRight:
		newRect.position.x += canvasSize.x;
		newRect.position.y += canvasSize.y / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomLeft:
		newRect.position.y += canvasSize.y;
		break;
	case Utility::AlignmentPoint::BottomCenter:
		newRect.position.x += canvasSize.x / 2.0f;
		newRect.position.y += canvasSize.y;
		break;
	case Utility::AlignmentPoint::BottomRight:
		newRect.position.x += canvasSize.x;
		newRect.position.y += canvasSize.y;
		break;
	default:
		break;
	}

	// 表示倍率を適用
	newRect.position *= canvasRatio;
	newRect.size *= canvasRatio;

	return newRect;
}

// 回転の中心を取得
Math::Vector2 Renderings::TextRenderHelper::GetRotateOrigin(const Math::Rect& rect, Utility::AlignmentPoint pivot)
{
	Math::Vector2 origin = rect.position;
	switch (pivot)
	{
	case Utility::AlignmentPoint::TopLeft:
		origin.x -= rect.size.x / 2.0f;
		origin.y -= rect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::TopCenter:
		origin.y -= rect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::TopRight:
		origin.x += rect.size.x / 2.0f;
		origin.y -= rect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleLeft:
		origin.x -= rect.size.x / 2.0f;
		break;
	case Utility::AlignmentPoint::MiddleCenter:
		break;
	case Utility::AlignmentPoint::MiddleRight:
		origin.x += rect.size.x / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomLeft:
		origin.x -= rect.size.x / 2.0f;
		origin.y += rect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomCenter:
		origin.y += rect.size.y / 2.0f;
		break;
	case Utility::AlignmentPoint::BottomRight:
		origin.x += rect.size.x / 2.0f;
		origin.y += rect.size.y / 2.0f;
		break;
	default:
		break;
	}

	return origin;
}

// テキストフォーマットを生成
Microsoft::WRL::ComPtr<IDWriteTextFormat> Renderings::TextRenderHelper::CreateTextFormat
(
	const std::wstring&         fontName,
	float                       fontSize,
	float                       lineSpace,
	DWRITE_TEXT_ALIGNMENT       textAlignment,
	DWRITE_PARAGRAPH_ALIGNMENT  paragraphAlignment,
	Ref<IDWriteFactory8>        dWriteFactoryRef,
	Ref<IDWriteFontCollection3> fontCollectionRef
)
{
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	dWriteFactoryRef->CreateTextFormat
	(
		fontName.c_str(),
		fontCollectionRef.Get(),
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ja-jp",
		textFormat.GetAddressOf()
	);

	// 行の上端とベースラインの距離
	float baseline{};
	switch (paragraphAlignment)
	{
	case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
		// 行の上端に文字の上端ががつくようにする
		baseline = fontSize;
		break;
	case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
		// 行の中心に文字の中心がつくようにする
		baseline = fontSize * (0.4f + lineSpace / 2.0f);
		break;
	case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
		// 行の下端に文字の下端がつくようにする
		baseline = fontSize * (lineSpace - 0.2f);
		break;
	default:
		break;
	}

	// 行間を設定
	textFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, fontSize * lineSpace, baseline);

	// 左右配置
	textFormat->SetTextAlignment(textAlignment);
	// 上下配置
	textFormat->SetParagraphAlignment(paragraphAlignment);

	return textFormat;
}

// 文字を描画
void Renderings::TextRenderHelper::DrawTextLayout
(
	float                       canvasRatio,
	const Math::Vector2&        position,
	Ref<const Text>             textRef,
	Ref<ID2D1DeviceContext7>    contextRef,
	Ref<IDWriteFactory8>        dWriteFactoryRef,
	Ref<IDWriteFontCollection3> fontCollectionRef,
	Ref<IDWriteTextLayout>      textLayoutRef,
	Ref<ID2D1SolidColorBrush>   brushRef,
	Ref<DecorationTextRenderer> DecorationRendererRef
)
{
	// 色付け詳細リスト
	const auto& colorDescList = textRef->GetDecorationString(m_renderer.GetValue()).GetColorDescList();
	// ルビ詳細リスト
	const auto& rubyDescList = textRef->GetDecorationString(m_renderer.GetValue()).GetRubyDescList();

	// エフェクトを追加
	for (const auto& colorDesc : colorDescList)
	{
		textLayoutRef->SetDrawingEffect
		(
			colorDesc.effect.Get(),
			DWRITE_TEXT_RANGE
			{
				static_cast<UINT32>(colorDesc.beginIndex),
				static_cast<UINT32>(colorDesc.length)
			}
		);
	}

	// 文字を描画
	DecorationRendererRef->Initialize(contextRef, brushRef);
	textLayoutRef->Draw
	(
		nullptr,
		DecorationRendererRef.Get(),
		position.x,
		position.y
	);

	// ルビを描画
	if (rubyDescList.size() != 0)
	{
		// フォントサイズ
		float fontSize = textRef->GetFontSize() * canvasRatio;

		// テキストフォーマット
		Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat = CreateTextFormat
		(
			textRef->GetFontName(),
			fontSize * 0.45f,
			1.0f,
			DWRITE_TEXT_ALIGNMENT_CENTER,
			DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
			dWriteFactoryRef,
			fontCollectionRef
		);

		// 元の色
		D2D1_COLOR_F oldColor = brushRef->GetColor();

		for (const auto& rubyDesc : rubyDescList)
		{
			// 色を設定
			for (const auto& colorDesc : colorDescList)
			{
				if (rubyDesc.beginIndex == Math::Clamp(rubyDesc.beginIndex, colorDesc.beginIndex, colorDesc.beginIndex + colorDesc.length))
				{
					brushRef->SetColor(colorDesc.effect->GetColor());
				}
			}

			// ヒット領域
			DWRITE_HIT_TEST_METRICS hitTestMetrics[8]{};
			// 領域数
			UINT32 hitTestMetricsCount = 0;

			textLayoutRef->HitTestTextRange
			(
				static_cast<UINT32>(rubyDesc.beginIndex),
				static_cast<UINT32>(rubyDesc.length),
				position.x,
				position.y,
				hitTestMetrics,
				8,
				&hitTestMetricsCount
			);

			// ルビの長方形
			Math::Rect rubyRect
			{
				Math::Vector2{ hitTestMetrics[0].left, hitTestMetrics[0].top },
				Math::Vector2{ fontSize * rubyDesc.m_text.size(), fontSize }
			};

			// 文字の真上に描画位置が来るようにする
			switch (textRef->GetParagraphAlignment())
			{
			case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
				rubyRect.position.y -= fontSize * 0.2f;
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
				rubyRect.position.y -= fontSize * (textRef->GetLineSpace() / 2.0f - 0.8f);
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
				rubyRect.position.y -= fontSize * (textRef->GetLineSpace() - 1.4f);
				break;
			default:
				break;
			}

			// 各領域の幅を足す
			for (const auto& element : hitTestMetrics)
			{
				rubyRect.position.x += element.width / 2.0f;
			}

			// テキストレイアウト
			Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
			dWriteFactoryRef->CreateTextLayout
			(
				rubyDesc.m_text.c_str(),
				static_cast<UINT32>(rubyDesc.m_text.size()),
				textFormat.Get(),
				rubyRect.size.x,
				rubyRect.size.y,
				textLayout.GetAddressOf()
			);

			// 描画
			textLayout->Draw
			(
				nullptr,
				DecorationRendererRef.Get(),
				rubyRect.position.x - rubyRect.size.x / 2.0f,
				rubyRect.position.y - rubyRect.size.y / 2.0f
			);

			// 元の色に戻す
			brushRef->SetColor(oldColor);
		}
	}
}
