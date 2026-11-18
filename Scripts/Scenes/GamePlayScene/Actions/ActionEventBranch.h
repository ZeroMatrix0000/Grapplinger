/*
 * FileName:     ActionEventBranch.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * イベント分岐アクション
 */

#pragma once

#include "Action.h"

namespace Systems
{
	class IFlagManager;
}
class ActionManager;

// イベント分岐アクション
class ActionEventBranch : public Action
{

public:


	/* メンバ関数 */

	// コンストラクタ
	ActionEventBranch(const ComponentDesc& desc, Ref<Systems::IFlagManager> iFlagManager);

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
	// falseイベント名
	std::string m_falseEventName;
	// trueイベント名
	std::string m_trueEventName;

	// フラグ管理
	Ref<Systems::IFlagManager> m_iFlagManager;

	// アクション管理
	Ref<ActionManager> m_actionManager;

};