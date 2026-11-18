/*
 * FileName:     FlagManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * ゲームフラグ
 */

#pragma once

#include "IFlagManager.h"

namespace Systems
{
	// ゲームフラグ
	class FlagManager : public IFlagManager
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		FlagManager();


		// ゲームフラグを追加
		void AddFlag(const std::string& flagName, const std::any& flag)
		{
			m_flags.emplace(flagName, flag);
		}


	private:


		/* メンバ関数 */

		// フラグの参照を取得
		Ref<std::any> GetFlagAnyRef(const std::string& flagName) override
		{
			return Ref{ m_flags.at(flagName) };
		}


		/* メンバ変数 */

		// フラグリスト
		std::unordered_map<std::string, std::any> m_flags;

	};
}