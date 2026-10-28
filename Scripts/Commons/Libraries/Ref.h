/*
 * FileName:     Ref.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 自作参照型（参照先変更可能・nullptr禁止）
 */

#pragma once

namespace Libraries
{
	// 自作参照型（参照先変更可能・nullptr禁止）
	template<typename T>
	class Ref
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		explicit Ref(T& ref)
			: m_ptr{ &ref }
		{
			Utility::ThrowIf(!&ref);
		}

		// 参照の取得
		T& GetValue() const { return *m_ptr; }
		// ポインタの取得
		T* Get() const { return m_ptr; }
		// アドレスの取得
		T* const* GetAddress() const { return &m_ptr; }

		// 定数化
		Ref<const T> Const() const { return Ref<const T>{ *m_ptr }; }

		// キャスト
		template<typename U>
		Ref<U> Cast() const { return Ref<U>{ *static_cast<U*>(m_ptr) }; }

		// 強制キャスト
		template<typename U>
		Ref<U> ReinterpretCast() const { return Ref<U>{ *reinterpret_cast<U*>(m_ptr) }; }

		// 自分自身の参照
		Ref<Ref<T>> Nest() { return Ref<Ref<T>>{ *this }; }

		
		/* 演算子オーバーロード */

		T* operator->() const { return m_ptr; }

		Ref& operator=(T& ref)
		{
			m_ptr = &ref;
			return *this;
		}


	private:


		/* メンバ変数 */

		// ポインタ
		T* m_ptr;

	};

	template<typename T>
	bool operator==(const Ref<T>& a, const Ref<T>& b) { return a.Get() == b.Get(); }

	// ハッシュ関数
	
}

namespace std
{
	template<typename T>
	struct hash<Libraries::Ref<T>>
	{
		size_t operator()(const Libraries::Ref<T>& ref) const
		{
			return hash<T*>()(ref.Get());
		}
	};
}
