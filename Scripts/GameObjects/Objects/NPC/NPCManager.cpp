/*
 * FileName:     NPCManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * NPC管理
 */

#include "Pch.h"
#include "NPCManager.h"

#include "NPC.h"
#include "../Player/Player.h"
#include "../Player/States/PlayerStates.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/Renderings/ICameraScreen.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Renderings/Text/Text.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/GameObjects/UIs/InteractCursor/InteractCursor.h"

// コンストラクタ
NPCManager::NPCManager(const ComponentDesc& desc)
	: Component{ desc }
	, m_isHideCursor{}
	, m_canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>().Const() }
	, m_nPCRefs{}
	, m_interactCursorRefs{}
	, m_pFocusedNPC{}
{
}

// 初期値を設定
void NPCManager::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(m_canvasRef.Nest(), "Canvas");
	serializer.Load(json);
}

// 初期化処理
void NPCManager::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	// キャンバスを設定
	for (Ref interactCursorRef : m_interactCursorRefs)
	{
		interactCursorRef->SetCanvasRef(m_canvasRef);
	}
}

// 更新処理
void NPCManager::Update(float elapsedTime)
{
	for (int i = 0; i < m_nPCRefs.size(); i++)
	{
		m_nPCRefs.at(i)->Update(elapsedTime);
		m_interactCursorRefs.at(i)->Update(elapsedTime, m_nPCRefs.at(i).Get() == m_pFocusedNPC && !m_isHideCursor);
	}
}

// 向きを設定
void NPCManager::SetRotation(const Math::Vector3& playerPosition)
{
	// 距離の最小
	float distanceMin = Math::INFTY;

	m_pFocusedNPC = nullptr;

	for (Ref nPCRef : m_nPCRefs)
	{
		// スピン中ならスキップ
		if (nPCRef->IsSpinning())
		{
			continue;
		}

		// プレイヤーとの距離
		float distance = Math::Geometry::Distance(playerPosition, nPCRef->GetSphere());
		// 距離がある程度近かったら
		if (distance < 0.0f)
		{
			// 向く方向
			Math::Vector3 direction = playerPosition - nPCRef->GetPosition();
			// Y軸方向は無視
			direction.y = 0.0f;
			// 正規化
			direction.Normalize();
			// 回転
			Math::Quaternion rotation = Math::Quaternion::FromToRotation(Math::Vector3::Backward, direction);
			nPCRef->SetRotation(&rotation);

			// 最も近いNPCを記録
			if (distance < distanceMin)
			{
				distanceMin = distance;
				m_pFocusedNPC = nPCRef.Get();
			}
		}
		else
		{
			nPCRef->SetRotation();
		}
	}
}

// カーソルを設定
void NPCManager::SetCursor(Ref<const Renderings::ICameraScreen> iCameraScreenRef)
{
	for (int i = 0; i < m_nPCRefs.size(); i++)
	{
		const Math::Box& box = m_nPCRefs.at(i)->GetBox();

		Math::Vector2 screenPosition = Math::Geometry::Projection
		(
			box.position + box.size * Math::Vector3::Up / 2.0f,
			iCameraScreenRef->GetViewMatrix(),
			iCameraScreenRef->GetProjectionMatrix(),
			m_interactCursorRefs.at(i)->GetCanvas().GetSize()
		);

		m_interactCursorRefs.at(i)->SetPosition(screenPosition);
	}
}

// プレイヤーとの当たり判定
void NPCManager::PlayerCollide(Ref<Player> playerRef)
{
	// 直方体
	const Math::Box& playerBox = playerRef->GetBox();

	for (Ref nPCRef : m_nPCRefs)
	{
		const Math::Box& nPCBox = nPCRef->GetBox();

		// 外接する非回転直方体同士が触れていなければ何もしない
		if (!Math::Geometry::IsCollide(playerBox.CreateNonRotatingBox(), nPCBox.CreateNonRotatingBox()))
		{
			continue;
		}

		// 距離の方向
		Math::Vector3 direction{};
		// 距離
		float distance = Math::Geometry::Distance(playerBox, nPCBox, &direction);

		if (distance < 0.0f)
		{
			// 座標補正
			playerRef->PositionCorrect(distance, direction);
			// プレイヤーが糸伸ばし中かつ移動中ならスピン
			if (playerRef->IsState<PlayerStateExpandWire>() && playerRef->GetMoveVelocity() != Math::Vector3::Zero)
			{
				// 左側に当たったら時計回り
				nPCRef->Spin(Math::Vector3::Up.Dot(playerRef->GetMoveVelocity().Cross(nPCBox.position - playerBox.position)) > 0.0f);
			}
		}
	}
}

// 当たり判定を取得
std::vector<Math::Box> NPCManager::GetBoxes() const
{
	std::vector<Math::Box> boxes{};
	for (Ref nPCRef : m_nPCRefs)
	{
		boxes.push_back(nPCRef->GetBox());
	}
	return boxes;
}

// NPC を追加
void NPCManager::AddNPC(Ref<NPC> nPCRef)
{
	m_nPCRefs.push_back(nPCRef);
	Ref interactCursorRef = Instantiate("Prefab_InteractCursor")->GetComponent<InteractCursor>();
	m_interactCursorRefs.push_back(interactCursorRef);
	interactCursorRef->SetName(nPCRef->GetName());
	// キャンバスを設定
	interactCursorRef->SetCanvasRef(m_canvasRef);
}
