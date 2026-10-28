/*
 * FileName:     ImageSource.h
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 画像ソース
 */

#pragma once

namespace Renderings
{
	// 画像ソース
	class ImageSource
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		ImageSource();

		// 初期化処理
		void Initialize(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef);

		// シェーダーリソースビューの参照を取得
		Ref<ID3D11ShaderResourceView> GetTextureRef() const { return Ref{ *m_texture.Get() }; }
		// リソースの参照を取得
		Ref<ID3D11Texture2D> GetResourceRef() const { return Ref{ *m_resource.Get() }; }

		// 初期化済みかどうかを取得
		bool IsInitialized() const { return m_texture.Get() && m_resource.Get(); }


		/* 静的関数 */

		// 生成
		static ImageSource Create(const std::wstring& filePath, Ref<ID3D11Device5> deviceRef);


	private:


		/* メンバ変数 */

		// シェーダーリソースビュー
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		// リソース
		Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_resource;

	};
}
