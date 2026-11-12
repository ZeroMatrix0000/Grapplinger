/*
 * FileName:     ISceneManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 * 
 * シーン管理のインタフェース
 */

#pragma once

#include "../Systems/OnlyOne.h"

namespace Scenes
{
	template<typename TContext>
	class Scene;

	// シーン管理のインタフェース
	template<typename TTransitionData, typename TContext>
	class ISceneManager : public Systems::OnlyOne
	{

		typedef Scene<TContext> Scene;


	public:


		/* メンバ関数 */

		// コンストラクタ
		ISceneManager()
			: OnlyOne{ typeid(ISceneManager) }
		{
		}
		// デストラクタ
		virtual ~ISceneManager() = default;

		// 次のシーンを設定
		template<typename TScene> requires IsDerived<TScene, Scene>
		void SetNextScene()
		{
			SetNextScene(typeid(Ref<TScene>));
		}

		// シーン切り替え中かどうか
		virtual bool IsChanging() const = 0;

		// 遷移データの参照を取得
		virtual Ref<TTransitionData> GetTransitionDataRef() = 0;


	private:


		/* メンバ関数 */

		// 次のシーンを設定
		virtual void SetNextScene(const std::type_index& index) = 0;


	};
}
