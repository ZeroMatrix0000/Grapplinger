/*
 * FileName:     Scene.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * シーン
 */

#pragma once

#include "../Components/Component.h"

namespace Scenes
{
	// シーン
	template<typename TTransitionData, typename TContext>
	class Scene : public Component
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Scene(const ComponentDesc& desc, Ref<const TContext> context)
			: Component{ desc }
			, m_gameObjects{}
			, m_context{ context }
		{
		}

		// 初期化処理
		void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override {};
		// 初期化処理
		virtual void Initialize(const TTransitionData& data) = 0;
		// 更新処理
		virtual void Update(float elapsedTime) = 0;
		// 終了処理
		virtual void Finalize() = 0;


	protected:


		/* メンバ関数 */

		// ゲームオブジェクトリストを取得
		Ref<std::vector<std::unique_ptr<GameObject>>> GetGameObjects() { return Ref{ m_gameObjects }; }

		// コンテキストを取得
		Ref<const TContext> GetContext() { return m_context; }


	private:


		/* メンバ変数 */

		// ゲームオブジェクトリスト
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;

		// コンテキスト
		Ref<const TContext> m_context;

	};
}
