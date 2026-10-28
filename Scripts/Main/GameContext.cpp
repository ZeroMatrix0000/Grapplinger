/*
 * FileName:     GameContext.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ゲームコンテキスト
 */

#include "Pch.h"
#include "GameContext.h"

// コンストラクタ
GameContext::GameContext()
	: OnlyOne{ typeid(GameContext) }
	, m_pIWindowController{}
	, m_pIInput{}
	, m_pIComponentManager{}
	, m_pIGameObjectManager{}
	, m_pISceneManager{}
	, m_pIGameInput{}
{
}

// 初期化処理
void GameContext::Initialize
(
	Ref<Systems::IWindowController> iWindowController,
	Ref<Systems::IInput>            iInput,
	Ref<IComponentManager>          iComponentManager,
	Ref<IGameObjectManager>         iGameObjectManager,
	Ref<ISceneManager>              iSceneManager,
	Ref<IGameInput>                 iGameInput
)
{
	m_pIWindowController  = iWindowController.Get();
	m_pIInput             = iInput.Get();
	m_pIComponentManager  = iComponentManager.Get();
	m_pIGameObjectManager = iGameObjectManager.Get();
	m_pISceneManager      = iSceneManager.Get();
	m_pIGameInput         = iGameInput.Get();
}
