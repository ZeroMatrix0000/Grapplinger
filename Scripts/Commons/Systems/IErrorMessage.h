/*
 * FileName:     IErrorMessage.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * エラーメッセージのインタフェース
 */

#pragma once

#include "../Systems/OnlyOne.h"

namespace Systems
{
	// エラーメッセージのインタフェース
	class IErrorMessage : public OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		IErrorMessage()
			: OnlyOne{ typeid(IErrorMessage) }
		{
			s_instance = this;
		}
		// デストラクタ
		virtual ~IErrorMessage() = default;

		// メッセージを追加
		virtual void AddMessage(const std::wstring& text) = 0;


		/* 静的関数 */

		// インスタンスを取得
		static Ref<IErrorMessage> GetInstance() { return Ref{ *s_instance }; }


	private:


		/* 静的変数 */

		// インスタンス
		static IErrorMessage* s_instance;

	};
}

inline Systems::IErrorMessage* Systems::IErrorMessage::s_instance = nullptr;
