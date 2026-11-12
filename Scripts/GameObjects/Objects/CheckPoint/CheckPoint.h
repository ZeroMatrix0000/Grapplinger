/*
 * FileName:     CheckPoint.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * チェックポイント
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Colliders
{
	class BoxCollider;
}
namespace Renderings
{
	class ICameraScreen;
}

// チェックポイント
class CheckPoint : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	CheckPoint(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// カメラ画面の参照を追加
	void AddICameraScreenRef(Ref<const Renderings::ICameraScreen> iCameraScreenRef) const;

	// 優先順位を取得
	int GetOrder() const { return m_order; }
	// 当たり判定を取得
	const Math::Box& GetBox() const;
	// 座標を取得
	const Math::Vector3& GetPosition() const;


private:


	/* メンバ変数 */

	// 優先順位
	int m_order;

	// 当たり判定の参照
	Ref<Colliders::BoxCollider> m_boxColliderRef;

};
