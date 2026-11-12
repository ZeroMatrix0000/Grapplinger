/*
 * FileName:     GamePlaySceneStateField.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/04
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
#include "Scripts/GameObjects/Objects/Slime/SlimeManager.h"
#include "Scripts/GameObjects/Objects/CheckPoint/CheckPointManager.h"
#include "Scripts/GameObjects/UIs/Z2Talk/Z2Talk.h"
#include "Scripts/GameObjects/UIs/BlackBelt/BlackBelt.h"
#include "Scripts/GameObjects/UIs/MessageWindow/MessageWindow.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IGameInput.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Renderings/Model3D/Model3DSource.h"
#include "Scripts/Commons/Renderings/Shader/PixelShader.h"
#include "Scripts/Commons/Scenes/ISceneManager.h"

// コンストラクタ
GamePlaySceneStateField::GamePlaySceneStateField()
	: m_retryTimer{}
{
}

// 開始処理
void GamePlaySceneStateField::Enter(Ref<GamePlaySceneInternals> internalsRef)
{
	m_retryTimer.Initialize(0.75f, 0.0f, 0.75f);

	// レティクルを表示
	internalsRef->reticleRef->SetColor(DirectX::Colors::White);
	// ターゲット照準を表示
	internalsRef->targetPointRef->Appear(true);
}

// 更新処理
void GamePlaySceneStateField::Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime)
{
	// プレイヤーがフリーズ状態なら何もしない
	if (internalsRef->playerRef->IsState<PlayerStateFreeze>())
	{
		internalsRef->playerRef->Update(elapsedTime, Math::Vector3::Zero, false, false);
		return;
	}

	// ゲーム入力
	Ref iGameInput = internalsRef->gameContextRef->GetIGameInput();

	// 敗北状態
	if (internalsRef->playerRef->IsState<PlayerStateDefeat>())
	{
		// 入力を無効化
		if (iGameInput->GetIsActive())
		{
			iGameInput->SetActive(false);

			// プレイヤーの丸影の半径を 0 に
			Ref constantBufferRef = internalsRef->groundModelRef->GetModelSource()->GetPixelShader()->GetConstantBufferRef();
			constantBufferRef->SetVariable("ShadowRadius", 0.0f);

			// カメラを固定
			internalsRef->playerCameraRef->FixEyePosition();
		}

		m_retryTimer -= elapsedTime;
		if (!internalsRef->gameContextRef->GetISceneManager()->IsChanging() && m_retryTimer.IsMin())
		{
			internalsRef->gameContextRef->GetISceneManager()->SetNextScene<GamePlayScene>();
		}
	}

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

	// スライムの更新
	internalsRef->slimeManagerRef->Update(elapsedTime);

	// プレイヤーの更新
	internalsRef->playerRef->Update(elapsedTime, iGameInput->GetPlayerMove(), iGameInput->GetPlayerDash(), iGameInput->GetPlayerJump());
	internalsRef->nPCManagerRef->PlayerCollide(internalsRef->playerRef);
	internalsRef->slimeManagerRef->PlayerCollide(internalsRef->playerRef);
	internalsRef->playerRef->MeshCorrect(internalsRef->groundMeshRef->GetWorldMesh());
	internalsRef->playerRef->UpdateModel(elapsedTime);
	internalsRef->playerRef->DeathFloorCheck(internalsRef->deathFloorRef->GetWorldBox());

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

	// シェーダにオブジェクトの座標を設定
	Ref constantBufferRef = internalsRef->groundModelRef->GetModelSource()->GetPixelShader()->GetConstantBufferRef();
	auto slimeBoxes = internalsRef->slimeManagerRef->GetBoxes();
	for (size_t i = 0; i < slimeBoxes.size(); i++)
	{
		constantBufferRef->SetVariable("ShadowPosition", slimeBoxes.at(i).position + Math::Vector3::Down * slimeBoxes.at(i).size.y / 2.0f, 16 * (i + internalsRef->nPCManagerRef->GetNPCCount() + 1));
		constantBufferRef->SetVariable("ShadowRadius", 0.75f, 16 * (i + internalsRef->nPCManagerRef->GetNPCCount() + 1));
	}
	constantBufferRef->SetVariable("ShadowCount", static_cast<int>(1 + internalsRef->nPCManagerRef->GetNPCCount() + slimeBoxes.size()));

	// シーン遷移中でないなら
	if (!internalsRef->gameContextRef->GetISceneManager()->IsChanging())
	{
		// チェックポイントの計算
		Ref dataRef = internalsRef->gameContextRef->GetISceneManager()->GetTransitionDataRef();
		dataRef->checkPointNumber = Math::Max(dataRef->checkPointNumber, internalsRef->checkPointManagerRef->Check(internalsRef->playerRef->GetBox()));
	}
}

// 終了処理
void GamePlaySceneStateField::Exit(Ref<GamePlaySceneInternals> internalsRef)
{
}
