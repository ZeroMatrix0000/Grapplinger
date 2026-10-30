/*
 * FileName:     Slime.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/30
 *
 * スライム
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

// スライム
class Slime : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	Slime(const ComponentDesc& desc);

	// 初期化処理
	void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;


private:


};
