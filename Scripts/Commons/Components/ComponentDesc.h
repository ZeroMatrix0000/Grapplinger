/*
 * FileName:     ComponentDesc.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * コンポーネント概要
 */

#pragma once

namespace GameObjects
{
	class GameObject;
	class IGameObjectInstantiator;
}

namespace Components
{
	class ComponentCreatePermit;

	// コンポーネント概要
	struct ComponentDesc
	{

		/* メンバ関数 */

		// コンストラクタ
		ComponentDesc(Ref<GameObject> owner, Ref<IGameObjectInstantiator> iGameObjectInstantiator, const ComponentCreatePermit&)
			: owner{ owner }
			, iGameObjectInstantiator{ iGameObjectInstantiator }
		{
		}


		/* メンバ変数 */

		// 所有者
		const Ref<GameObject> owner;
		// ゲームオブジェクト生成インタフェース
		const Ref<IGameObjectInstantiator> iGameObjectInstantiator;

	};
}
