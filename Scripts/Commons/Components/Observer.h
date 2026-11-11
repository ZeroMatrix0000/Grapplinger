/*
 * FileName:     Observer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * メッセージを受け取るクラス
 */

#pragma once

namespace Components
{
	// メッセージを受け取るクラス
	class Observer
	{

	public:


		/* メンバ関数 */

		// デストラクタ
		virtual ~Observer() = default;

		// メッセージを受け取る
		virtual void AcceptMessage(const std::string& message, const std::any& value) {}

	};
}
