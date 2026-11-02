/*
 * FileName:     SlimeManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライム管理
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

class Slime;
class Player;

// NPC管理
class SlimeManager : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	SlimeManager(const ComponentDesc& desc);

	// 初期化処理
	void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

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

};
