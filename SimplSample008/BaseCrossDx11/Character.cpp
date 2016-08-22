/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	壁スプライト実体
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos)
	{}
	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横10個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,0,0,1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(0, 1.0f, 0, 1.0f), Vector2(10, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(0, 0, 1.0f, 1.0f), Vector2(0.0f, 10)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 1.0f, 0, 1.0f), Vector2(10, 10)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
		//矩形の大きさ
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_Scale = Vector2(w, h);
	}


	void WallSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			m_Scale,			//スケーリング
			Vector2(0, 0),		//回転の中心（重心）
			0,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;

		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_Trace) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}



	//--------------------------------------------------------------------------------------
	///	四角形スプライト実体
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos),
		m_TotalTime(0)
	{}
	SquareSprite::~SquareSprite() {}
	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(4.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(4.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
		//矩形の大きさ
		m_Scale = Vector2(512.0f, 128.0f);
	}

	//頂点の変更
	void SquareSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SquareObject::OnUpdate()"
			);
		}
		//頂点の変更
		VertexPositionTexture* vertices
			= (VertexPositionTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);

		}
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);
	}


	void SquareSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateVertex(ElapsedTime);
	}

	void SquareSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			m_Scale,			//スケーリング
			Vector2(0, 0),		//回転の中心（重心）
			0,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;

		//コンスタントバッファの準備
		DiffuseSpriteConstantBuffer sb;
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		//デフィーズはすべて通す
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBDiffuseSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBDiffuseSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_Trace) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();

	}



}
//end basecross
