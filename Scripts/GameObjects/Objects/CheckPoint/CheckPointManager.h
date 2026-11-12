/*
 * FileName:     CheckPointManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * チェックポイント管理
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Renderings
{
	class ICameraScreen;
}
class CheckPoint;

// チェックポイント
class CheckPointManager : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	CheckPointManager(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// チェック
	int Check(const Math::Box& player);

	// 座標を取得
	const Math::Vector3& GetPosition(int index);

	// メッセージを受け取る
	void AcceptMessage(const std::string& message, const std::any& value) override;


private:


	/* メンバ変数 */

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::ICameraScreen> m_iCameraScreenRef;

	// チェックポイントの参照リスト
	std::vector<Ref<const CheckPoint>> m_checkPointRefs;


};
