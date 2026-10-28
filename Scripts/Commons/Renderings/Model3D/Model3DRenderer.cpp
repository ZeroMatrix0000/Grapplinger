/*
 * FileName:     Model3DRenderer.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * 3Dモデル描画
 */

#include "Pch.h"
#include "Model3DRenderer.h"

#include "Model3D.h"
#include "Model3DSource.h"
#include "../ICameraScreen.h"
#include "../Particle/Particle.h"
#include "../Shader/VertexShader.h"
#include "../Shader/PixelShader.h"
#include "Scripts/Commons/GameObjects/GameObject.h"
#include "Scripts/Commons/Components/Transform.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

 // コンストラクタ
Renderings::Model3DRenderer::Model3DRenderer()
	: IModel3DRenderer{}
	, m_instanceBuffer{}
	, m_inputLayout{}
	, m_renderObjects{}
	, m_pDevice{}
	, m_pContext{}
	, m_pCommonStates{}
	, m_pDepthStencilView{}
	, m_pVertexShader{}
	, m_pPixelShader{}
	, m_whiteTexture{}
{
}

// 初期化処理
void Renderings::Model3DRenderer::Initialize
(
	Ref<ID3D11Device5> deviceRef,
	Ref<ID3D11DeviceContext4> contextRef,
	Ref<const DirectX::CommonStates> commonStatesRef
)
{
	m_pDevice = deviceRef.Get();
	m_pContext = contextRef.Get();
	m_pCommonStates = commonStatesRef.Get();

	// バッファの詳細
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(InputElement) * 65536;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// インスタンスバッファを作成
	Utility::ThrowIfFailed(m_pDevice->CreateBuffer
	(
		&desc,
		nullptr,
		m_instanceBuffer.GetAddressOf()
	));

	CreateWhiteTexture();
}

// 描画処理
void Renderings::Model3DRenderer::Render(bool isInstance)
{
	// シェーダを設定
	m_pContext->VSSetShader(m_pVertexShader->GetD3DShaderRef().Get(), nullptr, 0);
	m_pContext->PSSetShader(m_pPixelShader->GetD3DShaderRef().Get(), nullptr, 0);

	// サンプラーを設定
	auto sampler = m_pCommonStates->LinearWrap();
	m_pContext->PSSetSamplers(0, 1, &sampler);

	// 深度ステンシルを設定
	m_pContext->OMSetDepthStencilState(m_pCommonStates->DepthDefault(), 0);

	// 定数バッファ
	Ref vSConstantBufferRef = m_pVertexShader->GetConstantBufferRef();
	// 定数バッファ
	Ref pSConstantBufferRef = m_pPixelShader->GetConstantBufferRef();

	for (const auto& [iCameraScreenRef, renderObjectsList] : m_renderObjects)
	{
		// 定数バッファを設定
		vSConstantBufferRef->SetVariable("View", iCameraScreenRef->GetViewMatrix().Transpose());
		vSConstantBufferRef->SetVariable("Projection", iCameraScreenRef->GetProjectionMatrix().Transpose());
		m_pVertexShader->SetConstantBuffer(0, Ref{ *m_pContext });
		pSConstantBufferRef->SetVariable("AmbientColor", Math::Vector3{ 0.5f, 0.5f, 0.5f });
		pSConstantBufferRef->SetVariable("LightDirection", Math::Vector3{ -0.25f, -1.0f, -0.5f });
		pSConstantBufferRef->SetVariable("LightColor", Math::Vector3{ 0.5f, 0.5f, 0.5f });
		pSConstantBufferRef->SetVariable("EyePosition", iCameraScreenRef->GetEyePosition());
		m_pPixelShader->SetConstantBuffer(0, Ref{ *m_pContext });

		// モデルソースリスト
		std::vector<Ref<const Model3DSource>> modelSources{};
		for (const auto& [modelSource, _] : renderObjectsList)
		{
			modelSources.push_back(modelSource);
		}
		// 描画順にソート
		std::ranges::sort(modelSources, [](Ref<const Model3DSource> a, Ref<const Model3DSource> b)
			{
				return a->GetDrawingOrder() < b->GetDrawingOrder();
			});

		for (const auto& modelSource : modelSources)
		{
			// 描画オブジェクトリストの参照
			Ref<const RenderObjects> renderObjectsRef{ renderObjectsList.at(modelSource) };

			// ピクセルシェーダ
			Ref pixelShader = modelSource->GetPixelShader();
			if (pixelShader->IsInitialized())
			{
				// ピクセルシェーダを設定
				m_pContext->PSSetShader(pixelShader->GetD3DShaderRef().Get(), nullptr, 0);
				// 定数バッファを設定
				pixelShader->SetConstantBuffer(2, Ref{ *m_pContext });
			}

			// 深度ステンシルステート
			ID3D11DepthStencilState* depthDtencilState = modelSource->GetDepthStencilState();
			if (depthDtencilState)
			{
				// 深度ステンシルを設定
				m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
				m_pContext->OMSetDepthStencilState(depthDtencilState, 0);
			}

			// 行列リスト
			std::vector<InputElement> inputElements{};
			inputElements.reserve(renderObjectsRef->modelRefs.size() + renderObjectsRef->particleElementRefs.size());

			for (Ref modelRef : renderObjectsRef->modelRefs)
			{
				// モデルの所有者のトランスフォーム
				Ref transform{ modelRef->GetConstOwner()->GetConstComponent<Transform>() };
				if (transform == modelRef->GetConstOwner()->GetNullComponent<Transform>().Const())
				{
					// エラーメッセージを追加
					Systems::IErrorMessage::GetInstance()->AddMessage(Utility::FormatWString
					(
						L"Transform コンポーネントが見つかりません。 | %s",
						Utility::string2wstring(modelRef->GetConstOwner()->GetName()).c_str()
					));
					return;
				}

				// 入力要素リストに追加
				Ref<InputElement> inputElementRef{ inputElements.emplace_back() };
				DirectX::XMStoreFloat3x4(&inputElementRef->instMatrix, transform->GetWorldMatrix());
				inputElementRef->color = modelRef->GetColor();

				// インスタンス描画じゃないなら
				if (!isInstance)
				{
					// バッファを更新
					UpdateInstanceBuffer(inputElements);

					// パーツ番号
					size_t partIndex = 0;
					for (const auto& mesh : modelSource->GetModel().meshes)
					{
						for (const auto& part : mesh->meshParts)
						{
							// パーツの情報を設定
							SetPartInfo(modelSource->GetPartInfo(partIndex));
							// 描画
							Draw(iCameraScreenRef, Ref{ *part }.Const());

							partIndex++;
						}
					}

					// 入力要素を削除
					inputElements.clear();
				}
			}

			for (Ref elementRef : renderObjectsRef->particleElementRefs)
			{
				// 入力要素リストに追加
				Ref<InputElement> inputElementRef{ inputElements.emplace_back() };
				DirectX::XMStoreFloat3x4(&inputElementRef->instMatrix, elementRef->matrix);
				inputElementRef->color = elementRef->color;

				// インスタンス描画じゃないなら
				if (!isInstance)
				{
					// バッファを更新
					UpdateInstanceBuffer(inputElements);

					// パーツ番号
					size_t partIndex = 0;
					for (const auto& mesh : modelSource->GetModel().meshes)
					{
						for (const auto& part : mesh->meshParts)
						{
							// パーツの情報を設定
							SetPartInfo(modelSource->GetPartInfo(partIndex));
							// 描画
							Draw(iCameraScreenRef, Ref{ *part }.Const());

							partIndex++;
						}
					}

					// 入力要素を削除
					inputElements.clear();
				}
			}

			// インスタンスバッファを更新
			UpdateInstanceBuffer(inputElements);

			// パーツ番号
			size_t partIndex = 0;
			for (const auto& mesh : modelSource->GetModel().meshes)
			{
				for (const auto& part : mesh->meshParts)
				{
					// 定数バッファを設定
					SetPartInfo(modelSource->GetPartInfo(partIndex));
					// 描画
					DrawInstanced(iCameraScreenRef, Ref{ *part }.Const(), inputElements.size());

					partIndex++;
				}
			}

			// ピクセルシェーダを元に戻す
			if (pixelShader->IsInitialized())
			{
				m_pContext->PSSetShader(m_pPixelShader->GetD3DShaderRef().Get(), nullptr, 0);
			}

			// 深度ステンシルを元に戻す
			if (depthDtencilState)
			{
				// 深度ステンシルを設定
				m_pContext->OMSetDepthStencilState(m_pCommonStates->DepthDefault(), 0);
			}
		}
	}

	m_pContext->PSSetShader(nullptr, nullptr, 0);
}

// 頂点シェーダを設定
void Renderings::Model3DRenderer::SetVertexShader(Ref<const VertexShader> vertexShader)
{
	m_pVertexShader = vertexShader.Get();
	CreateInputLayout();
}

// モデルの参照を追加
void Renderings::Model3DRenderer::AddModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef)
{
	if (iCameraScreenRef == iCameraScreenRef->GetConstOwner()->GetNullComponent<ICameraScreen>().Const())
	{
		return;
	}

	// モデルソース
	Ref<const Model3DSource> modelSource = modelRef->GetPModelSource();
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// カメラに対応した描画オブジェクトリストが見つからなければ生成
	auto cameraIt = m_renderObjects.find(iCameraScreenRef);
	if (cameraIt == m_renderObjects.end())
	{
		m_renderObjects.emplace(iCameraScreenRef, std::unordered_map<Ref<const Model3DSource>, RenderObjects>{});
		cameraIt = m_renderObjects.find(iCameraScreenRef);
	}

	// モデルソースに対応した描画オブジェクトリストが見つからなければ生成
	auto modelIt = cameraIt->second.find(modelSource);
	if (modelIt == cameraIt->second.end())
	{
		cameraIt->second.emplace(modelSource, RenderObjects{});
		modelIt = cameraIt->second.find(modelSource);
	}

	// モデルのポインタの追加
	modelIt->second.modelRefs.push_back(modelRef);
}

// モデルの参照を削除
void Renderings::Model3DRenderer::RemoveModelRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Model3D> modelRef)
{
	// モデルソース
	Ref<const Model3DSource> modelSource = modelRef->GetPModelSource();
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// ポインタリスト
	auto& modelRefs = m_renderObjects.at(iCameraScreenRef).at(modelSource).modelRefs;

	auto it = std::ranges::find(modelRefs, modelRef);
	if (it != modelRefs.end())
	{
		modelRefs.erase(it);
	}

	// 空の配列を削除
	if (modelRefs.size() == 0 && m_renderObjects.at(iCameraScreenRef).at(modelSource).particleElementRefs.size() == 0)
	{
		m_renderObjects.at(iCameraScreenRef).erase(modelSource);
		if (m_renderObjects.at(iCameraScreenRef).size() == 0)
		{
			m_renderObjects.erase(iCameraScreenRef);
		}
	}
}

// パーティクルの参照を追加
void Renderings::Model3DRenderer::AddParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef)
{
	if (iCameraScreenRef == iCameraScreenRef->GetConstOwner()->GetNullComponent<ICameraScreen>().Const())
	{
		return;
	}

	// モデルソース
	Ref<const Model3DSource> modelSource = particleElementRef->modelSource;
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// カメラに対応した描画オブジェクトリストが見つからなければ生成
	auto cameraIt = m_renderObjects.find(iCameraScreenRef);
	if (cameraIt == m_renderObjects.end())
	{
		m_renderObjects.emplace(iCameraScreenRef, std::unordered_map<Ref<const Model3DSource>, RenderObjects>{});
		cameraIt = m_renderObjects.find(iCameraScreenRef);
	}

	// モデルソースに対応した描画オブジェクトリストが見つからなければ生成
	auto modelIt = cameraIt->second.find(modelSource);
	if (modelIt == cameraIt->second.end())
	{
		cameraIt->second.emplace(modelSource, RenderObjects{});
		modelIt = cameraIt->second.find(modelSource);
	}

	// モデルのポインタの追加
	modelIt->second.particleElementRefs.push_back(particleElementRef);
}

// パーティクルの参照を削除
void Renderings::Model3DRenderer::RemoveParticleRef(Ref<const ICameraScreen> iCameraScreenRef, Ref<const Particle::RenderElement> particleElementRef)
{
	// モデルソース
	Ref<const Model3DSource> modelSource = particleElementRef->modelSource;
	if (!modelSource->IsInitialized())
	{
		return;
	}

	// ポインタリスト
	auto& particleElementRefs = m_renderObjects.at(iCameraScreenRef).at(modelSource).particleElementRefs;

	auto it = std::ranges::find(particleElementRefs, particleElementRef);
	if (it != particleElementRefs.end())
	{
		particleElementRefs.erase(it);
	}

	// 空の配列を削除
	if (particleElementRefs.size() == 0 && m_renderObjects.at(iCameraScreenRef).at(modelSource).modelRefs.size() == 0)
	{
		m_renderObjects.at(iCameraScreenRef).erase(modelSource);
		if (m_renderObjects.at(iCameraScreenRef).size() == 0)
		{
			m_renderObjects.erase(iCameraScreenRef);
		}
	}
}

// 白画像を作成
void Renderings::Model3DRenderer::CreateWhiteTexture()
{
	// 白色
	const uint32_t white = 0xFFFFFFFF;

	// テクスチャ詳細
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// サブリソースデータ
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &white;
	data.SysMemPitch = sizeof(uint32_t);

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = m_pDevice->CreateTexture2D
	(
		&desc,
		&data,
		texture.GetAddressOf()
	);
	if (FAILED(hr))
	{
		return;
	}

	// シェーダリソースビュー詳細
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビュー
	hr = m_pDevice->CreateShaderResourceView(
		texture.Get(),
		&srvDesc,
		m_whiteTexture.GetAddressOf()
	);
	if (FAILED(hr))
	{
		return;
	}
}

// 入力レイアウトを作成
void Renderings::Model3DRenderer::CreateInputLayout()
{
	const D3D11_INPUT_ELEMENT_DESC inputElements[] =
	{
		{
			"SV_Position",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			24,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"InstMatrix",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"InstMatrix",
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			16,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"InstMatrix",
			2,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			32,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"Color",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			48,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		}
	};

	Utility::ThrowIfFailed(m_pDevice->CreateInputLayout
	(
		inputElements,
		_countof(inputElements),
		m_pVertexShader->GetBlobRef()->GetBufferPointer(),
		m_pVertexShader->GetBlobRef()->GetBufferSize(),
		m_inputLayout.GetAddressOf()
	));
}

// インスタンスバッファを更新
void Renderings::Model3DRenderer::UpdateInstanceBuffer(const std::vector<InputElement>& inputElements)
{
	// サブリソース
	D3D11_MAPPED_SUBRESOURCE mapped{};
	// GPU へ転送
	Utility::ThrowIfFailed(m_pContext->Map
	(
		m_instanceBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped
	));
	std::memcpy(mapped.pData, inputElements.data(), sizeof(InputElement) * inputElements.size());
	m_pContext->Unmap(m_instanceBuffer.Get(), 0);
}

// パーツの情報を設定
void Renderings::Model3DRenderer::SetPartInfo(const Model3DSource::PartInfo& partInfo)
{
	// テクスチャを設定
	m_pContext->PSSetShaderResources(0, 1, partInfo.texture.Get() ? partInfo.texture.GetAddressOf() : m_whiteTexture.GetAddressOf());

	// 定数バッファ
	Ref constantBufferRef = m_pPixelShader->GetConstantBufferRef();
	// 定数バッファを設定
	constantBufferRef->SetVariable("DiffuseColor", partInfo.effectInfo.diffuseColor);
	constantBufferRef->SetVariable("EmissiveColor", partInfo.effectInfo.emissiveColor);
	constantBufferRef->SetVariable("SpecularColor", partInfo.effectInfo.specularColor);
	constantBufferRef->SetVariable("SpecularPower", partInfo.effectInfo.specularPower);
	m_pPixelShader->SetConstantBuffer(1, Ref{ *m_pContext });
}

// 描画
void Renderings::Model3DRenderer::Draw(Ref<const ICameraScreen> iCameraScreenRef, Ref<const DirectX::ModelMeshPart> partRef)
{
	// 入力レイアウト
	m_pContext->IASetInputLayout(m_inputLayout.Get());

	// 頂点バッファ
	ID3D11Buffer* buffers[] =
	{
		partRef->vertexBuffer.Get(),
		m_instanceBuffer.Get()
	};
	UINT strides[] =
	{
		partRef->vertexStride,
		sizeof(InputElement)
	};
	UINT offsets[] =
	{
		0,
		0
	};
	m_pContext->IASetVertexBuffers(
		0,
		2,
		buffers,
		strides,
		offsets
	);

	// 番号バッファ
	m_pContext->IASetIndexBuffer
	(
		partRef->indexBuffer.Get(),
		partRef->indexFormat,
		0
	);

	// トポロジー
	m_pContext->IASetPrimitiveTopology(partRef->primitiveType);

	// 描画
	m_pContext->DrawIndexed
	(
		partRef->indexCount,
		partRef->startIndex,
		partRef->vertexOffset
	);
}

// インスタンス描画
void Renderings::Model3DRenderer::DrawInstanced(Ref<const ICameraScreen> iCameraScreenRef, Ref<const DirectX::ModelMeshPart> partRef, size_t instMatricesCount)
{
	// 頂点バッファ
	ID3D11Buffer* buffers[] =
	{
		partRef->vertexBuffer.Get(),
		m_instanceBuffer.Get()
	};
	UINT strides[] =
	{
		partRef->vertexStride,
		sizeof(InputElement)
	};
	UINT offsets[] =
	{
		0,
		0
	};
	m_pContext->IASetVertexBuffers(
		0,
		2,
		buffers,
		strides,
		offsets
	);

	// 番号バッファ
	m_pContext->IASetIndexBuffer
	(
		partRef->indexBuffer.Get(),
		partRef->indexFormat,
		0
	);

	// トポロジー
	m_pContext->IASetPrimitiveTopology(partRef->primitiveType);

	// 入力レイアウト
	m_pContext->IASetInputLayout(m_inputLayout.Get());

	// 描画
	m_pContext->DrawIndexedInstanced
	(
		partRef->indexCount,
		static_cast<UINT>(instMatricesCount),
		partRef->startIndex,
		partRef->vertexOffset,
		0
	);
}
