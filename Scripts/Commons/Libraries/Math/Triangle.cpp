/*
 * FileName:     Triangle.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/09
 *
 * 三角形
 */

#include "Pch.h"

// 法線を取得
Math::Vector3 Libraries::Math::Triangle::GetNormal() const
{
	Vector3 normal = (v2 - v1).Cross(v3 - v2);
	if (normal.Length() < Math::EPSILON)
	{
		return Vector3::Zero;
	}
	normal.Normalize();
	return normal;
}

// 外接する非回転直方体を生成
Math::NonRotatingBox Libraries::Math::Triangle::CreateNonRotatingBox() const
{
	Vector3 vertex[3] = { v1, v2, v3 };

	// 各成分の最小値
	Vector3 min{ Math::INFTY };
	// 各成分の最小値
	Vector3 max{ -Math::INFTY };
	for (size_t i = 0; i < 3; i++)
	{
		if (vertex[i].x < min.x) min.x = vertex[i].x;
		if (vertex[i].y < min.y) min.y = vertex[i].y;
		if (vertex[i].z < min.z) min.z = vertex[i].z;
		if (vertex[i].x > max.x) max.x = vertex[i].x;
		if (vertex[i].y > max.y) max.y = vertex[i].y;
		if (vertex[i].z > max.z) max.z = vertex[i].z;
	}

	return NonRotatingBox{ (min + max) / 2.0f, max - min };
}

Math::Triangle& Libraries::Math::Triangle::operator+=(const Vector3& v)
{
	v1 += v;
	v2 += v;
	v3 += v;
	return *this;
}
