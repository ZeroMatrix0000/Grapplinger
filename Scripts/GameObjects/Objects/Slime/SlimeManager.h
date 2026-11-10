/*
 * FileName:     SlimeManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * スライム管理
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class Transform;
}
namespace Renderings
{
	class ParticleManager;
	class ICameraScreen;
}
class Slime;
class Player;

// NPC管理
class SlimeManager : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	SlimeManager(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// プレイヤーとの当たり判定
	void PlayerCollide(Ref<Player> playerRef);

	// 当たり判定を取得
	std::vector<Math::Box> GetBoxes() const;


private:


	/* メンバ変数 */

	// スライムの参照リスト
	std::vector<Ref<Slime>> m_slimeRefList;

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::ICameraScreen> m_iCameraScreenRef;

	// トランスフォームの参照
	Ref<Transform> m_transformRef;
	// パーティクル管理の参照
	Ref<Renderings::ParticleManager> m_particleManagerRef;

};
