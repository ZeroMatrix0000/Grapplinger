/*
 * FileName:     TextRendererHelper.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * テキスト描画のヘルパー関数群
 */

#pragma once

#include "Scripts/Commons/Systems/OnlyOne.h"

namespace Renderings
{
	class Text;
	class TextRenderer;
	class DecorationTextRenderer;

	// テキスト描画のヘルパー関数群
	struct TextRenderHelper : Systems::OnlyOne
	{

		/* メンバ関数 */

		// コンストラクタ
		TextRenderHelper(Ref<TextRenderer> renderer);

		// 長方形をキャンバス情報とピボットとアンカーに沿って移動
		Math::Rect AdjustRect
		(
			const Math::Vector2&    canvasSize,
			float                   canvasRatio,
			const Math::Rect&       rect,
			Utility::AlignmentPoint pivot,
			Utility::AlignmentPoint anchor
		);

		// 回転の中心を取得
		Math::Vector2 GetRotateOrigin(const Math::Rect& rect, Utility::AlignmentPoint pivot);

		// テキストフォーマットを生成
		Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat
		(
			const std::wstring&         fontName,
			float                       fontSize,
			float                       lineSpace,
			DWRITE_TEXT_ALIGNMENT       textAlignment,
			DWRITE_PARAGRAPH_ALIGNMENT  paragraphAlignment,
			Ref<IDWriteFactory8>        dWriteFactoryRef,
			Ref<IDWriteFontCollection3> fontCollectionRef
		);
		
		// 文字を描画
		void DrawTextLayout
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
		);


		/* メンバ変数 */

		// テキスト描画
		Ref<TextRenderer> m_renderer;

	};
}
