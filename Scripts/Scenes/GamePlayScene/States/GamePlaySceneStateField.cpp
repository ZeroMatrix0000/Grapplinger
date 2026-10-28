/*
 * FileName:     GamePlaySceneStateField.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ゲームプレイシーンのフィールド状態
 */

#include "Pch.h"
#include "GamePlaySceneStateField.h"

#include "../GamePlaySceneInternals.h"
#include "GamePlaySceneStates.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Objects/Player/Player.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/GameObjects/Objects/Player/States/PlayerStates.h"
#include "Scripts/GameObjects/Objects/NPC/NPCManager.h"
#include "Scripts/GameObjects/UIs/Z2Talk/Z2Talk.h"
#include "Scripts/GameObjects/UIs/BlackBelt/BlackBelt.h"
#include "Scripts/GameObjects/UIs/MessageWindow/MessageWindow.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IGameInput.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/Renderings/Image/Image.h"

// コンストラクタ
GamePlaySceneStateField::GamePlaySceneStateField()
{
}

// 開始処理
void GamePlaySceneStateField::Enter(Ref<GamePlaySceneInternals> internalsRef)
{
	// レティクルを表示
	internalsRef->reticleRef->SetColor(DirectX::Colors::White);
	// ターゲット照準を表示
	internalsRef->targetPointRef->Appear(true);
}

// 更新処理
void GamePlaySceneStateField::Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime)
{
	// ゲーム入力
	Ref iGameInput = internalsRef->gameContextRef->GetIGameInput();

	// 糸伸ばし入力で糸を伸ばす
	if (iGameInput->GetPlayerExpandWire() && internalsRef->playerRef->CanExpandWire())
	{
		TargetPoint::Info info = internalsRef->targetPointRef->GetTargetInfo();
		internalsRef->playerRef->ExpandWire(info.position, static_cast<PlayerInternals::TargetType>(info.type));
	}

	// NPCの更新
	internalsRef->nPCManagerRef->SetRotation(internalsRef->playerRef->GetPosition());
	internalsRef->nPCManagerRef->Update(elapsedTime);
	internalsRef->nPCManagerRef->SetCursor(internalsRef->iCameraScreenRef.Const());

	// プレイヤーの更新
	internalsRef->playerRef->Update(elapsedTime, iGameInput->GetPlayerMove(), iGameInput->GetPlayerDash(), iGameInput->GetPlayerJump());
	internalsRef->nPCManagerRef->PlayerCollide(internalsRef->playerRef);
	internalsRef->playerRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh());
	internalsRef->playerRef->UpdateModel(elapsedTime);

	// カメラの更新
	internalsRef->playerCameraRef->SetTarget(internalsRef->playerRef->GetCameraTarget());
	internalsRef->playerCameraRef->Rotate(iGameInput->GetPlayerCameraRotate());
	internalsRef->playerCameraRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh(), internalsRef->playerRef->GetPosition());
	internalsRef->playerCameraRef->Update(elapsedTime);
	internalsRef->iCameraScreenRef->UpdateViewMatrix();

	// プレイヤーが話せる状態か
	bool canSpeak = internalsRef->playerRef->IsState<PlayerStateOnGround>();

	// 糸伸ばし中ならカーソルを非表示
	internalsRef->nPCManagerRef->HideCursor(!canSpeak);

	// はなすの更新
	internalsRef->z2TalkRef->SetPosition(internalsRef->playerRef->GetBox(), internalsRef->iCameraScreenRef.Const());
	internalsRef->z2TalkRef->Update(elapsedTime, internalsRef->nPCManagerRef->GetPFocusedNPC() != nullptr && canSpeak);

	// 上下の黒帯の更新
	internalsRef->blackBeltRef->Update(elapsedTime);

	// 話しかけられるNPC
	const NPC* pFocusedNPC = internalsRef->nPCManagerRef->GetPFocusedNPC();
	// 糸を伸ばしていないかつ
	// NPCに話しかけられる状態かつ
	// プレイヤーが話せる状態かつ
	// インタラクト入力がされたならイベント状態へ
	if (pFocusedNPC && canSpeak && iGameInput->GetFieldInteract())
	{
		SetNextState(std::make_unique<GamePlaySceneStateEvent>());
	}

	// メッセージウィンドウの更新
	internalsRef->messageWindowRef->Update(elapsedTime);

	// ターゲットの更新
	internalsRef->targetPointRef->Update(elapsedTime);
}

// 終了処理
void GamePlaySceneStateField::Exit(Ref<GamePlaySceneInternals> internalsRef)
{
}
