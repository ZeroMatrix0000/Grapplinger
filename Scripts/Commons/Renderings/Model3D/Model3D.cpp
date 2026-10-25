/*
 * FileName:     Model3D.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/25
 *
 * 3Dモデル
 */

#include "Pch.h"
#include "Model3D.h"

#include "IModel3DRenderer.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Renderings/ICameraScreen.h"
#include "Scripts/Commons/Systems/IResources.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"

Renderings::Model3D::Model3D(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRendererRef, Ref<Systems::IResources> iResourcesRef)
	: Component{ desc }
	, m_color{ DirectX::Colors::White }
	, m_modelSourceRef{ iResourcesRef->GetNullModelSource() }
	, m_iCameraScreenRefs{}
	, m_iModelRendererRef{ iModelRendererRef }
	, m_iResourcesRef{ iResourcesRef }
{
}

// デストラクタ
Renderings::Model3D::~Model3D()
{
	// 描画者からモデルを削除
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		m_iModelRendererRef->RemoveModelRef(iCameraScreenRef, Ref{ *this }.Const());
	}
}

// 初期化処理
void Renderings::Model3D::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinderRef)
{
	std::string modelSourceName{};

	Systems::JsonSerializer serializer{ iGameObjectFinderRef };
	serializer.AddParameter(&modelSourceName, "ModelSourceName");
	serializer.AddParameter(&m_color, "Color");
	serializer.AddParameter(&m_iCameraScreenRefs, "CameraScreens");
	serializer.Load(json);

	if (!modelSourceName.empty())
	{
		SetModelSource(modelSourceName);
	}
}

// モデルソースを設定
void Renderings::Model3D::SetModelSource(const std::string& modelSourceName)
{
	// 描画者からモデルを削除
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		m_iModelRendererRef->RemoveModelRef(iCameraScreenRef, Ref{ *this }.Const());
	}
	m_modelSourceRef = m_iResourcesRef->GetModelSource(modelSourceName);
	// 描画者にモデルを追加
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		m_iModelRendererRef->AddModelRef(iCameraScreenRef, Ref{ *this }.Const());
	}
}

// 映るカメラ画面を追加
void Renderings::Model3D::AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	if (std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef) == m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.push_back(iCameraScreenRef);
		m_iModelRendererRef->AddModelRef(iCameraScreenRef, Ref{ *this }.Const());
	}
}

// 映るカメラ画面を削除
void Renderings::Model3D::RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	auto it = std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef);
	if (it != m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.erase(it);
		m_iModelRendererRef->RemoveModelRef(iCameraScreenRef, Ref{ *this }.Const());
	}
}
