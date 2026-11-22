/*
 * FileName:     Goal.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * ゴール
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class Transform;
}
namespace Colliders
{
	class BoxCollider;
	class SphereCollider;
}
namespace Renderings
{
	class ICameraScreen;
	class ParticleManager;
}
class Player;

// ゴール
class Goal : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	Goal(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// プレイヤーとの当たり判定
	void PlayerCollide(Ref<Player> playerRef);

	// 体力を取得
	int GetHp() const { return m_hp; }

	// 隠す
	void Hide();


private:


	/* メンバ関数 */

	// 浮遊の大きさ
	float m_floatSize;
	// 浮遊周期
	Cycled m_floatTimer;
	// 回転周期
	Cycled m_rotateTimer;
	// パーティクル周期
	float m_particleTime;
	// 体力
	int m_hp;

	// 基準位置
	Math::Vector3 m_basePosition;
	// 揺れ
	Easing::Value<Math::Vector3> m_sway;
	// パーティクル周期
	float m_particleTimer;
	// 隠しているか
	bool m_isHide;

	// トランスフォームの参照
	Ref<Transform> m_transformRef;
	// 長方形の当たり判定の参照
	Ref<Colliders::BoxCollider> m_boxColliderRef;
	// 球の当たり判定の参照
	Ref<Colliders::SphereCollider> m_sphereColliderRef;
	// パーティクル管理の参照
	Ref<Renderings::ParticleManager> m_particleManagerRef;

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::ICameraScreen> m_iCameraScreenRef;

};
