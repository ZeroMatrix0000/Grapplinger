/*
 * FileName:     Game.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ゲーム
 */

#include "Pch.h"
#include "Game.h"

#include "ResourcesInitializer.h"
#include "ComponentsRegisterer.h"
#include "IsDerivedRegister.h"
#include "Scripts/Scenes/Scenes.h"

// コンストラクタ
Game::Game()
	: OnlyOne{ typeid(Game) }
	, m_hWindow{}
	, m_deviceResources{}
	, m_renderingResources{}
	, m_deviceResources2D{ Ref{ m_deviceResources }.Const() }
	, m_renderer{}
	, m_resources{}
	, m_timer{}
	, m_windowController{}
	, m_input{}
	, m_errorMessage{}
	, m_componentManager{ Ref<IGameObjectInstantiator>{ m_gameObjectManager } }
	, m_gameObjectManager{ Ref<IComponentManager>{ m_componentManager }, Ref<Systems::IResources>{ m_resources } }
	, m_sceneManager{ Ref{ m_context }.Const() }
	, m_gameInput{ Ref<const Systems::IInput>{ m_input } }
	, m_context{}
{
}

// 初期化処理
void Game::Initialize(const HWND& hWindow)
{
	// ウィンドウハンドルを保持
	m_hWindow = hWindow;

	// エラーメッセージの初期化
	m_errorMessage.Initialize(5.0f, 20, true);

	// デバイスリソースの初期化
	m_deviceResources.Initialize(hWindow);
	m_deviceResources2D.Initialize();

	// デバイス
	Ref deviceRef = m_deviceResources.GetD3DDeviceRef();
	// コンテキスト
	Ref contextRef = m_deviceResources.GetD3DContextRef();
	// スワップチェイン
	Ref swapChainRef = m_deviceResources.GetSwapChainRef();

	// 描画リソースの初期化
	m_renderingResources.Initialize(deviceRef);
	m_renderingResources.SetEffectFactoryDirectory(L"Resources\\Models");

	// コモンステート
	Ref commonStatesRef = m_renderingResources.GetCommonStatesRef();

	// リソースの初期化
	ResourcesInitializer::InitializeResources
	(
		m_deviceResources,
		m_renderingResources,
		Ref{ m_resources }
	);

	// 描画の初期化
	m_renderer.Initialize(deviceRef, contextRef, commonStatesRef);
	// テキスト描画を初期化
	m_renderer.GetTextRenderer()->Initialize
	(
		m_deviceResources2D.GetDeviceRef(),
		m_deviceResources2D.GetContextRef(),
		m_deviceResources2D.GetBackBufferRef()
	);
	m_renderer.GetModelRenderer()->SetDepthStencilView(m_deviceResources.GetDepthStencilViewRef());
	m_renderer.GetModelRenderer()->SetVertexShader(m_resources.GetVertexShader("InstancingTest"));
	m_renderer.GetModelRenderer()->SetPixelShader(m_resources.GetPixelShader("InstancingTest"));
	m_renderer.GetTextRenderer()->CreateFontCollection(L"Resources\\Fonts");

	// タイマーの初期化
	m_timer.Initialize();

	// ウィンドウ管理の初期化
	m_windowController.Initialize(hWindow);
	// 入力の初期化
	m_input.Initialize(hWindow);

	// コンポーネントの登録
	ComponentsRegisterer::RegisterComponents
	(
		Ref{ m_componentManager },
		Ref{ m_gameObjectManager },
		m_renderer.GetModelRenderer().Cast<Renderings::IModel3DRenderer>(),
		m_renderer.GetImageRenderer().Cast<Renderings::IImageRenderer>(),
		m_renderer.GetTextRenderer().Cast<Renderings::ITextRenderer>(),
		m_renderer.GetColliderRenderer().Cast<Renderings::IColliderRenderer>(),
		Ref<Systems::IResources>{ m_resources },
		Ref{ m_context }.Const()
	);

	// エラーメッセージのオブジェクトを生成
	m_errorMessage.CreateObjects(Ref<IGameObjectInstantiator>{ m_gameObjectManager });

	// シーン管理の初期化
	InitializeSceneManager();

	// コンテキストの初期化
	m_context.Initialize
	(
		Ref<Systems::IWindowController>{ m_windowController },
		Ref<Systems::IInput>{ m_input },
		Ref<IComponentManager>{ m_componentManager },
		Ref<IGameObjectManager>{ m_gameObjectManager },
		Ref<Scenes::ISceneManager<SceneTransitionData, GameContext>>{ m_sceneManager },
		Ref<IGameInput>{ m_gameInput }
	);

	// 最初のシーンを設定
	//m_sceneManager.SetFirstScene<SampleScene>();
	m_sceneManager.SetFirstScene<TitleScene>();
}

// 更新処理
void Game::Update()
{
	// タイマーの更新
	m_timer.Update();

	// 入力の更新
	m_input.Update();

	// F4でフルスクリーン表示切り替え
	if (m_input.GetKeyDown(KeyName::F4))
	{
		m_windowController.ChangeFullScreen();
	}
	// F3で描画モード切り替え
	if (m_input.GetKeyDown(KeyName::F3))
	{
		m_renderer.ChangeRenderMode();
	}

	// シーン管理の更新
	m_sceneManager.Update(m_timer.GetDeltaTime());

	// エラーメッセージの更新
	m_errorMessage.Update(m_timer.GetDeltaTime());
}

// 描画処理
void Game::Render()
{
	// ウィンドウを受け取っていないなら何もしない
	if (!m_hWindow)
	{
		return;
	}

	// 画面の初期化
	m_deviceResources.Clear();

	// 描画
	m_renderer.Render();

	// 画面の表示
	m_deviceResources.Present(false);
}

// 終了処理
void Game::Finalize()
{
	// シーン管理の終了
	m_sceneManager.Finalize();
}

// ウィンドウサイズ変更時の処理
void Game::OnWindowSizeChanged(const Math::Vector2Int& outputSize)
{
	// ウィンドウを受け取っていないなら何もしない
	if (!m_hWindow)
	{
		return;
	}

	// ウィンドウの大きさの幅か高さが0なら何もしない
	if (outputSize.x == 0.0f || outputSize.y == 0.0f)
	{
		return;
	}

	// 2Dリソースをリセット
	m_deviceResources2D.Reset();

	m_deviceResources.OnWindowSizeChanged(outputSize);
	m_windowController.SetOutputSize(outputSize);

	m_deviceResources2D.OnWindowSizeChanged();

	m_renderer.GetModelRenderer()->SetDepthStencilView(m_deviceResources.GetDepthStencilViewRef());
	m_renderer.GetTextRenderer()->OnWindowSizeChanged(m_deviceResources2D.GetBackBufferRef());

	m_sceneManager.OnWindowSizeChanged(outputSize);
}

// シーン管理の初期化
void Game::InitializeSceneManager()
{
	// コンポーネント管理のインタフェース
	Ref iComponentManager = Ref<IComponentManager>{ m_componentManager };
	// 未参照オブジェクト
	Ref nullGameObject = m_gameObjectManager.GetNullGameObject();

	// シーンの追加
	m_sceneManager.AddScene<SampleScene>([iComponentManager, nullGameObject]
		{
			return std::unique_ptr<SampleScene>{ static_cast<SampleScene*>(iComponentManager->Create<SampleScene>(nullGameObject).release()) };
		});
	m_sceneManager.AddScene<TitleScene>([iComponentManager, nullGameObject]
		{
			return std::unique_ptr<TitleScene>{ static_cast<TitleScene*>(iComponentManager->Create<TitleScene>(nullGameObject).release()) };
		});
	m_sceneManager.AddScene<GamePlayScene>([iComponentManager, nullGameObject]
		{
			return std::unique_ptr<GamePlayScene>{ static_cast<GamePlayScene*>(iComponentManager->Create<GamePlayScene>(nullGameObject).release()) };
		});
	// シーン管理の初期化
	m_sceneManager.Initialize(m_windowController.GetOutputSize(), Ref<IGameObjectInstantiator>{ m_gameObjectManager });
}
