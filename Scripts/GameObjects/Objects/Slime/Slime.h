/*
 * FileName:     Slime.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * スライム
 */

#pragma once

#include "SlimeInternals.h"
#include "Scripts/Commons/Components/Component.h"
#include "Scripts/Commons/Systems/State.h"

namespace Renderings
{
	class ICameraScreen;
}

// スライム
class Slime : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	Slime(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 当たり判定を取得
	const Math::Box& GetBox() const;


private:


	/* メンバ関数 */

	// 状態を変更
	void SetState(std::unique_ptr<Systems::State<SlimeInternals>> state);


	/* メンバ関数 */

	// 内部データ
	SlimeInternals m_internals;

	// 状態
	std::unique_ptr<Systems::State<SlimeInternals>> m_currentState;

	// カメラ画面のインタフェースの参照
	Ref<const Renderings::ICameraScreen> m_iCameraScreenRef;

};
