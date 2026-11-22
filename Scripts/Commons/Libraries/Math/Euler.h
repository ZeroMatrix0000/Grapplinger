/*
 * FileName:     Euler.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * オイラー角
 */

#pragma once

namespace Libraries
{
	namespace Math
	{
		// オイラー角
		struct Euler
		{

			/* メンバ関数 */

			// コンストラクタ
			constexpr Euler()
				: x{}
				, y{}
				, z{}
			{
			}
			// コンストラクタ
			constexpr Euler(float i)
				: x{ i }
				, y{ i }
				, z{ i }
			{
			}
			// コンストラクタ
			constexpr Euler(float x, float y, float z)
				: x{ x }
				, y{ y }
				, z{ z }
			{
			}
			// コンストラクタ
			constexpr Euler(const Vector3& v)
				: x{ v.x }
				, y{ v.y }
				, z{ v.z }
			{
			}

			// フォーマットを整える
			Euler& Format();

			// 四元数を生成
			Quaternion CreateQuaternion() const { return Quaternion::CreateFromYawPitchRoll(Math::Deg2Rad(y), Math::Deg2Rad(x), Math::Deg2Rad(z)); }


			/* 演算子オーバーロード */

			constexpr Euler& operator+=(const Euler& v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}

			constexpr operator Vector3() const { return Vector3{ x, y, z }; }


			/* 静的関数 */

			// 線形補間
			static Euler Lerp(const Euler& x, const Euler& y, float t);


			/* メンバ変数 */

			float x;
			float y;
			float z;

		};

		/* 演算子オーバーロード */

		constexpr Euler operator+(const Euler& v1, const Euler& v2) { return Euler{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }
		constexpr Euler operator-(const Euler& v1, const Euler& v2) { return Euler{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }
		constexpr Euler operator*(const Euler&  v, float         k) { return Euler{  v.x *    k,  v.y *    k,  v.z    * k }; }
	}
}
