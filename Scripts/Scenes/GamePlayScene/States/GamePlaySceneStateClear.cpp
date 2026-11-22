/*
 * FileName:     GamePlaySceneStateClear.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/22
 *
 * ゲームプレイシーンのクリア状態
 */

#include "Pch.h"
#include "GamePlaySceneStateClear.h"

#include "GamePlaySceneStates.h"
#include "../GamePlaySceneInternals.h"
#include "Scripts/GameObjects/Objects/Player/Player.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/GameObjects/Objects/NPC/NPCManager.h"
#include "Scripts/GameObjects/Objects/Goal/Goal.h"
#include "Scripts/GameObjects/UIs/Z2Talk/Z2Talk.h"
#include "Scripts/GameObjects/UIs/BlackBelt/BlackBelt.h"
#include "Scripts/GameObjects/UIs/MessageWindow/MessageWindow.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/GameObjects/UIs/SideMenu/SideMenu.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IGameInput.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Systems/IInput.h"

 // コンストラクタ
GamePlaySceneStateClear::GamePlaySceneStateClear()
{
}

// 開始処理
void GamePlaySceneStateClear::Enter(Ref<GamePlaySceneInternals> internalsRef)
{
	// レティクルを非表示
	internalsRef->reticleRef->SetColor(DirectX::Colors::Transparent);
	// ターゲット照準を非表示
	internalsRef->targetPointRef->Hide();
	// ポーズ画面を開く
	internalsRef->clearScreenRef->Pause(true);

	// ゴールを隠す
	internalsRef->goalRef->Hide();

	// マウスをループ解除
	internalsRef->gameContextRef->GetIInput()->SetMousePositionLoop(false);
	internalsRef->gameContextRef->GetIInput()->DisplayCursor(true);
}

// 更新処理
void GamePlaySceneStateClear::Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime)
{
	// ゲーム入力
	Ref iGameInput = internalsRef->gameContextRef->GetIGameInput();

	// 上選択
	if (iGameInput->GetMenuAbove())
	{
		internalsRef->clearScreenRef->SelectAbove();
	}
	// 下選択
	if (iGameInput->GetMenuBelow())
	{
		internalsRef->clearScreenRef->SelectBelow();
	}
	// 決定
	if (iGameInput->GetMenuDecide())
	{
		internalsRef->clearScreenRef->Decide();
	}

	// NPCの更新
	internalsRef->nPCManagerRef->SetRotation(internalsRef->playerRef->GetPosition());
	internalsRef->nPCManagerRef->Update(elapsedTime);
	internalsRef->nPCManagerRef->SetCursor(internalsRef->iCameraScreenRef.Const());

	// ゴールの更新
	internalsRef->goalRef->Update(elapsedTime);

	// プレイヤーの更新
	internalsRef->playerRef->Update(elapsedTime, Math::Vector3::Zero, false, false);
	internalsRef->nPCManagerRef->PlayerCollide(internalsRef->playerRef);
	internalsRef->playerRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh());
	internalsRef->playerRef->UpdateModel(elapsedTime);

	// はなすの更新
	internalsRef->z2TalkRef->SetPosition(internalsRef->playerRef->GetBox(), internalsRef->iCameraScreenRef.Const());
	internalsRef->z2TalkRef->Update(elapsedTime, false);

	// 上下の黒帯の更新
	internalsRef->blackBeltRef->Update(elapsedTime);

	// カメラの更新
	internalsRef->playerCameraRef->SetTarget(internalsRef->playerRef->GetCameraTarget());
	internalsRef->playerCameraRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh(), internalsRef->playerRef->GetPosition());
	internalsRef->playerCameraRef->Update(elapsedTime);
	internalsRef->iCameraScreenRef->UpdateViewMatrix();

	// メッセージウィンドウの更新
	internalsRef->messageWindowRef->Update(elapsedTime);

	// ポーズ画面の更新
	internalsRef->pauseScreenRef->Update(elapsedTime);
	internalsRef->pauseScreenRef->UpdateConstantBuffer(elapsedTime);
	// クリア画面の更新
	internalsRef->clearScreenRef->Update(elapsedTime);

	// 背景の更新
	internalsRef->backGroundRef->GetPixelShader()->GetConstantBufferRef()->SetVariable
	(
		"Quaternion",
		internalsRef->playerCameraRef->GetRotation().CreateQuaternion()
	);
}

// 終了処理
void GamePlaySceneStateClear::Exit(Ref<GamePlaySceneInternals> internalsRef)
{
	// マウスをループ
	internalsRef->gameContextRef->GetIInput()->SetMousePositionLoop(true);
	internalsRef->gameContextRef->GetIInput()->DisplayCursor(false);
}
