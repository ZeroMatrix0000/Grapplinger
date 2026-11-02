/*
 * FileName:     SlimeManager.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/02
 *
 * スライム管理
 */

#include "Pch.h"
#include "SlimeManager.h"

#include "Slime.h"
#include "../Player/Player.h"
#include "../Player/States/PlayerStates.h"
#include "Scripts/GameObjects/Particles/SlimeDefeatParticle.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
SlimeManager::SlimeManager(const ComponentDesc& desc)
	: Component{ desc }
	, m_slimeRefList{}
	, m_transformRef{ GetOwner()->GetNullComponent<Transform>() }
	, m_particleManagerRef{ GetOwner()->GetNullComponent<Renderings::ParticleManager>() }
{
}

// 初期化処理
void SlimeManager::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_transformRef = GetOwner()->GetComponent<Transform>();
	m_particleManagerRef = GetOwner()->GetComponent<Renderings::ParticleManager>();

	// カメラ画面の参照
	Ref iCameraScreenRef = GetOwner()->GetNullComponent<Renderings::ICameraScreen>().Const();

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_slimeRefList }, "SlimeList");
	serializer.AddParameter(iCameraScreenRef.Nest(), "CameraScreen");
	serializer.Load(json);

	if (iCameraScreenRef != GetOwner()->GetNullComponent<Renderings::ICameraScreen>().Const())
	{
		m_particleManagerRef->AddICameraScreenRef(iCameraScreenRef);
	}
}

// 更新処理
void SlimeManager::Update(float elapsedTime)
{
	// スライムごとに更新
	for (Ref slimeRef : m_slimeRefList)
	{
		slimeRef->Update(elapsedTime);
	}

	// パーティクル管理の更新
	m_particleManagerRef->Update(elapsedTime);
}

// プレイヤーとの当たり判定
void SlimeManager::PlayerCollide(Ref<Player> playerRef)
{
	// プレイヤーの直方体
	const Math::Box& playerBox = playerRef->GetBox();

	for (int i = 0; i < m_slimeRefList.size(); i++)
	{
		// 参照
		Ref slimeRef = m_slimeRefList.at(i);
		// スライムの直方体
		const Math::Box& slimeBox = slimeRef->GetBox();

		// 外接する非回転直方体同士が触れていなければ何もしない
		if (!Math::Geometry::IsCollide(playerBox.CreateNonRotatingBox(), slimeBox.CreateNonRotatingBox()))
		{
			continue;
		}

		// 距離の方向
		Math::Vector3 direction{};
		// 距離
		float distance = Math::Geometry::Distance(playerBox, slimeBox, &direction);

		if (distance < 0.0f)
		{
			// 座標補正
			playerRef->PositionCorrect(distance, direction);
			// プレイヤーが糸伸ばし中かつ移動中なら消す
			if (playerRef->IsState<PlayerStateExpandWire>() && playerRef->GetMoveVelocity() != Math::Vector3::Zero)
			{
				// パーティクルを再生
				m_transformRef->SetPosition(slimeBox.position);
				m_particleManagerRef->Play<SlimeDefeatParticle>();
				m_slimeRefList.erase(m_slimeRefList.begin() + i);
				Destroy(slimeRef->GetOwner());
				i--;
			}
		}
	}
}

// 当たり判定を取得
std::vector<Math::Box> SlimeManager::GetBoxes() const
{
	std::vector<Math::Box> boxes{};
	for (Ref slimeRef : m_slimeRefList)
	{
		boxes.push_back(slimeRef->GetBox());
	}
	return boxes;
}
