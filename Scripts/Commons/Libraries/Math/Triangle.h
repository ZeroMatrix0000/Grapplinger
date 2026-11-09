/*
 * FileName:     Triangle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/09
 *
 * 三角形
 */

#pragma once

namespace Libraries
{
	namespace Math
	{
		// 三角形
		struct Triangle
		{

			/* メンバ関数 */

			// コンストラクタ
			constexpr Triangle()
				: v1{ Vector3::Zero }
				, v2{ Vector3::Zero }
				, v3{ Vector3::Zero }
			{
			}
			// コンストラクタ
			constexpr Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3)
				: v1{ v1 }
				, v2{ v2 }
				, v3{ v3 }
			{
			}

			// 重心を取得
			Vector3 GetCenter() const { return (v1 + v2 + v3) / 3.0f; }
			// 法線を取得
			Vector3 GetNormal() const;

			// 外接する非回転直方体を生成
			NonRotatingBox CreateNonRotatingBox() const;


			/* 演算子オーバーロード */

			Triangle& operator+=(const Vector3& v);


			/* メンバ変数 */

			// 頂点1
			Vector3 v1;
			// 頂点2
			Vector3 v2;
			// 頂点3
			Vector3 v3;

		};
	}
}
