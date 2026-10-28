/*
 * FileName:     TitleScene.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * タイトルシーン
 */

#include "Pch.h"
#include "TitleScene.h"

#include "../Scenes.h"
#include "Scripts/GameObjects/UIs/SelectMenu/SelectMenu.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/IWindowController.h"
#include "Scripts/Commons/Systems/IInput.h"
#include "Scripts/Commons/Scenes/ISceneManager.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Renderings/Model3D/Model3DSource.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Renderings/Shader/ConstantBuffer.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Renderings/CameraScreen.h"
#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectManager.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"

// コンストラクタ
TitleScene::TitleScene(const ComponentDesc& desc, Ref<const GameContext> context)
	: Scene{ desc, context }
	, m_cameraScreenRef{ GetOwner()->GetNullComponent<Renderings::CameraScreen<Camera::EulerTargetCamera>>() }
	, m_canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>() }
	, m_selectMenuRef{ GetOwner()->GetNullComponent<SelectMenu>() }
	, m_titleRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_titleImageRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_titlePosition{}
	, m_titleSway{}
{
}

// 初期化処理
void TitleScene::Initialize(const SceneTransitionData& data)
{
	// コンテキスト
	const auto& gameContext = GetContext();

	// 出力サイズ
	const Math::Vector2& outputSize = gameContext->GetIWindowController()->GetOutputSize();

	// ゲームオブジェクト管理
	Ref iGameObjectManager = gameContext->GetIGameObjectManager();

	iGameObjectManager->SetGameObjectsRef(GetGameObjects());
	iGameObjectManager->Load("Scene_Title");

	// カメラ画面を取得
	m_cameraScreenRef = iGameObjectManager->Find("TitleCamera")->GetComponent<Renderings::CameraScreen<Camera::EulerTargetCamera>>();
	m_cameraScreenRef->SetProjectionMatrix(outputSize);

	// キャンバスを取得
	m_canvasRef = iGameObjectManager->Find("Canvas")->GetComponent<Renderings::Canvas>();
	m_canvasRef->SetSize(outputSize);

	// 地面を取得
	Ref<GameObject> ground = iGameObjectManager->Find("Ground");
	ground->GetComponent<Colliders::MeshCollider>()->ApplyTransform();
	ground->GetComponent<Renderings::Model3D>()->GetModelSource()->GetPixelShader()->GetConstantBufferRef()->SetVariable("ShadowCount", 0);

	// 選択メニューを取得
	m_selectMenuRef = iGameObjectManager->Find("TitleMenu")->GetComponent<SelectMenu>();
	m_selectMenuRef->AddOption(L"スタート", [gameContext] { gameContext->GetISceneManager()->SetNextScene<GamePlayScene>(); });
	m_selectMenuRef->AddOption(L"ゲームを<ruby=しゅうりょう>終了</ruby>", [gameContext] { gameContext->GetIWindowController()->Destroy(); });

	// タイトルを取得
	Ref<GameObject> title = iGameObjectManager->Find("Title");
	m_titleRef = title->GetComponent<RectTransform>();
	m_titleImageRef = title->GetComponent<Renderings::Image>();
	m_titleImageRef->GetPixelShader()->GetConstantBufferRef()->SetVariable("TextureSize", m_titleImageRef->GetSize());
	m_titlePosition = m_titleRef->GetRect().position;
	m_titleSway.Initialize(0.0f, 0.0f, 360.0f);
}

// 更新処理
void TitleScene::Update(float elapsedTime)
{
	// シーン切り替え中でないなら
	if (!GetContext()->GetISceneManager()->IsChanging())
	{
		// 入力管理
		Ref iInput = GetContext()->GetIInput();

		// W or ↑ で上選択
		if (iInput->GetKeyDown(KeyName::W) || iInput->GetKeyDown(KeyName::Up))
		{
			m_selectMenuRef->SelectUp();
		}
		// S or ↓ で下選択
		if (iInput->GetKeyDown(KeyName::S) || iInput->GetKeyDown(KeyName::Down))
		{
			m_selectMenuRef->SelectDown();
		}
		// Z or Space で実行
		if (iInput->GetKeyDown(KeyName::Z) || iInput->GetKeyDown(KeyName::Space))
		{
			m_selectMenuRef->Execute();
		}
	}

	// 選択メニューの更新
	m_selectMenuRef->Update(elapsedTime);

	// タイトルの更新
	m_titleSway += 90.0f * elapsedTime;
	m_titleRef->SetPosition(m_titlePosition + Math::Vector2::UnitY * Math::Sin(Math::Deg2Rad(m_titleSway)) * 50.0f);
	Ref bufferRef = m_titleImageRef->GetPixelShader()->GetConstantBufferRef();
	bufferRef->SetVariable("Time", bufferRef->GetVariable<float>("Time") + elapsedTime);
	//buffer->SetVariable("timer", buffer->GetVariable<float>("time") + elapsedTime);

	// カメラ画面の更新
	Camera::EulerTargetCamera camera = m_cameraScreenRef->GetCamera();
	camera.rotation.y += 30.0f * elapsedTime;
	camera.rotation.Format();
	m_cameraScreenRef->SetCamera(camera);
	m_cameraScreenRef->UpdateViewMatrix();
}

// 終了処理
void TitleScene::Finalize()
{
}

// メッセージを受け取る
void TitleScene::AcceptMessage(const std::string& message)
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
