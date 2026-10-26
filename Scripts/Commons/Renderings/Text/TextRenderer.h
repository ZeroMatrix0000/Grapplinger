/*
 * FileName:     TextRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * テキスト描画
 */

#pragma once

#include "ITextRenderer.h"
#include "TextRendererHelper.h"
#include "DecorationTextRenderer.h"

namespace Renderings
{
	class PixelShader;
	
	// テキスト描画
	class TextRenderer : public ITextRenderer
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		TextRenderer();

		// 初期化処理
		void Initialize(ID2D1Device7* pDevice, ID2D1DeviceContext7* pContext, ID2D1Bitmap1* pBackBuffer);
		// フォントコレクションの作成
		void CreateFontCollection(const std::wstring& directoryPath);

		// 描画開始
		void Begin();
		// 描画処理
		void Draw(Ref<const Text> textRef, bool isDebug);
		// 描画終了
		void End();

		// ウィンドウサイズ変更時の処理
		void OnWindowSizeChanged(ID2D1DeviceContext7* pContext, ID2D1Bitmap1* pBackBuffer);

		// テキストの参照を追加
		void AddTextRef(Ref<const Text> textRef) override;
		// テキストの参照を削除
		void RemoveTextRef(Ref<const Text> textRef) override;

		// テキストの参照リストをソート
		void SortTextRefs();

		// テキストの参照リストを取得
		const std::vector<Ref<const Text>> GetTextRefs() const { return m_textRefs; }


	private:


		/* メンバ変数 */

		// ヘルパー関数群
		TextRenderHelper m_helper;

		// DirectWriteファクトリー
		Microsoft::WRL::ComPtr<IDWriteFactory8> m_dWriteFactory;

		// テキスト用コンテキスト
		Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_textContext;

		// フォントコレクション
		Microsoft::WRL::ComPtr<IDWriteFontCollection3> m_fontCollection;

		// ブラシ
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush;

		// 膨張エフェクト
		Microsoft::WRL::ComPtr<ID2D1Effect> m_dilateEffect;
		// 色変更エフェクト
		Microsoft::WRL::ComPtr<ID2D1Effect> m_floodEffect;
		// 画像合体エフェクト
		Microsoft::WRL::ComPtr<ID2D1Effect> m_compositeEffect;

		// デコレーション描画
		DecorationTextRenderer m_decorationRenderer;

		// テキストの参照リスト
		std::vector<Ref<const Text>> m_textRefs;

		// デバイスコンテキストのポインタ
		ID2D1DeviceContext7* m_pContext;
		// バックバッファのポインタ
		ID2D1Bitmap1* m_pBackBuffer;

	};
}
