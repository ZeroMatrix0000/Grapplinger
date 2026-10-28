/*
 * FileName:     PlayerModel.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * プレイヤーのモデル
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class Transform;
}
namespace Renderings
{
	class ParticleManager;
	class ICameraScreen;
}
class Player;

// プレイヤーのモデル
class PlayerModel : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	PlayerModel(const ComponentDesc& desc);

	// 初期化処理
	void Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime, Ref<const Player> playerRef);

	// 移動
	void Translate(float elapsedTime, Ref<const Player> playerRef);
	// スケーリング
	void Scaling(float elapsedTime, Ref<const Player> playerRef);
	// 回転
	void Rotate(float elapsedTime, Ref<const Player> playerRef);

	// 映るカメラ画面の参照を追加
	void AddICameraScreenRef(Ref<const Renderings::ICameraScreen> iCameraScreenRef);

	// トランスフォームを設定
	void SetTransform(Ref<const Transform> transformRef);

	// 糸伸ばし時の角度を設定
	void SetWireAngle(float angle) { m_wireAngle.SetTarget(angle); }

	// パーティクル管理の参照を設定
	void SetParticleManagerRef(Ref<Renderings::ParticleManager> particleManagerRef) { m_particleManagerRef = particleManagerRef; }


private:


	/* メンバ変数 */

	// 跳ねる高さ
	float m_bounceHeight;
	// 落下速度係数
	float m_fallSpeedCoefficient;
	// スケール変動の最大値
	float m_scalingMax;
	// ダッシュ時の傾き
	float m_dashAngleMax;

	// 跳ねる周期
	Cycled m_bounceTime;

	// ジャンプまたは落下中の高さ
	Spring<float> m_fallingHeight;
	// スケールの差
	Spring<Math::Vector3> m_scaling;
	// ダッシュ時の傾き
	Spring<float> m_dashAngle;
	// 糸伸ばし時の傾き
	Spring<float> m_wireAngle;

	// 自身のトランスフォームの参照
	Ref<Transform> m_transformRef;

	// シルエットのトランスフォームの参照
	Ref<Transform> m_silhouetteRef;

	// パーティクル管理
	Ref<Renderings::ParticleManager> m_particleManagerRef;

};
