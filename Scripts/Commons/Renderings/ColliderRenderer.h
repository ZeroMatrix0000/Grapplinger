/*
 * FileName:     ColliderRenderer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 当たり判定描画
 */

#pragma once

#include "IColliderRenderer.h"

namespace Renderings
{
	// 当たり判定描画
	class ColliderRenderer : public IColliderRenderer
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		ColliderRenderer();

		// 初期化処理
		void Initialize
		(
			Ref<ID3D11Device5> deviceRef,
			Ref<ID3D11DeviceContext4> contextRef,
			Ref<const DirectX::CommonStates> commonStatesRef
		);
		// 描画処理
		void Render();

		// 長方形の当たり判定の参照を追加
		void AddPBoxCollider(Ref<const Colliders::BoxCollider> boxColliderRef) override;
		// 長方形の当たり判定の参照を削除
		void RemovePBoxCollider(Ref<const Colliders::BoxCollider> boxColliderRef) override;

		// 球の当たり判定の参照を追加
		void AddPSphereCollider(Ref<const Colliders::SphereCollider> sphereColliderRef) override;
		// 球の当たり判定の参照を削除
		void RemovePSphereCollider(Ref<const Colliders::SphereCollider> sphereColliderRef) override;

		// メッシュの当たり判定の参照を追加
		void AddPMeshCollider(Ref<const Colliders::MeshCollider> meshColliderRef) override;
		// メッシュの当たり判定の参照を削除
		void RemovePMeshCollider(Ref<const Colliders::MeshCollider> meshColliderRef) override;


	private:


		/* メンバ関数 */

		// 直方体を描画
		void DrawBox(const Math::Box& box, const Math::Color& color) const;
		// 球を描画
		void DrawSphere(const Math::Sphere& sphere, const Math::Color& color, const Math::Vector3& eyePosition, int segmentCount) const;
		// 円を描画
		void DrawCircle(const Math::Circle& circle, const Math::Color& color, int segmentCount) const;
		// 三角形を描画
		void DrawTriangle(const Math::Triangle& triangle, const Math::Color& color) const;
		// メッシュを描画
		void DrawMesh(const Mesh& mesh, const Math::Color& color) const;


		/* メンバ変数 */

		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
		// ベーシックエフェクト
		std::unique_ptr<DirectX::BasicEffect>                                  m_basicEffect;

		// 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		// 長方形の当たり判定の参照リスト
		std::vector<Ref<const Colliders::BoxCollider>>    m_boxColliderRefs;
		// 球の当たり判定の参照リスト
		std::vector<Ref<const Colliders::SphereCollider>> m_sphereColliderRefs;
		// メッシュの当たり判定の参照リスト
		std::vector<Ref<const Colliders::MeshCollider>>   m_meshColliderRefs;

		// デバイスコンテキスト
		ID3D11DeviceContext4*        m_pContext;
		// コモンステート
		const DirectX::CommonStates* m_pCommonStates;

	};
}
