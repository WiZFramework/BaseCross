/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	立方体グループ実体
	//--------------------------------------------------------------------------------------
	CubeObjectGroup::CubeObjectGroup(const wstring& TextureFileName) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_MaxInstance(2000)
	{}
	CubeObjectGroup::~CubeObjectGroup() {}

	void CubeObjectGroup::CreateBuffers() {
		//Cubeの作成
		float HelfSize = 0.5f;
		vector<Vector3> PosVec = {
			{ Vector3(-HelfSize, HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, -HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, -HelfSize, -HelfSize) },
			{ Vector3(HelfSize, HelfSize, HelfSize) },
			{ Vector3(-HelfSize, HelfSize, HelfSize) },
			{ Vector3(HelfSize, -HelfSize, HelfSize) },
			{ Vector3(-HelfSize, -HelfSize, HelfSize) },
		};
		vector<UINT> PosIndeces = {
			0, 1, 2, 3,
			1, 4, 3, 6,
			4, 5, 6, 7,
			5, 0, 7, 2,
			5, 4, 0, 1,
			2, 3, 7, 6,
		};


		vector<Vector3> FaceNormalVec = {
			{ Vector3(0, 0, -1.0f) },
			{ Vector3(1.0f, 0, 0) },
			{ Vector3(0, 0, 1.0f) },
			{ Vector3(-1.0f, 0, 0) },
			{ Vector3(0, 1.0f, 0) },
			{ Vector3(0, -1.0f, 0) }
		};

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormalTexture Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				Data.normal = FaceNormalVec[i];
				switch (j) {
				case 0:
					Data.textureCoordinate = Vector2(0, 0);
					break;
				case 1:
					Data.textureCoordinate = Vector2(1.0f, 0);
					break;
				case 2:
					Data.textureCoordinate = Vector2(0, 1.0f);
					break;
				case 3:
					Data.textureCoordinate = Vector2(1.0f, 1.0f);
					break;
				}
				vertices.push_back(Data);
			}

			indices.push_back((uint16_t)BasePosCount + 0);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 2);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 3);
			indices.push_back((uint16_t)BasePosCount + 2);

			BasePosCount += 4;
		}
		//メッシュの作成（変更できない）
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//インスタンス行列バッファの作成
		//Max値で作成する
		vector<Matrix4X4> matrices(m_MaxInstance);
		for (auto& m : matrices) {
			m = Matrix4X4();
		}
		MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
	}


	void CubeObjectGroup::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
		//インスタンス配列の作成
		for (UINT count = 0; count < 500; count++) {
			CubeObject Data;
			Data.Refresh();
			m_CubeObjectVec.push_back(Data);
		}
	}
	void CubeObjectGroup::OnUpdate() {
		if (m_CubeObjectVec.size() >= m_MaxInstance) {
			throw BaseException(
				L"インスタンス上限を超えてます",
				L"if(m_CubeObjectVec.size() >= m_MaxInstance)",
				L"CubeObjectGroup::OnUpdate()"
			);

		}

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& v : m_CubeObjectVec) {
			v.m_Posision += v.m_Velocity * ElapsedTime;
			Quaternion QtSpan(v.m_QuaternionRot, v.m_QuaternionVelocity * ElapsedTime);
			v.m_Quaternion *= QtSpan;
			v.m_Quaternion.Normalize();
			if (v.m_Posision.Length() >= 2.0f) {
				v.Refresh();
			}
		}

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pID3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CubeObjectGroup::OnUpdate()"
			);
		}
		//行列の変更
		auto* matrices = (Matrix4X4*)mappedBuffer.pData;
		Matrix4X4 World;
		for (size_t i = 0; i < m_CubeObjectVec.size(); i++) {
			//ワールド行列の決定
			World.AffineTransformation(
				m_CubeObjectVec[i].m_Scale,			//スケーリング
				Vector3(0, 0, 0),		//回転の中心（重心）
				m_CubeObjectVec[i].m_Quaternion,		//回転角度
				m_CubeObjectVec[i].m_Posision		//位置
			);
			//転置する
			World.Transpose();
			matrices[i] = World;
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);
	}
	void CubeObjectGroup::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();


		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(Matrix4X4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { m_CubeMesh->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_CubeMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTInstance::GetPtr()->GetInputLayout());

		//ブレンドステート
		//透明処理しない
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//ラスタライザステート（表面描画）
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//ビュー行列の決定
		Matrix4X4 View, Proj;
		View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.Transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.World = Matrix4X4();	//ワールド行列はダミー
		sb.View = View;
		sb.Projection = Proj;
		//ライティング
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//エミッシブ加算。
		sb.Emissive = Color4(0.4f, 0.4f, 0.4f, 0);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//描画
		pD3D11DeviceContext->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), m_CubeObjectVec.size(), 0, 0, 0);
		//後始末
		Dev->InitializeStates();
	}

}
//end basecross
