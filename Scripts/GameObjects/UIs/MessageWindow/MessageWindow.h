/*
 * FileName:     MessageWindow.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * メッセージウィンドウ
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class RectTransform;
}
namespace Renderings
{
	class Text;
	class Image;
}

// メッセージウィンドウ
class MessageWindow : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	MessageWindow(const ComponentDesc& desc);

	// 初期化処理
	void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 出現
	void Appear(bool isAppear);

	// テキストを設定
	void SetText(const std::wstring& text = L"");
	// テキストを全て表示
	void DisplayAllText();

	// 話者を設定
	void SetCharacter(const std::wstring& text = L" ");

	// 動いているかどうか
	bool IsMoving() const { return m_moveRate.IsMoving(); }

	// テキストを全て表示したか
	bool IsDisplayAllText() const { return m_textCount.IsMax(); }


private:


	/* メンバ変数 */

	// 移動時間
	float m_moveTime;
	// 1秒に表示される文字数
	float m_textVelocity;

	// 背景サイズ
	Math::Vector2 m_backgroundSize;
	// キャラクター名サイズ
	Math::Vector2 m_characterSize;
	// テキストサイズ
	Math::Vector2 m_textSize;

	// キャラクター名フォントサイズ
	float m_characterFontSize;
	// テキストフォントサイズ
	float m_textFontSize;

	// 移動率
	Easing::Value<float> m_moveRate;

	// 文字
	std::wstring m_text;
	// 文字数カウント
	Limited m_textCount;

	// 背景のトランスフォームの参照
	Ref<RectTransform> m_backgroundTransformRef;
	// 背景の参照
	Ref<Renderings::Image> m_backgroundRef;

	// テキストのトランスフォームの参照
	Ref<RectTransform> m_textTransformRef;
	// テキストの参照
	Ref<Renderings::Text> m_textRef;

	// キャラクター名のトランスフォームの参照
	Ref<RectTransform> m_characterTransformRef;
	// キャラクター名の参照
	Ref<Renderings::Text> m_characterRef;

};
