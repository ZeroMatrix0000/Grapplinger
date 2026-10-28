/*
 * FileName:     Ptr.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * 自作参照型（参照先変更可能・nullptr禁止）
 */

#pragma once

namespace Libraries
{
	// 自作参照型（参照先変更可能・nullptr許容）
	template<typename T>
	class Ptr
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		explicit Ptr()
			: m_ptr{}
		{
		}
		// コンストラクタ
		explicit Ptr(T* ptr)
			: m_ptr{ ptr }
		{
		}

		// ポインタの取得
		T* Get() const { return m_ptr; }
		// アドレスの取得
		T* const* GetAddress() const { return &m_ptr; }

		// 定数化
		Ptr<const T> Const() const { return Ptr<const T>{ *m_ptr }; }

		// キャスト
		template<typename U>
		Ptr<U> Cast() const { return Ptr<U>{ static_cast<U*>(m_ptr) }; }

		// 強制キャスト
		template<typename U>
		Ptr<U> ReinterpretCast() const { return Ptr<U>{ reinterpret_cast<U*>(m_ptr) }; }

		// 自分自身の参照
		Ptr<Ptr<T>> Nest() { return Ptr<Ptr<T>>{ this }; }


		/* 演算子オーバーロード */

		T* operator->() const { return m_ptr; }

		Ptr& operator=(T* ptr)
		{
			m_ptr = ptr;
			return *this;
		}


	private:


		/* メンバ変数 */

		// ポインタ
		T* m_ptr;

	};

	template<typename T>
	bool operator==(const Ptr<T>& a, const Ptr<T>& b) { return a.Get() == b.Get(); }

}

namespace std
{
	// ハッシュ関数
	template<typename T>
	struct hash<Libraries::Ptr<T>>
	{
		size_t operator()(const Libraries::Ptr<T>& ptr) const
		{
			return hash<T*>()(ptr.Get());
		}
	};
}
