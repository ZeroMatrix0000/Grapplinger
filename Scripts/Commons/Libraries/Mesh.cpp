/*
 * FileName:     Mesh.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * メッシュ
 */

#include "Pch.h"

// コンストラクタ
Libraries::Mesh::Mesh()
	: v{}
	, f{}
{
}

// ロード（失敗したらfalse）
bool Libraries::Mesh::Load(const std::string& filePath)
{
	v.clear();
	f.clear();

	// ファイル
	std::ifstream fileStream{ filePath };
	// 開けなかったら終了
	if (!fileStream.is_open())
	{
		fileStream.close();
		return false;
	}

	bool isV = true;
	v.push_back(std::vector<Math::Vector3>{});

	size_t size = 0;

	while (true)
	{
		// 1行読み込む
		std::string s{};
		std::getline(fileStream, s);
		if (s.empty())
		{
			break;
		}
		std::stringstream ss{ s };

		// 識別子
		std::string id{};
		ss >> id;
		if (id == "v")
		{
			if (!isV)
			{
				size += v.back().size();
				isV = true;
				v.push_back(std::vector<Math::Vector3>{});
			}

			Math::Vector3 vertex{};
			ss >> vertex.x >> vertex.y >> vertex.z;
			v.back().push_back(vertex);
		}
		else if (id == "f")
		{
			if (isV)
			{
				isV = false;
				f.push_back(std::vector<std::array<size_t, 3>>{});
			}

			size_t v1, v2, v3;
			ss >> v1 >> v2 >> v3;
			f.back().push_back(std::array<size_t, 3>{ v1 - size, v2 - size, v3 - size });
		}
	}

	fileStream.close();
	return true;
}

// 行列を適用
void Libraries::Mesh::ApplyMatrix(const Math::Matrix& matrix)
{
	for (auto& vertices : v)
	{
		for (auto& vertex : vertices)
		{
			vertex = Math::Vector3::Transform(vertex, matrix);
		}
	}
}

// 非回転直方体を取得
Math::NonRotatingBox Libraries::Mesh::GetNonRotatingBox(size_t index) const
{
	// 各成分の最小値
	Math::Vector3 min{ Math::INFTY };
	// 各成分の最小値
	Math::Vector3 max{ -Math::INFTY };

	for (const auto& vertex : v.at(index))
	{
		if (vertex.x < min.x) min.x = vertex.x;
		if (vertex.y < min.y) min.y = vertex.y;
		if (vertex.z < min.z) min.z = vertex.z;
		if (vertex.x > max.x) max.x = vertex.x;
		if (vertex.y > max.y) max.y = vertex.y;
		if (vertex.z > max.z) max.z = vertex.z;
	}

	return Math::NonRotatingBox{ (min + max) / 2.0f, max - min };
}
