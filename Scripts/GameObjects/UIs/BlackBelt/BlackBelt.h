/*
 * FileName:     BlackBelt.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * 上下の黒帯
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

// 上下の黒帯
class BlackBelt : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	BlackBelt(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 出現
	void Appear(bool isAppear);

	// 幅を設定
	void SetWidth(float width);


private:


	/* メンバ変数 */

	// 移動時間
	float m_moveTime;

	// 高さ
	float m_height;

	// 移動率
	Easing::Value<float> m_moveRate;

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::Canvas> m_canvasRef;

	// 上の画像のトランスフォームの参照
	Ref<RectTransform> m_topImageTransformRef;
	// 上の画像の参照
	Ref<Renderings::Image> m_topImageRef;
	// 下の画像のトランスフォームの参照
	Ref<RectTransform> m_bottomImageTransformRef;
	// 下の画像の参照
	Ref<Renderings::Image> m_bottomImageRef;

};
