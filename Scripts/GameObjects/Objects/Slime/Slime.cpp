/*
 * FileName:     Slime.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/30
 *
 * スライム
 */

#include "Pch.h"
#include "Slime.h"

// コンストラクタ
Slime::Slime(const ComponentDesc& desc)
	: Component{ desc }
{
}

// 初期化処理
void Slime::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
}
