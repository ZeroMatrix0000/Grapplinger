/*
 * FileName:     SampleScene.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * サンプルシーン
 */

#include "Pch.h"
#include "SampleScene.h"

#include "Scripts/GameObjects/Objects/DebugCamera/DebugCamera.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/IWindowController.h"
#include "Scripts/Commons/Systems/IInput.h"
#include "Scripts/Commons/Scenes/ISceneManager.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectManager.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"
#include "Scripts/Commons/Renderings/Particle/ParticleSample.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Colliders/SphereCollider.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"

// コンストラクタ
SampleScene::SampleScene(const ComponentDesc& desc, Ref<const GameContext> context)
	: Scene{ desc, context }
	, m_cameraScreenRef{ GetOwner()->GetNullComponent<Renderings::ICameraScreen>() }
	, m_debugCameraRef{ GetOwner()->GetNullComponent<DebugCamera>() }
	, m_canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>() }
	, m_particleManagerRef{ GetOwner()->GetNullComponent<Renderings::ParticleManager>() }
{
}

// 初期化処理
void SampleScene::Initialize(const SceneTransitionData& data)
{
	// コンテキスト
	Ref gameContext = GetContext();

	// 出力サイズ
	const Math::Vector2& outputSize = gameContext->GetIWindowController()->GetOutputSize();

	// ゲームオブジェクト管理
	Ref iGameObjectManager = gameContext->GetIGameObjectManager();

	iGameObjectManager->SetGameObjectsRef(GetGameObjects());
	iGameObjectManager->Load("Scene_Sample");

	// デバッグカメラを取得
	Ref<GameObject> camera = iGameObjectManager->Find("DebugCamera");
	m_cameraScreenRef = camera->GetComponent<Renderings::ICameraScreen>();
	m_cameraScreenRef->SetProjectionMatrix(outputSize);
	m_debugCameraRef = camera->GetComponent<DebugCamera>();

	// キャンバスを取得
	m_canvasRef = iGameObjectManager->Find("Canvas")->GetComponent<Renderings::Canvas>();
	m_canvasRef->SetSize(outputSize);

	// パーティクル管理を取得
	m_particleManagerRef = iGameObjectManager->Find("ParticleManager")->GetComponent<Renderings::ParticleManager>();

	// プレイヤーを取得
	Ref<GameObject> player = iGameObjectManager->Find("Player");
	player->GetComponent<Colliders::BoxCollider>()->ApplyTransform();
	player->GetComponent<Colliders::SphereCollider>()->ApplyTransform();

	// 地面を取得
	Ref<GameObject> ground = iGameObjectManager->Find("Ground");
	ground->GetComponent<Colliders::MeshCollider>()->ApplyTransform();
}

// 更新処理
void SampleScene::Update(float elapsedTime)
{
	// 入力管理
	Ref iInput = GetContext()->GetIInput();

	// F5でシーン移動
	if (iInput->GetKeyDown(KeyName::F5))
	{
		GetContext()->GetISceneManager()->SetNextScene<SampleScene>();
		return;
	}

	if (iInput->GetKey(KeyName::Space))
	{
		m_particleManagerRef->Play<Renderings::ParticleSample>();
	}
	m_particleManagerRef->Update(elapsedTime);

	// カメラの更新
	m_debugCameraRef->SetInput
	(
		iInput->GetMouseMovement(),
		iInput->GetMouseButton(MouseButtonName::Right),
		iInput->GetMouseButton(MouseButtonName::Middle),
		iInput->GetMouseWheelDelta()
	);
	m_debugCameraRef->Update(elapsedTime);
	m_cameraScreenRef->UpdateViewMatrix();
}

// 終了処理
void SampleScene::Finalize()
{
}

// メッセージを受け取る
void SampleScene::AcceptMessage(const std::string& message)
{
	if (message == "WindowSizeChanged")
	{
		// 出力サイズ
		const Math::Vector2& outputSize = GetContext()->GetIWindowController()->GetOutputSize();
		// プロジェクション行列を設定
		m_cameraScreenRef->SetProjectionMatrix(outputSize);
		// キャンバスのサイズを設定
		m_canvasRef->SetSize(outputSize);
	}
}
