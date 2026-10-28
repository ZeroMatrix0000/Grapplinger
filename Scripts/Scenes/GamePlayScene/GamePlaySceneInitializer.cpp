/*
 * FileName:     GamePlaySceneInitializer.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
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
#include "Scripts/GameObjects/UIs/Z2Talk/Z2Talk.h"
#include "Scripts/GameObjects/UIs/BlackBelt/BlackBelt.h"
#include "Scripts/GameObjects/UIs/MessageWindow/MessageWindow.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/Scenes/GamePlayScene/Actions/ActionManager.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Commons/GameObjects/IGameObjectManager.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Renderings/Model3D/Model3DSource.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Renderings/Shader/ConstantBuffer.h"
#include "Scripts/Commons/Systems/IInput.h"
#include "Scripts/Commons/Systems/IWindowController.h"

void GamePlaySceneInitializer::operator()(Ref<GamePlaySceneInternals> internalsRef) const
{
	// マウスをループ
	internalsRef->gameContextRef->GetPIInput()->SetMousePositionLoop(true);
	internalsRef->gameContextRef->GetPIInput()->DisplayCursor(false);

	// 出力サイズ
	const Math::Vector2& outputSize = internalsRef->gameContextRef->GetPIWindowController()->GetOutputSize();

	// ゲームオブジェクト管理
	auto* pIGameObjectManager = internalsRef->gameContextRef->GetPIGameObjectManager();

	pIGameObjectManager->SetGameObjectsRef(internalsRef->gameObjectsRef);
	pIGameObjectManager->Load("Scene_GamePlay");

	// プレイヤーを取得
	internalsRef->playerRef = pIGameObjectManager->Find("Player")->GetComponent<Player>();
	internalsRef->playerRef->UpdateModel();

	// プレイヤーカメラを取得
	Ref<GameObject> camera = pIGameObjectManager->Find("PlayerCamera");
	internalsRef->iCameraScreenRef = camera->GetComponent<Renderings::ICameraScreen>();
	internalsRef->playerCameraRef = camera->GetComponent<PlayerCamera>();
	internalsRef->playerCameraRef->SetPosition(internalsRef->playerRef->GetCameraTarget());
	internalsRef->iCameraScreenRef->UpdateViewMatrix();
	internalsRef->iCameraScreenRef->SetProjectionMatrix(outputSize);

	// キャンバスを取得
	internalsRef->canvasRef = pIGameObjectManager->Find("Canvas")->GetComponent<Renderings::Canvas>();
	internalsRef->canvasRef->SetSize(outputSize);

	// NPCマネージャーを取得
	internalsRef->nPCManagerRef = pIGameObjectManager->Find("NPCManager")->GetComponent<NPCManager>();

	// 地面を取得
	Ref<GameObject> ground = pIGameObjectManager->Find("Ground");
	internalsRef->groundMeshRef = ground->GetComponent<Colliders::MeshCollider>();
	internalsRef->groundModelRef = ground->GetComponent<Renderings::Model3D>();
	internalsRef->groundMeshRef->ApplyTransform();

	// レティクルを取得
	internalsRef->reticleRef = pIGameObjectManager->Find("Reticle")->GetComponent<Renderings::Image>();

	// はなすを取得
	internalsRef->z2TalkRef = pIGameObjectManager->Find("Z2Talk")->GetComponent<Z2Talk>();
	// 上下の黒帯取得
	internalsRef->blackBeltRef = pIGameObjectManager->Find("BlackBelt")->GetComponent<BlackBelt>();
	internalsRef->blackBeltRef->SetWidth(internalsRef->canvasRef->GetSize().x);

	// メッセージウィンドウを取得
	internalsRef->messageWindowRef = pIGameObjectManager->Find("MessageWindow")->GetComponent<MessageWindow>();

	// ターゲット照準を取得
	internalsRef->targetPointRef = pIGameObjectManager->Find("TargetPoint")->GetComponent<TargetPoint>();

	// アクション管理を取得
	internalsRef->actionManagerRef = pIGameObjectManager->Find("ActionManager")->GetComponent<ActionManager>();

	// シェーダにプレイヤーの座標を設定
	Ref constantBufferRef = internalsRef->groundModelRef->GetPModelSource()->GetPixelShader()->GetConstantBufferRef();
	constantBufferRef->SetVariable("ShadowPosition", internalsRef->playerRef->GetPosition() + Math::Vector3::Down * internalsRef->playerRef->GetBox().size.y / 2.0f);
	constantBufferRef->SetVariable("ShadowRadius", 0.75f);
	// NPC の当たり判定
	const auto nPCBoxes = internalsRef->nPCManagerRef->GetPBoxes();
	for (size_t i = 0; i < nPCBoxes.size(); i++)
	{
		constantBufferRef->SetVariable("ShadowPosition", nPCBoxes.at(i)->position + Math::Vector3::Down * nPCBoxes.at(i)->size.y / 2.0f, 16 * (i + 1));
		constantBufferRef->SetVariable("ShadowRadius", 0.75f, 16 * (i + 1));
	}
	constantBufferRef->SetVariable("ShadowCount", static_cast<int>(1 + nPCBoxes.size()));
}
