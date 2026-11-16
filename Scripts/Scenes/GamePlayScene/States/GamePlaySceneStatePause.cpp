/*
 * FileName:     GamePlaySceneStatePause.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/16
 *
 * ゲームプレイシーンのポーズ状態
 */

#include "Pch.h"
#include "GamePlaySceneStatePause.h"

#include "GamePlaySceneStates.h"
#include "../GamePlaySceneInternals.h"
#include "Scripts/GameObjects/UIs/TargetPoint/TargetPoint.h"
#include "Scripts/GameObjects/UIs/PauseScreen/PauseScreen.h"
#include "Scripts/Main/GameContext.h"
#include "Scripts/Main/IGameInput.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Systems/IInput.h"

// コンストラクタ
GamePlaySceneStatePause::GamePlaySceneStatePause()
{
}

// 開始処理
void GamePlaySceneStatePause::Enter(Ref<GamePlaySceneInternals> internalsRef)
{
	// レティクルを非表示
	internalsRef->reticleRef->SetColor(DirectX::Colors::Transparent);
	// ターゲット照準を非表示
	internalsRef->targetPointRef->Appear(false);
	// ポーズ画面を開く
	internalsRef->pauseScreenRef->Pause(true);

	// マウスをループ解除
	internalsRef->gameContextRef->GetIInput()->SetMousePositionLoop(false);
	internalsRef->gameContextRef->GetIInput()->DisplayCursor(true);
}

// 更新処理
void GamePlaySceneStatePause::Update(Ref<GamePlaySceneInternals> internalsRef, float elapsedTime)
{
	// ゲーム入力
	Ref iGameInput = internalsRef->gameContextRef->GetIGameInput();

	// ポーズ画面を開く
	if (iGameInput->GetPause())
	{
		SetNextState(std::make_unique<GamePlaySceneStateField>());
		return;
	}

	// ポーズ画面の更新
	internalsRef->pauseScreenRef->Update(elapsedTime);
}

// 終了処理
void GamePlaySceneStatePause::Exit(Ref<GamePlaySceneInternals> internalsRef)
{
	// ポーズ画面を閉じる
	internalsRef->pauseScreenRef->Pause(false);

	// マウスをループ
	internalsRef->gameContextRef->GetIInput()->SetMousePositionLoop(true);
	internalsRef->gameContextRef->GetIInput()->DisplayCursor(false);
}
