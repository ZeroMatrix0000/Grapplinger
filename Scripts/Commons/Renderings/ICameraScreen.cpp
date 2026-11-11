/*
 * FileName:     ICameraScreen.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/11
 *
 * カメラ画面のインタフェース
 */

#include "Pch.h"
#include "ICameraScreen.h"

#include "Model3D/Model3D.h"
#include "Model3D/Model3DSource.h"
#include "Model3D/IModel3DRenderer.h"

// コンストラクタ
Renderings::ICameraScreen::ICameraScreen(const ComponentDesc& desc, Ref<IModel3DRenderer> iModelRenderer)
	: Component{ desc }
	, m_iModelRenderer{ iModelRenderer }
	, m_renderObjectsList{}
{
	m_iModelRenderer->AddICameraScreen(Ref{ *this }.Const());
}

// デストラクタ
Renderings::ICameraScreen::~ICameraScreen()
{
	m_iModelRenderer->RemoveICameraScreen(Ref{ *this }.Const());
}

// モデルの参照を追加
void Renderings::ICameraScreen::AddModelRef(Ref<const Model3D> modelRef) const
{
	// モデルソース
	Ref modelSource = modelRef->GetModelSource();
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// モデルソースに対応した描画オブジェクトリストが見つからなければ生成
	auto it = m_renderObjectsList.find(modelSource);
	if (it == m_renderObjectsList.end())
	{
		it = m_renderObjectsList.emplace(modelSource, RenderObjects{}).first;
	}

	// モデルのポインタの追加
	it->second.modelRefs.push_back(modelRef);
}

// モデルの参照を削除
void Renderings::ICameraScreen::RemoveModelRef(Ref<const Model3D> modelRef) const
{
	// モデルソース
	Ref modelSource = modelRef->GetModelSource();
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// ポインタリスト
	auto& modelRefs = m_renderObjectsList.at(modelSource).modelRefs;

	auto it = std::ranges::find(modelRefs, modelRef);
	if (it != modelRefs.end())
	{
		modelRefs.erase(it);
	}

	// 空の配列を削除
	if (modelRefs.size() == 0 && m_renderObjectsList.at(modelSource).particleElementRefs.size() == 0)
	{
		m_renderObjectsList.erase(modelSource);
	}
}

// パーティクルの参照を追加
void Renderings::ICameraScreen::AddParticleRef(Ref<const Particle::RenderElement> particleElementRef) const
{
	// モデルソース
	Ref modelSource = particleElementRef->modelSource;
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// モデルソースに対応した描画オブジェクトリストが見つからなければ生成
	auto it = m_renderObjectsList.find(modelSource);
	if (it == m_renderObjectsList.end())
	{
		it = m_renderObjectsList.emplace(modelSource, RenderObjects{}).first;
	}

	// モデルのポインタの追加
	it->second.particleElementRefs.push_back(particleElementRef);
}

// パーティクルの参照を削除
void Renderings::ICameraScreen::RemoveParticleRef(Ref<const Particle::RenderElement> particleElementRef) const
{
	// モデルソース
	Ref modelSource = particleElementRef->modelSource;
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// ポインタリスト
	auto& particleElementRefs = m_renderObjectsList.at(modelSource).particleElementRefs;

	auto it = std::ranges::find(particleElementRefs, particleElementRef);
	if (it != particleElementRefs.end())
	{
		particleElementRefs.erase(it);
	}

	// 空の配列を削除
	if (particleElementRefs.size() == 0 && m_renderObjectsList.at(modelSource).modelRefs.size() == 0)
	{
		m_renderObjectsList.erase(modelSource);
	}
}
