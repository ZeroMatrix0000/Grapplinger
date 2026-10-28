/*
 * FileName:     TargetPoint.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * ターゲット照準
 */

#include "Pch.h"
#include "TargetPoint.h"

#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/Colliders/BoxCollider.h"
#include "Scripts/Commons/Colliders/SphereCollider.h"
#include "Scripts/Commons/Colliders/MeshCollider.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Renderings/ICameraScreen.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Renderings/Canvas.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
TargetPoint::TargetPoint(const ComponentDesc& desc)
	: Component{ desc }
	, m_targetDistance{}
	, m_rayLength{}
	, m_brightInterval{}
	, m_brightTimer{}
	, m_info{}
	, m_transformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_imageRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_playerRef{ GetOwner()->GetNullComponent<Colliders::BoxCollider>() }
	, m_iCameraScreenRef{ GetOwner()->GetNullComponent<Renderings::ICameraScreen>() }
	, m_targetObjectsRef{}
	, m_groundsRef{}
{
}

// 初期化処理
void TargetPoint::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_imageRef = GetOwner()->GetComponent<Renderings::Image>();
	m_transformRef = GetOwner()->GetComponent<RectTransform>();

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_targetDistance }, "TargetDistance");
	serializer.AddParameter(Ref{ m_rayLength }, "RayLength");
	serializer.AddParameter(Ref{ m_brightInterval }, "BrightInterval");
	serializer.AddParameter(m_playerRef.Nest(), "Player");
	serializer.AddParameter(m_iCameraScreenRef.Nest(), "CameraScreen");
	serializer.AddParameter(Ref{ m_targetObjectsRef }, "TargetObjects");
	serializer.AddParameter(Ref{ m_groundsRef }, "Grounds");
	serializer.Load(json);
}

// 更新処理
void TargetPoint::Update(float elapsedTime)
{
	// 点滅
	m_brightTimer += elapsedTime;
	if (m_brightTimer >= m_brightInterval)
	{
		m_brightTimer -= m_brightInterval;
		Math::Color color = Math::Color{ m_imageRef->GetColor() == Math::Color{ DirectX::Colors::Red } ? DirectX::Colors::Orange : DirectX::Colors::Red };
		m_imageRef->SetColor(color);
	}

	m_info.position = Math::Vector3::Zero;
	m_info.type = Type::Air;

	// プレイヤーの座標
	const Math::Vector3& playerPosition = m_playerRef->GetWorldBox().position;

	// カメラの目線の位置
	Math::Vector3 eyePosition = m_iCameraScreenRef->GetEyePosition();
	// カメラの目線の方向
	Math::Vector3 eyeDirection = m_iCameraScreenRef->GetEyeDirection();

	// 距離の最小
	float distanceMin = 0.0f;

	// 画面の中心に最も近いオブジェクトを探す
	{

		for (Ref targetObjectRef : m_targetObjectsRef)
		{
			// 球
			const Math::Sphere& sphere = targetObjectRef->GetWorldSphere();

			// カメラの中心から発射されるレイとの距離
			float distance = Math::Geometry::Distance
			(
				sphere,
				Math::Line{ eyePosition, eyePosition + eyeDirection * m_rayLength }
			);

			// 最も距離が短いターゲットを記録
			if (distance < distanceMin && eyeDirection.Dot(sphere.centerPosition - playerPosition) > 0.0f)
			{
				// プレイヤーとの距離
				float playerDistance = Math::Vector3::Distance(playerPosition, sphere.centerPosition);
				if (playerDistance <= m_targetDistance)
				{
					distanceMin = distance;
					m_info.position = sphere.centerPosition;
					m_info.type = Type::Object;
				}
			}
		}
	}

	// ターゲットがオブジェクトなら
	if (m_info.type == Type::Object)
	{
		Math::Vector2 screenPosition = Math::Geometry::Projection
		(
			m_info.position,
			m_iCameraScreenRef->GetViewMatrix(),
			m_iCameraScreenRef->GetProjectionMatrix(),
			m_imageRef->GetCanvasRef()->GetSize()
		);

		m_transformRef->SetPosition(screenPosition);
		return;
	}

	distanceMin = m_targetDistance;

	// 地面なら探す
	for (Ref groundRef : m_groundsRef)
	{
		const auto& mesh = groundRef->GetWorldMesh();

		for (const auto& face : mesh.f)
		{
			// メッシュの三角形
			Math::Triangle triangle = Math::Triangle{ mesh.v.at(face.at(0)), mesh.v.at(face.at(1)) , mesh.v.at(face.at(2)) };

			// カメラの中心と三角形の方向指定最短距離
			float distance = Math::Geometry::AngledDistance(eyePosition, triangle, eyeDirection);
			if (distance == Math::INFTY)
			{
				continue;
			}

			// 交点
			Math::Vector3 intersect = eyePosition + eyeDirection * distance;

			// プレイヤーより手前側にあるなら
			if (eyeDirection.Dot(intersect - playerPosition) > 0.0f)
			{
				float playerDistance = Math::Vector3::Distance(playerPosition, intersect);
				if (playerDistance <= distanceMin)
				{
					distanceMin = playerDistance;
					m_info.position = intersect;
					m_info.type = Type::Ground;
				}
			}
		}
	}

	// ターゲットが地面なら
	if (m_info.type == Type::Ground)
	{
		m_transformRef->SetPosition(m_imageRef->GetCanvasRef()->GetSize() / 2.0f);
		return;
	}

	distanceMin = Math::Geometry::AngledDistance(eyePosition, Math::Sphere{ playerPosition, m_targetDistance }, eyeDirection).second;
	m_info.position = eyePosition + eyeDirection * distanceMin;
	m_transformRef->SetPosition(Math::Vector2{ -10.0f, -10.0f });
}

// ターゲットを表示
void TargetPoint::Appear(bool isAppear)
{
	m_imageRef->SetColor(isAppear ? DirectX::Colors::Red : DirectX::Colors::Transparent);
}

// ターゲット情報を取得
const TargetPoint::Info& TargetPoint::GetTargetInfo()
{
	// プレイヤーの座標
	const Math::Vector3& playerPosition = m_playerRef->GetWorldBox().position;
	// プレイヤーからターゲットを指す方向
	Math::Vector3 direction = m_info.position - playerPosition;

	// 距離
	float distanceMin = direction.Length();

	direction /= distanceMin;

	for (Ref groundRef : m_groundsRef)
	{
		const auto& mesh = groundRef->GetWorldMesh();

		for (const auto& face : mesh.f)
		{
			// メッシュの三角形
			Math::Triangle triangle = Math::Triangle{ mesh.v.at(face.at(0)), mesh.v.at(face.at(1)) , mesh.v.at(face.at(2)) };

			// カメラの中心と三角形の方向指定最短距離
			float distance = Math::Geometry::AngledDistance(playerPosition, triangle, direction);

			if (distance > 0.0f && distance < distanceMin)
			{
				distanceMin = distance;
				m_info.position = playerPosition + direction * distance;
				m_info.type = Type::Ground;
			}
		}
	}

	return m_info;
}
