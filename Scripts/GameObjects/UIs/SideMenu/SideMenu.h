/*
 * FileName:     SideMenu.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/22
 *
 * サイドメニュー
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class RectTransform;
}
namespace Renderings
{
	class Image;
	class Text;
	class Canvas;
}

// サイドメニュー
class SideMenu : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	SideMenu(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);
	// 定数バッファの更新
	void UpdateConstantBuffer(float elapsedTime);

	// キャンバスサイズを設定
	void SetCanvasSize(const Math::Vector2& outputSize);

	// ポーズする
	void Pause(bool isPause);
	// ポーズ中かどうか
	bool IsPausing() const { return m_isPausing; }

	// メニューの追加
	void AddMenu(const std::wstring& str, const std::function<void()>& onDecide);

	// 上選択
	void SelectAbove();
	// 下選択
	void SelectBelow();
	// 決定
	void Decide();


private:


	/* メンバ変数 */

	// タイトル
	std::wstring m_titleStr;
	// メニューの間隔
	float m_menuInterval;
	// メニューの初期位置
	Math::Vector2 m_menuBasePosition;
	// メニューバーの初期サイズ
	Math::Vector2 m_menuBarBaseSize;
	// メニューの初期フォントサイズ
	float m_menuBaseFontSize;

	// ポーズ中かどうか
	bool m_isPausing;
	// 選択番号
	int m_selectNumber;

	// キャンバスのインタフェースの参照
	Ref<Renderings::Canvas> m_canvasRef;

	// 背景
	RectTransform* m_pBackGround;
	
	// 左帯
	Ref<RectTransform> m_sideBelt;
	// 左帯の画像
	Ref<Renderings::Image> m_sideBeltImage;
	// 左帯の太さ
	float m_sideBeltWidth;
	// 左帯の位置
	Easing::Value<Math::Vector2> m_sideBeltPosition;

	// タイトル
	Ref<RectTransform> m_title;

	// メニューリスト
	std::vector<Ref<RectTransform>> m_menuList;
	// メニューの基準位置リスト
	std::vector<Math::Vector2> m_menuBasePositionList;
	// メニューバーリスト
	std::vector<Ref<RectTransform>> m_menuBarList;
	// メニューバーの大きさリスト
	std::vector<Easing::Value<Math::Vector2>> m_menuBarSizeList;
	// メニューバーの基準位置リスト
	std::vector<Math::Vector2> m_menuBarBasePositionList;
	// メニューテキストリスト
	std::vector<Ref<Renderings::Text>> m_menuTextList;
	// メニューフォントサイズリスト
	std::vector<Easing::Value<float>> m_menuFontSizeList;
	// 決定時の処理
	std::vector<std::function<void()>> m_menuOnDecideList;

};
