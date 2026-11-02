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
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
SlimeManager::SlimeManager(const ComponentDesc& desc)
	: Component{ desc }
	, m_slimeRefList{}
{
}

// 初期化処理
void SlimeManager::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_slimeRefList }, "SlimeList");
	serializer.Load(json);
}

// 更新処理
void SlimeManager::Update(float elapsedTime)
{
	// スライムごとに更新
	for (Ref slimeRef : m_slimeRefList)
	{
		slimeRef->Update(elapsedTime);
	}
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
