/*
 * FileName:     SelectMenu.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * 選択メニュー
 */

#pragma once

#include "SelectMenuParams.h"
#include "Scripts/Commons/Components/Component.h"

namespace Renderings
{
	class Image;
	class Text;
	class Canvas;
}
namespace Components
{
	class RectTransform;
}

 // 選択メニュー
class SelectMenu : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	SelectMenu(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 選択肢を追加
	void AddOption(const std::wstring& str, const std::function<void()>& Process);

	// 上の項目を選択
	void SelectUp();
	// 下の項目を選択
	void SelectDown();

	// 実行
	void Execute();


private:


	/* メンバ変数 */

	// パラメータ
	SelectMenuParams m_params;

	// 選択番号
	int m_selectNumber;
	// カーソルのY座標のズレ
	Easing::Value<float> m_cursorDelayY;
	// カーソルの左右の揺れの周期
	Cycled m_cursorSwayTimer;

	// 選択肢のテキストの参照
	std::vector<Ref<Renderings::Text>> m_textRefs;
	// 選択肢の処理
	std::vector<std::function<void()>> m_Processes;
	// 選択肢の表示サイズ
	std::vector<Easing::Value<float>> m_fontSizes;

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::Canvas> m_canvasRef;

	// カーソル画像の参照
	Ref<Renderings::Image> m_cursorImageRef;
	// カーソルの2D用トランスフォームの参照
	Ref<RectTransform> m_cursorRectTransformRef;

};
