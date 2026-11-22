/*
 * FileName:     Mesh.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/21
 *
 * メッシュ
 */

#pragma once

namespace Libraries
{
	// メッシュ
	struct Mesh
	{

		/* メンバ関数 */

		// コンストラクタ
		Mesh();

		// ロード（失敗したらfalse）
		bool Load(const std::string& filePath);

		// 行列を適用
		void ApplyMatrix(const Math::Matrix& matrix);

		// 非回転直方体を取得
		Math::NonRotatingBox GetNonRotatingBox(size_t index) const;


		/* メンバ変数 */

		// 頂点情報
		std::vector<std::vector<Math::Vector3>> v;

		// 面情報（三角形のみ）
		std::vector<std::vector<std::array<size_t, 3>>> f;

	};
}
