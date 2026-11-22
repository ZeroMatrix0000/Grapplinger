/*
 * FileName:     Goal.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/22
 *
 * ゴール
 */

#include "Pch.h"
#include "Goal.h"

#include "../Player/Player.h"
#include "../Player/States/PlayerStates.h"
#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/GameObjects/Particles/GoalParticle.h"
#include "Scripts/GameObjects/Particles/GoalDefeatParticle.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Colliders/SphereCollider.h"
#include "Scripts/Commons/Renderings/Model3D/Model3D.h"
#include "Scripts/Commons/Renderings/Particle/ParticleManager.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

// コンストラクタ
Goal::Goal(const ComponentDesc& desc)
	: Component{ desc }
	, m_floatSize{}
	, m_floatTimer{}
	, m_rotateTimer{}
	, m_particleTime{}
	, m_hp{}
	, m_basePosition{}
	, m_sway{}
	, m_particleTimer{}
	, m_isHide{}
	, m_transformRef{ GetOwner()->GetNullComponent<Transform>() }
	, m_boxColliderRef{ GetOwner()->GetNullComponent<Colliders::BoxCollider>() }
	, m_sphereColliderRef{ GetOwner()->GetNullComponent<Colliders::SphereCollider>() }
	, m_particleManagerRef{ GetOwner()->GetNullComponent<Renderings::ParticleManager>() }
	, m_iCameraScreenRef{ GetOwner()->GetNullComponent<Renderings::ICameraScreen>().Const() }
{
}

// 初期値を設定
void Goal::SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	float floatTime = m_floatTimer.GetMax();
	float rotateTime = m_rotateTimer.GetMax();

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_floatSize }, "FloatSize");
	serializer.AddParameter(Ref{ floatTime }, "FloatTime");
	serializer.AddParameter(Ref{ rotateTime }, "RotateTime");
	serializer.AddParameter(Ref{ m_particleTime }, "ParticleTime");
	serializer.AddParameter(Ref{ m_hp }, "Hp");
	serializer.AddParameter(m_iCameraScreenRef.Nest(), "CameraScreen");
	serializer.Load(json);

	m_floatTimer.Initialize(0.0f, 0.0f, floatTime);
	m_rotateTimer.Initialize(0.0f, 0.0f, rotateTime);
}

// 初期化処理
void Goal::Initalize(Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_transformRef = GetOwner()->GetComponent<Transform>();
	m_boxColliderRef = GetOwner()->GetComponent<Colliders::BoxCollider>();
	m_sphereColliderRef = GetOwner()->GetComponent<Colliders::SphereCollider>();
	m_particleManagerRef = GetOwner()->GetComponent<Renderings::ParticleManager>();
	m_boxColliderRef->ApplyTransform();
	m_sphereColliderRef->ApplyTransform();

	// 基準位置を保持
	m_basePosition = m_transformRef->GetPosition();
	// 揺れの初期化
	m_sway.SetMovement(Math::Vector3::Zero, 0.0f);

	// カメラを追加
	GetOwner()->GetComponent<Renderings::Model3D>()->AddICameraScreenRef(m_iCameraScreenRef);
	m_boxColliderRef->AddICameraScreenRef(m_iCameraScreenRef);
	m_sphereColliderRef->AddICameraScreenRef(m_iCameraScreenRef);
	m_particleManagerRef->AddICameraScreenRef(m_iCameraScreenRef);

	// ターゲット管理に追加
	iGameObjectFinder->Find("TargetPoint")->AcceptMessage("AddTargetObject", m_sphereColliderRef);
}

// 更新処理
void Goal::Update(float elapsedTime)
{
	m_floatTimer += elapsedTime;
	m_rotateTimer += elapsedTime;
	m_sway.Tick(elapsedTime);

	// 移動回転
	m_transformRef->SetPosition
	(
		m_basePosition + Math::Vector3::Up * m_floatSize * Math::Sin(m_floatTimer.GetRatio() * 2.0f * Math::PI) + m_sway.GetMovement()
	);
	m_transformRef->SetRotation(Math::Euler{ m_rotateTimer.GetRatio() * 360.0f }.CreateQuaternion());

	m_boxColliderRef->ApplyTransform();
	m_sphereColliderRef->ApplyTransform();


	// パーティクルの更新
	m_particleTimer += elapsedTime;
	if (m_particleTimer >= m_particleTime && !m_isHide)
	{
		m_particleTimer -= m_particleTime;
		m_particleManagerRef->Play<GoalParticle>();
	}
	m_particleManagerRef->Update(elapsedTime);
}

// プレイヤーとの当たり判定
void Goal::PlayerCollide(Ref<Player> playerRef)
{
	// プレイヤーの直方体
	const Math::Box& playerBox = playerRef->GetBox();
	// ゴールの直方体
	const Math::Box& goalBox = m_boxColliderRef->GetWorldBox();

	// 外接する非回転直方体同士が触れていなければ何もしない
	if (!Math::Geometry::IsCollide(playerBox.CreateNonRotatingBox(), goalBox.CreateNonRotatingBox()))
	{
		return;
	}

	// 距離の方向
	Math::Vector3 direction{};
	// 距離
	float distance = Math::Geometry::Distance(playerBox, goalBox, &direction);

	if (distance < 0.0f)
	{
		// プレイヤーが糸伸ばし中かつ移動中なら消す
		if (playerRef->IsState<PlayerStateExpandWire>() && playerRef->GetMoveVelocity() != Math::Vector3::Zero)
		{
			// 揺れていないなら体力を減らす
			if (!m_sway.IsMoving())
			{
				m_hp--;

				Math::Vector3 moveDirection = direction;
				moveDirection.Normalize();
				moveDirection;

				m_sway.SetMovement(moveDirection, Math::Vector3::Zero, 0.25f, Easing::Type::Expo, Easing::InOut::Out);
			}
		}
		// プレイヤーの敗北
		else if (!playerRef->IsState<PlayerStateDefeat>())
		{
			playerRef->PositionCorrect(distance, direction);
		}
	}
}

// 隠す
void Goal::Hide()
{
	m_isHide = true;
	m_transformRef->SetScale(Math::Vector3::Zero);
	// パーティクルの再生
	m_particleManagerRef->Play<GoalDefeatParticle>();
}
