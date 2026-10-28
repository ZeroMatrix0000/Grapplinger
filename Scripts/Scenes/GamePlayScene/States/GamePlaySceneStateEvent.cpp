/*
 * FileName:     GamePlaySceneStateEvent.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ゲームプレイシーンのイベント状態
 */

#include "Pch.h"
#include "GamePlaySceneStateEvent.h"

#include "GamePlaySceneStates.h"
#include "../GamePlaySceneInternals.h"
#include "../Actions/ActionManager.h"
#include "Scripts/GameObjects/Objects/Player/Player.h"
#include "Scripts/GameObjects/Objects/Player/PlayerCamera.h"
#include "Scripts/GameObjects/Objects/NPC/NPC.h"
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
GamePlaySceneStateEvent::GamePlaySceneStateEvent()
{
}

// 開始処理
void GamePlaySceneStateEvent::Enter(Ref<GamePlaySceneInternals> internalsRef)
{
	// イベントを設定
	internalsRef->actionManagerRef->LoadEvent(internalsRef->nPCManagerRef->GetPFocusedNPC()->GetEventName());

	// レティクルを非表示
	internalsRef->reticleRef->SetColor(DirectX::Colors::Transparent);

	// カーソルを隠す
	internalsRef->nPCManagerRef->HideCursor(true);

	// 黒帯を出現させる
	internalsRef->blackBeltRef->Appear(true);

	// ターゲット照準を非表示
	internalsRef->targetPointRef->Appear(false);
}

// 更新処理
void GamePlaySceneStateEvent::Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime)
{
	// NPCの更新
	internalsRef->nPCManagerRef->SetRotation(internalsRef->playerRef->GetPosition());
	internalsRef->nPCManagerRef->Update(elapsedTime);
	internalsRef->nPCManagerRef->SetCursor(internalsRef->iCameraScreenRef.Const());

	// ゲーム入力
	Ref iGameInput = internalsRef->gameContextRef->GetIGameInput();

	// プレイヤーの更新
	RotatePlayer(internalsRef);
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
	internalsRef->playerCameraRef->SetTarget
	(
		(internalsRef->playerRef->GetCameraTarget() + internalsRef->nPCManagerRef->GetPFocusedNPC()->GetPosition()) / 2.0f
	);
	internalsRef->playerCameraRef->Rotate(iGameInput->GetPlayerCameraRotate());
	internalsRef->playerCameraRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh(), internalsRef->playerRef->GetPosition());
	internalsRef->playerCameraRef->Update(elapsedTime);
	internalsRef->iCameraScreenRef->UpdateViewMatrix();

	// メッセージウィンドウの更新
	internalsRef->messageWindowRef->Update(elapsedTime);

	// アクション管理の更新
	internalsRef->actionManagerRef->Update(elapsedTime, iGameInput.Const());
	// 全てのアクションが終了したら状態遷移
	if (internalsRef->actionManagerRef->IsCompleted())
	{
		SetNextState(std::make_unique<GamePlaySceneStateField>());
	}
}

// 終了処理
void GamePlaySceneStateEvent::Exit(Ref<GamePlaySceneInternals> internalsRef)
{
	// カーソルを表示
	internalsRef->nPCManagerRef->HideCursor(false);

	// 黒帯を隠す
	internalsRef->blackBeltRef->Appear(false);
}

// プレイヤーを会話相手の方に向ける
void GamePlaySceneStateEvent::RotatePlayer(Ref<GamePlaySceneInternals> internalsRef)
{
	// プレイヤーを会話相手の方に向ける
	// 向く方向
	Math::Vector3 direction = internalsRef->nPCManagerRef->GetPFocusedNPC()->GetBox().position - internalsRef->playerRef->GetPosition();
	// Y軸方向は無視
	direction.y = 0.0f;
	// 正規化
	direction.Normalize();
	// 回転
	Math::Quaternion rotation = Math::Quaternion::FromToRotation(Math::Vector3::Backward, direction);
	internalsRef->playerRef->SetRotation(rotation);
}
