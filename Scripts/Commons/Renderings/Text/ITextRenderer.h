/*
 * FileName:     ITextRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * テキスト描画のインタフェース
 */

#pragma once

#include "Scripts/Commons/Systems/OnlyOne.h"

namespace Renderings
{
	class Text;

	// テキスト描画のインタフェース
	class ITextRenderer : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		ITextRenderer()
			: OnlyOne{ typeid(ITextRenderer) }
		{
		}
		// デストラクタ
		virtual ~ITextRenderer() = default;

		// テキストの参照を追加
		virtual void AddTextRef(Ref<const Text> textRef) = 0;
		// テキストの参照を削除
		virtual void RemoveTextRef(Ref<const Text> textRef) = 0;

	};
}
