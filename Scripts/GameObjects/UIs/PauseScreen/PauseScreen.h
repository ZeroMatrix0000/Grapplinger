/*
 * FileName:     PauseScreen.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/16
 *
 * ポーズ画面
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
	class Canvas;
}

// ポーズ画面
class PauseScreen : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PauseScreen(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// キャンバスサイズを設定
	void SetCanvasSize(const Math::Vector2& outputSize);

	// ポーズする
	void Pause(bool isPause);


private:


	/* メンバ変数 */

	// ポーズ中かどうか
	bool m_isPausing;

	// 左帯の太さ
	float m_sideBeltWidth;

	// 左帯の位置
	Easing::Value<Math::Vector2> m_sideBeltPosition;

	// 背景
	Ref<RectTransform> m_backGround;
	// 左帯
	Ref<RectTransform> m_sideBelt;
	// 左帯の画像
	Ref<Renderings::Image> m_sideBeltImage;

	// キャンバスのインタフェースの参照
	Ref<Renderings::Canvas> m_canvasRef;

};
