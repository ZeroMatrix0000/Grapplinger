/*
 * FileName:     GameContext.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ゲームコンテキスト
 */

#pragma once

#include "Scripts/Commons/Systems/OnlyOne.h"
#include "SceneTransitionData.h"

namespace Systems
{
	class IWindowController;
	class IInput;
}
namespace GameObjects
{
	class IGameObjectManager;
}
namespace Components
{
	class IComponentManager;
}
namespace Scenes
{
	template<typename TTransitionData, typename TContext>
	class ISceneManager;
}
class IGameInput;

// ゲームコンテキスト
class GameContext : public Systems::OnlyOne
{

	typedef Scenes::ISceneManager<SceneTransitionData, GameContext> ISceneManager;


public:


	/* メンバ関数 */

	// コンストラクタ
	GameContext();

	// 初期化処理
	void Initialize
	(
		Ref<Systems::IWindowController> iWindowController,
		Ref<Systems::IInput>            iInput,
		Ref<IComponentManager>          iComponentManager,
		Ref<IGameObjectManager>         iGameObjectManager,
		Ref<ISceneManager>              iSceneManager,
		Ref<IGameInput>                 iGameInput
	);

	// ウィンドウ管理インタフェースを取得
	Ref<Systems::IWindowController> GetIWindowController() const { return Ref{ *m_pIWindowController }; }
	// 入力管理インタフェースを取得
	Ref<Systems::IInput> GetIInput() const { return Ref{ *m_pIInput }; }

	// コンポーネント工場を取得
	Ref<IComponentManager> GetIComponentManager() const { return Ref{ *m_pIComponentManager }; }
	// コンポーネント工場を取得
	Ref<IGameObjectManager> GetIGameObjectManager() const { return Ref{ *m_pIGameObjectManager }; }

	// シーン管理インタフェースを取得
	Ref<ISceneManager> GetISceneManager() const { return Ref{ *m_pISceneManager }; }

	// ゲーム入力インタフェースを取得
	Ref<IGameInput> GetIGameInput() const { return Ref{ *m_pIGameInput }; }


private:


	/* メンバ変数 */

	// ウィンドウ管理インタフェースのポインタ
	Systems::IWindowController* m_pIWindowController;
	// 入力管理インタフェースのポインタ
	Systems::IInput* m_pIInput;

	// コンポーネント管理インタフェースのポインタ
	IComponentManager* m_pIComponentManager;
	// ゲームオブジェクト管理インタフェースのポインタ
	IGameObjectManager* m_pIGameObjectManager;

	// シーン管理インタフェースのポインタ
	ISceneManager* m_pISceneManager;

	// ゲーム入力インタフェースのポインタ
	IGameInput* m_pIGameInput;

};