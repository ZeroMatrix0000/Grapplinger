/*
 * FileName:     FlagManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/18
 *
 * システムフラグのインタフェース
 */

#pragma once

#include "OnlyOne.h"

namespace Systems
{
	// システムフラグのインタフェース
	class IFlagManager : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		IFlagManager()
			: OnlyOne{ typeid(IFlagManager) }
		{
		}

		// フラグの参照を取得
		template<typename T>
		Ref<T> GetFlagRef(const std::string& flagName)
		{
			return Ref{ *std::any_cast<T>(GetFlagAnyRef(flagName).Get()) };
		}

		// フラグの参照を取得
		template<typename T>
		T GetFlag(const std::string& flagName)
		{
			return std::any_cast<T>(GetFlagAnyRef(flagName).GetValue());
		}


	private:


		/* メンバ関数 */

		// フラグの参照を取得
		virtual Ref<std::any> GetFlagAnyRef(const std::string& flagName) = 0;

	};
}
