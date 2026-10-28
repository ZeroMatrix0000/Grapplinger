/*
 * FileName:     RenderingResources.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 描画リソース
 */

#pragma once

#include "../Systems/OnlyOne.h"
#include "Model3D/InstancingEffectFactory.h"

namespace Renderings
{
	// 描画リソース
	class RenderingResources : public Systems::OnlyOne
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		RenderingResources();

		// 初期化処理
		void Initialize(Ref<ID3D11Device5> deviceRef);

		// エフェクトファクトリーのディレクトリを設定
		void SetEffectFactoryDirectory(const std::wstring& filePath);

		// コモンステートの参照の取得
		Ref<const DirectX::CommonStates> GetCommonStatesRef() const { return Ref{ *m_commonStates.get() }.Const(); }
		// エフェクトファクトリーの参照の取得
		Ref<InstancingEffectFactory> GetEffectFactoryRef() const { return Ref{ *m_effectFactory.get() }; }


	private:


		/* メンバ変数 */

		// コモンステート
		std::unique_ptr<DirectX::CommonStates>  m_commonStates;
		// エフェクトファクトリー
		std::unique_ptr<InstancingEffectFactory> m_effectFactory;

	};
}
