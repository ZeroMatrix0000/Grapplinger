/*
 * FileName:     SampleScene.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * サンプルシーン
 */

#pragma once

#include "Scripts/Commons/Scenes/Scene.h"

namespace Renderings
{
	class ICameraScreen;
	class Canvas;
	class ParticleManager;
}
class SceneTransitionData;
class GameContext;
class DebugCamera;

// サンプルシーン
class SampleScene : public Scenes::Scene<SceneTransitionData, GameContext>
{

	typedef Scene<SceneTransitionData, GameContext> Scene;


public:


	/* メンバ関数 */

	// コンストラクタ
	SampleScene(const ComponentDesc& desc, Ref<const GameContext> context);

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
	Ref<Renderings::ICameraScreen> m_cameraScreenRef;
	// デバッグカメラの参照
	Ref<DebugCamera> m_debugCameraRef;

	// キャンバスの参照
	Ref<Renderings::Canvas> m_canvasRef;

	// パーティクル管理の参照
	Ref<Renderings::ParticleManager> m_particleManagerRef;

};
