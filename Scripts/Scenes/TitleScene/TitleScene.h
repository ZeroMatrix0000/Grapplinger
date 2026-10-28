/*
 * FileName:     TitleScene.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
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

	// カメラ画面
	Renderings::CameraScreen<Camera::EulerTargetCamera>* m_pCameraScreen;
	// キャンバス
	Renderings::Canvas* m_pCanvas;

	// 選択メニューコンポーネント
	SelectMenu* m_pSelectMenu;

	// タイトル
	RectTransform* m_pTitle;
	// タイトル画像
	Renderings::Image* m_pTitleImage;
	// タイトルの基準位置
	Math::Vector2 m_titlePosition;
	// タイトルの移動周期
	Cycled m_titleSway;

};
