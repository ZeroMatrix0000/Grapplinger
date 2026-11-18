/*
 * FileName:     ActionSetFlag.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * フラグ設定アクション
 */

#pragma once

#include "Action.h"

namespace Systems
{
	class IFlagManager;
}

// フラグ設定アクション
class ActionSetFlag : public Action
{

public:


	/* メンバ関数 */

	// コンストラクタ
	ActionSetFlag(const ComponentDesc& desc, Ref<Systems::IFlagManager> iFlagManager);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// パラメータを設定
	void SetParams(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 開始処理
	void Enter() override;
	// 更新処理
	void Update(float elapsedTime, Ref<const IGameInput> iGameInput) override {}


private:


	/* メンバ変数 */

	// フラグ名
	std::string m_flagName;
	// 値
	bool m_value;

	// フラグ管理
	Ref<Systems::IFlagManager> m_iFlagManager;

};