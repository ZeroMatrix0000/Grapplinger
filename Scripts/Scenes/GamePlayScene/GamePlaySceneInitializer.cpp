/*
 * FileName:     GamePlaySceneInitializer.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/19
 *
 * ゲームプレイシーンの初期化関数
 */

#include "Pch.h"
#include "GamePlaySceneInitializer.h"

#include "GamePlaySceneInternals.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Objects/Player/Player.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/GameObjects/Objects/NPC/NPCManager.h"
#include "Scripts/GameObjects/Objects/Slime/SlimeManager.h"
#include "Scripts/GameObjects/Objects/CheckPoint/CheckPointManager.h"
#include "Scripts/GameObjects/UIs/Z2Talk/Z2Talk.h"
#include "Scripts/GameObjects/UIs/BlackBelt/BlackBelt.h"
#include "Scripts/GameObjects/UIs/MessageWindow/MessageWindow.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/GameObjects/UIs/SideMenu/SideMenu.h"
#include "Scripts/Scenes/GamePlayScene/Actions/ActionManager.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IGameInput.h"
#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/GameObjects/IGameObjectManager.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Scenes/ISceneManager.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Renderings/Model3D/Model3DSource.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Renderings/Shader/ConstantBuffer.h"
#include "Scripts/Commons/Systems/IWindowController.h"
#include "Scripts/Commons/Systems/IInput.h"
#include "Scripts/Commons/Systems/IFlagManager.h"

void GamePlaySceneInitializer::operator()(Ref<GamePlaySceneInternals> internalsRef) const
{
	// マウスをループ
	internalsRef->gameContextRef->GetIInput()->SetMousePositionLoop(true);
	internalsRef->gameContextRef->GetIInput()->DisplayCursor(false);

	// 出力サイズ
	const Math::Vector2& outputSize = internalsRef->gameContextRef->GetIWindowController()->GetOutputSize();

	// ゲームオブジェクト管理
	Ref iGameObjectManager = internalsRef->gameContextRef->GetIGameObjectManager();

	iGameObjectManager->SetGameObjectsRef(internalsRef->gameObjectsRef);
	iGameObjectManager->Load("Scene_GamePlay");

	// チェックポイントを取得
	internalsRef->checkPointManagerRef = iGameObjectManager->Find("CheckPointManager")->GetComponent<CheckPointManager>();
	const Math::Vector3& checkPointPosition = internalsRef->checkPointManagerRef->GetPosition
	(
		internalsRef->gameContextRef->GetIFlagManager()->GetFlag<int>("CheckPointNumber")
	);

	// プレイヤーを取得
	internalsRef->playerRef = iGameObjectManager->Find("Player")->GetComponent<Player>();
	internalsRef->playerRef->SetPosition(checkPointPosition);
	internalsRef->playerRef->UpdateModel();

	// キャンバスを取得
	internalsRef->canvasRef = iGameObjectManager->Find("Canvas")->GetComponent<Renderings::Canvas>();
	internalsRef->canvasRef->SetSize(outputSize);

	// NPCマネージャーを取得
	internalsRef->nPCManagerRef = iGameObjectManager->Find("NPCManager")->GetComponent<NPCManager>();
	// スライム管理を取得
	internalsRef->slimeManagerRef = iGameObjectManager->Find("SlimeManager")->GetComponent<SlimeManager>();
	Ref isDefeatSlimesRef = internalsRef->gameContextRef->GetIFlagManager()->GetFlagRef<bool>("IsDefeatSlimes");
	internalsRef->slimeManagerRef->SetIsDefeatSlimesRef(isDefeatSlimesRef);
	isDefeatSlimesRef.GetValue() = false;

	// 地面を取得
	Ref<GameObject> ground = iGameObjectManager->Find("Ground");
	internalsRef->groundMeshRef = ground->GetComponent<Colliders::MeshCollider>();
	internalsRef->groundModelRef = ground->GetComponent<Renderings::Model3D>();
	internalsRef->groundMeshRef->ApplyTransform();

	// 落下判定床を取得
	internalsRef->deathFloorRef = iGameObjectManager->Find("DeathFloor")->GetComponent<Colliders::BoxCollider>();
	internalsRef->deathFloorRef->ApplyTransform();

	// ゴールを取得
	internalsRef->goalRef = iGameObjectManager->Find("Goal")->GetComponent<Goal>();

	// レティクルを取得
	internalsRef->reticleRef = iGameObjectManager->Find("Reticle")->GetComponent<Renderings::Image>();

	// はなすを取得
	internalsRef->z2TalkRef = iGameObjectManager->Find("Z2Talk")->GetComponent<Z2Talk>();
	// 上下の黒帯取得
	internalsRef->blackBeltRef = iGameObjectManager->Find("BlackBelt")->GetComponent<BlackBelt>();
	internalsRef->blackBeltRef->SetWidth(internalsRef->canvasRef->GetSize().x);

	// メッセージウィンドウを取得
	internalsRef->messageWindowRef = iGameObjectManager->Find("MessageWindow")->GetComponent<MessageWindow>();

	// ターゲット照準を取得
	internalsRef->targetPointRef = iGameObjectManager->Find("TargetPoint")->GetComponent<TargetPoint>();

	// ポーズ画面を取得
	internalsRef->pauseScreenRef = iGameObjectManager->Find("PauseScreen")->GetComponent<SideMenu>();
	internalsRef->pauseScreenRef->SetCanvasSize(outputSize);
	internalsRef->pauseScreenRef->AddMenu(L"　ゲームを<ruby=つづ>続</ruby>ける", [internalsRef]
		{
			internalsRef->pauseScreenRef->Pause(false);
		});
	internalsRef->pauseScreenRef->AddMenu(L"　やり<ruby=なお>直</ruby>す", [internalsRef]
		{
			internalsRef->gameContextRef->GetISceneManager()->SetNextScene<GamePlayScene>();
			// 入力を無効化
			internalsRef->gameContextRef->GetIGameInput()->SetActive(false);
		});
	internalsRef->pauseScreenRef->AddMenu(L"　タイトルに<ruby=もど>戻</ruby>る", [internalsRef]
		{
			internalsRef->gameContextRef->GetISceneManager()->SetNextScene<TitleScene>();
			// 入力を無効化
			internalsRef->gameContextRef->GetIGameInput()->SetActive(false);
			// チェックポイントをリセット
			internalsRef->gameContextRef->GetIFlagManager()->GetFlagRef<int>("CheckPointNumber").GetValue() = 0;
		});

	// クリア画面を取得
	internalsRef->clearScreenRef = iGameObjectManager->Find("ClearScreen")->GetComponent<SideMenu>();
	internalsRef->clearScreenRef->SetCanvasSize(outputSize);
	internalsRef->clearScreenRef->AddMenu(L"　<ruby=さいしょ>最初</ruby>からやり<ruby=なお>直</ruby>す", [internalsRef]
		{
			internalsRef->gameContextRef->GetISceneManager()->SetNextScene<GamePlayScene>();
			// 入力を無効化
			internalsRef->gameContextRef->GetIGameInput()->SetActive(false);
			// チェックポイントをリセット
			internalsRef->gameContextRef->GetIFlagManager()->GetFlagRef<int>("CheckPointNumber").GetValue() = 0;
		});
	internalsRef->clearScreenRef->AddMenu(L"　タイトルに<ruby=もど>戻</ruby>る", [internalsRef]
		{
			internalsRef->gameContextRef->GetISceneManager()->SetNextScene<TitleScene>();
			// 入力を無効化
			internalsRef->gameContextRef->GetIGameInput()->SetActive(false);
			// チェックポイントをリセット
			internalsRef->gameContextRef->GetIFlagManager()->GetFlagRef<int>("CheckPointNumber").GetValue() = 0;
		});

	// 背景を取得
	internalsRef->backGroundRef = iGameObjectManager->Find("BackGround")->GetComponent<Renderings::Image>();
	internalsRef->backGroundRef->GetTransformRef()->SetSize(internalsRef->canvasRef->GetSize());
	internalsRef->backGroundRef->GetPixelShader()->GetConstantBufferRef()->SetVariable("TextureSize", internalsRef->canvasRef->GetSize());

	// プレイヤーカメラを取得
	Ref<GameObject> camera = iGameObjectManager->Find("PlayerCamera");
	internalsRef->iCameraScreenRef = camera->GetComponent<Renderings::ICameraScreen>();
	internalsRef->playerCameraRef = camera->GetComponent<PlayerCamera>();
	internalsRef->playerCameraRef->SetPosition(internalsRef->playerRef->GetCameraTarget());
	internalsRef->iCameraScreenRef->UpdateViewMatrix();
	internalsRef->iCameraScreenRef->SetProjectionMatrix(outputSize);

	// アクション管理を取得
	internalsRef->actionManagerRef = iGameObjectManager->Find("ActionManager")->GetComponent<ActionManager>();

	// シェーダにオブジェクトの座標を設定
	Ref constantBufferRef = internalsRef->groundModelRef->GetModelSource()->GetPixelShader()->GetConstantBufferRef();
	constantBufferRef->SetVariable("ShadowPosition", internalsRef->playerRef->GetPosition() + Math::Vector3::Down * internalsRef->playerRef->GetBox().size.y / 2.0f);
	constantBufferRef->SetVariable("ShadowRadius", 0.75f);
	// 当たり判定
	auto nPCBoxes = internalsRef->nPCManagerRef->GetBoxes();
	for (size_t i = 0; i < nPCBoxes.size(); i++)
	{
		constantBufferRef->SetVariable("ShadowPosition", nPCBoxes.at(i).position + Math::Vector3::Down * nPCBoxes.at(i).size.y / 2.0f, 16 * (i + 1));
		constantBufferRef->SetVariable("ShadowRadius", 0.75f, 16 * (i + 1));
	}
	auto slimeBoxes = internalsRef->slimeManagerRef->GetBoxes();
	for (size_t i = 0; i < slimeBoxes.size(); i++)
	{
		constantBufferRef->SetVariable("ShadowPosition", slimeBoxes.at(i).position + Math::Vector3::Down * slimeBoxes.at(i).size.y / 2.0f, 16 * (i + nPCBoxes.size() + 1));
		constantBufferRef->SetVariable("ShadowRadius", 0.75f, 16 * (i + nPCBoxes.size() + 1));
	}
	constantBufferRef->SetVariable("ShadowCount", static_cast<int>(1 + nPCBoxes.size() + slimeBoxes.size()));

	// フラグの初期化
	internalsRef->gameContextRef->GetIFlagManager()->GetFlagRef<bool>("IsTalk2NPC4").GetValue() = false;
}
