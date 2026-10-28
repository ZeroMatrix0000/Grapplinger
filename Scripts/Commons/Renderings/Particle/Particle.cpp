/*
 * FileName:     Particle.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * パーティクル
 */

#include "Pch.h"
#include "Particle.h"

#include "../Model3D/IModel3DRenderer.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/GameObjects/IGameObjectFinder.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IResources.h"

// コンストラクタ
Renderings::Particle::Particle(Ref<IModel3DRenderer> iModelRenderer, Ref<Systems::IResources> iResources)
	: m_updateElements{}
	, m_renderElements{}
	, m_isActive{ true }
	, m_iCameraScreenRefs{}
	, m_iModelRendererRef{ iModelRenderer }
	, m_iResources{ iResources }
{
}

// デストラクタ
Renderings::Particle::~Particle()
{
	// 描画者からパーティクルを削除
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		for (const auto& renderElement : m_renderElements)
		{
			m_iModelRendererRef->RemoveParticleRef(iCameraScreenRef, Ref{ *renderElement }.Const());
		}
	}
}

// ワールド行列を適用
void Renderings::Particle::ApplyTransform()
{
	for (int i = 0; i < m_updateElements.size(); i++)
	{
		// 更新情報
		UpdateElement& updateElement = m_updateElements.at(i);

		// 描画情報を更新
		m_renderElements.at(i)->matrix =
			Math::Matrix::CreateScale(updateElement.scale) *
			Math::Matrix::CreateFromQuaternion(updateElement.rotation) *
			Math::Matrix::CreateTranslation(updateElement.position)
			;
	}
}

// モデルソースを設定
void Renderings::Particle::SetModelSource(const std::string& modelSourceName, Ref<RenderElement> renderElementRef)
{
	// 描画者からパーティクルを削除
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		m_iModelRendererRef->RemoveParticleRef(iCameraScreenRef, renderElementRef.Const());
	}
	renderElementRef->modelSource = m_iResources->GetModelSource(modelSourceName);
	// 描画者にパーティクルを追加
	for (Ref iCameraScreenRef : m_iCameraScreenRefs)
	{
		m_iModelRendererRef->AddParticleRef(iCameraScreenRef, renderElementRef.Const());
	}
}

// 映るカメラ画面を追加
void Renderings::Particle::AddICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	if (std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef) == m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.push_back(iCameraScreenRef);
		for (const auto& renderElement : m_renderElements)
		{
			m_iModelRendererRef->AddParticleRef(iCameraScreenRef, Ref{ *renderElement }.Const());
		}
	}
}

// 映るカメラ画面を削除
void Renderings::Particle::RemoveICameraScreenRef(Ref<const ICameraScreen> iCameraScreenRef)
{
	auto it = std::ranges::find(m_iCameraScreenRefs, iCameraScreenRef);
	if (it != m_iCameraScreenRefs.end())
	{
		m_iCameraScreenRefs.erase(it);
		for (const auto& renderElement : m_renderElements)
		{
			m_iModelRendererRef->RemoveParticleRef(iCameraScreenRef, Ref{ *renderElement }.Const());
		}
	}
}

// 要素を追加
void Renderings::Particle::AddElement(UpdateElement** ppUpdateElement, RenderElement** ppRenderElement)
{
	if (ppUpdateElement)
	{
		*ppUpdateElement = &m_updateElements.emplace_back();
	}

	auto& renderElement = m_renderElements.emplace_back();
	renderElement = std::make_unique<RenderElement>(m_iResources);
	if (ppRenderElement)
	{
		*ppRenderElement = renderElement.get();
	}
}

// コンストラクタ
Renderings::Particle::RenderElement::RenderElement(Ref<Systems::IResources> iResources)
	: matrix{ Math::Matrix::Identity }
	, color{ DirectX::Colors::White }
	, modelSource{ iResources->GetNullModelSource() }
{
}
