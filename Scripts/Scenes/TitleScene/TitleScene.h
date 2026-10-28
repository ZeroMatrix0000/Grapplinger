/*
 * FileName:     TitleScene.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * タイトルシーン
 */

#pragma once

#include "Scripts/Commons/Scenes/Scene.h"

namespace Components
{
	class RectTransform;
}
namespace Renderings
{
	class Image;
	class Canvas;
	template<typename TCamera> requires IsSame
	<
		TCamera,
		Camera::QuaternionCamera,
		Camera::QuaternionTargetCamera,
		Camera::EulerCamera,
		Camera::EulerTargetCamera
	>
	class CameraScreen;
}
class SceneTransitionData;
class GameContext;
class SelectMenu;

// サンプルシーン
class TitleScene : public Scenes::Scene<SceneTransitionData, GameContext>
{

	typedef Scene<SceneTransitionData, GameContext> Scene;


public:


	/* メンバ関数 */

	// コンストラクタ
	TitleScene(const ComponentDesc& desc, Ref<const GameContext> context);

	// 初期化処理
	void Initialize(const SceneTransitionData& data) override;
	// 更新処理
	void Update(float elapsedTime) override;
	// 終了処理
	void Finalize() override;

	// メッセージを受け取る
	void AcceptMessage(const std::string& message) override;


private:


	/* メンバ変数 */

	// カメラ画面の参照
	Ref<Renderings::CameraScreen<Camera::EulerTargetCamera>> m_cameraScreenRef;
	// キャンバスの参照
	Ref<Renderings::Canvas> m_canvasRef;

	// 選択メニューコンポーネントの参照
	Ref<SelectMenu> m_selectMenuRef;

	// タイトルの参照
	Ref<RectTransform> m_titleRef;
	// タイトル画像の参照
	Ref<Renderings::Image> m_titleImageRef;
	// タイトルの基準位置
	Math::Vector2 m_titlePosition;
	// タイトルの移動周期
	Cycled m_titleSway;

};
