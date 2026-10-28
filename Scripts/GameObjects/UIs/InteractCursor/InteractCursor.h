/*
 * FileName:     InteractCursor.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * 話しかける・調べる際のカーソル
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class RectTransform;
}
namespace Renderings
{
	class ICameraScreen;
	class Canvas;
	class Text;
	class Image;
}

// 話しかける・調べる際のカーソル
class InteractCursor : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	InteractCursor(const ComponentDesc& desc);

	// 初期化処理
	void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime, bool isFocused);

	// 名前を設定
	void SetName(const std::wstring& name);

	// キャンバスの参照を設定
	void SetCanvasRef(Ref<const Renderings::Canvas> canvasRef);

	// 座標を設定
	void SetPosition(const Math::Vector2& position);

	// キャンバスを取得
	const Renderings::Canvas& GetCanvas() const;


private:


	/* メンバ変数 */

	// 不透明度の速度
	float m_alphaVelocity;

	// 不透明度
	Limited m_alpha;

	// テキストのトランスフォームの参照
	Ref<RectTransform> m_textTransformRef;
	// テキストの参照
	Ref<Renderings::Text> m_textRef;

	// 画像のトランスフォームの参照
	Ref<RectTransform> m_imageTransformRef;
	// 画像の参照
	Ref<Renderings::Image> m_imageRef;

};
