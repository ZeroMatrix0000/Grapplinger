/*
 * FileName:     ParticleManager.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/26
 *
 * パーティクル管理
 */

#include "Pch.h"
#include "ParticleManager.h"

#include "../Model3D/IModel3DRenderer.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Renderings/CameraScreen.h"
#include "Scripts/Commons/Systems/IResources.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

Renderings::ParticleManager::ParticleManager(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRendererRef, Ref<Systems::IResources> iResourcesRef)
	: Component{ desc }
	, m_particles{}
	, m_transformRef{ GetOwner()->GetNullComponent<Transform>().Const() }
	, m_iCameraScreenRefs{}
	, m_iModelRendererRef{ iModelRendererRef }
	, m_iResourcesRef{ iResourcesRef }
{
}

// 初期化処理
void Renderings::ParticleManager::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinderRef)
{
	m_transformRef = GetOwner()->GetComponent<Transform>().Const();

	Systems::JsonSerializer serializer{ iGameObjectFinderRef };
	serializer.AddParameter(&m_iCameraScreenRefs, "CameraScreens");
	serializer.Load(json);
}

// 更新処理
void Renderings::ParticleManager::Update(float elapsedTime)
{
	// パーティクルを更新
	for (auto& particle : m_particles)
	{
		particle->Update(elapsedTime);
	}

	// アクティブでないパーティクルを削除
	for (int i = 0; i < m_particles.size();)
	{
		if (!m_particles.at(i)->IsActive())
		{
			m_particles.erase(m_particles.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

// 映るカメラ画面を追加
void Renderings::ParticleManager::AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	if (std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef) == m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.push_back(iCameraScreenRef);
		for (auto& particle : m_particles)
		{
			particle->AddICameraScreenRef(iCameraScreenRef);
		}
	}
}

// 映るカメラ画面を削除
void Renderings::ParticleManager::RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	auto it = std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef);
	if (it != m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.erase(it);
		for (auto& particle : m_particles)
		{
			particle->RemoveICameraScreenRef(iCameraScreenRef);
		}
	}
}
