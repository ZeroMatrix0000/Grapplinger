/*
 * FileName:     Component.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * コンポーネント
 */

#pragma once

#include "Observer.h"

namespace GameObjects
{
	class GameObject;
	class IGameObjectInstantiator;
	class IGameObjectFinder;
}

namespace Components
{
	struct ComponentDesc;

	// コンポーネント
	class Component : public Observer
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Component(const ComponentDesc& desc);
		// デストラクタ
		virtual ~Component() = default;

		// コピーを破棄
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;

		// 初期化処理
		virtual void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) = 0;

		// 所有者を取得
		Ref<GameObject> GetOwner() { return m_owner = m_owner; }
		// 変更不可所有者を取得
		Ref<const GameObject> GetConstOwner() const { return m_owner.Const(); }

		// 未参照ゲームオブジェクトを取得
		Ref<GameObject> GetNullGameObject() const;


	protected:


		/* メンバ関数 */

		// ゲームオブジェクトを生成
		Ref<GameObject> Instantiate(const std::string& name) const;


	private:


		/* メンバ変数 */

		// 所有者
		Ref<GameObject> m_owner;

		// ゲームオブジェクト生成インタフェース
		const Ref<IGameObjectInstantiator> m_iGameObjectInstantiator;

	};
}
