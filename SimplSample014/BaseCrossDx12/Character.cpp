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
		//インスタンス描画用の行列のメッシュ（内容変更できる）
		m_InstanceMatrixMesh = MeshResource::CreateMeshResource(matrices, true);
	}

	///ルートシグネチャ作成
	void CubeObjectGroup::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///デスクプリタヒープ作成
	void CubeObjectGroup::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrvデスクプリタヒープ
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//サンプラーデスクプリタヒープ
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU側デスクプリタヒープのハンドルの配列の作成
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///サンプラー作成
	void CubeObjectGroup::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void CubeObjectGroup::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コンスタントバッファリソース（アップロードヒープ）の作成
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(PNTStaticConstantBuffer)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"CubeObjectGroup::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//コンスタントバッファは256バイトにアラインメント
		cbvDesc.SizeInBytes = (sizeof(PNTStaticConstantBuffer) + 255) & ~255;
		//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
		//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//コンスタントバッファのアップロードヒープのマップ
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"コンスタントバッファのマップに失敗しました",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"CubeObjectGroup::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void CubeObjectGroup::CreateShaderResourceView() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//テクスチャハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//テクスチャのシェーダリソースビューを作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//フォーマット
		srvDesc.Format = m_TextureResource->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureResource->GetTextureResDesc().MipLevels;
		//シェーダリソースビュー
		Dev->GetDevice()->CreateShaderResourceView(
			m_TextureResource->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///パイプラインステート作成
	void CubeObjectGroup::CreatePipelineState() {
		//パイプラインステートの作成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTextureMatrix, VSPNTInstance, PSPNTStatic>(m_RootSignature, PineLineDesc);
		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///コマンドリスト作成
	void CubeObjectGroup::CreateCommandList() {
		//コマンドリストは裏面カリングに初期化
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void CubeObjectGroup::UpdateConstantBuffer() {
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
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);

		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&sb),
			sizeof(sb));
	}



	///描画処理
	void CubeObjectGroup::DrawObject() {
		CommandList::Reset(m_PipelineState, m_CommandList);

		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		m_InstanceMatrixMesh->UpdateResources<Matrix4X4>(m_CommandList);
		m_TextureResource->UpdateResources(m_CommandList);

		//描画
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());
		//インスタンス描画は、頂点バッファを複数登録する
		const D3D12_VERTEX_BUFFER_VIEW Buf[2] = { 
			m_CubeMesh->GetVertexBufferView(),
			m_InstanceMatrixMesh->GetVertexBufferView() 
		};
		m_CommandList->IASetVertexBuffers(0, 2, Buf);

		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), m_CubeObjectVec.size(), 0, 0, 0);

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	void CubeObjectGroup::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		//インスタンス配列の作成
		for (UINT count = 0; count < 500; count++) {
			CubeObject Data;
			Data.Refresh();
			m_CubeObjectVec.push_back(Data);
		}
		///各初期化関数呼び出し
		///ルートシグネチャ作成
		CreateRootSignature();
		///デスクプリタヒープ作成
		CreateDescriptorHeap();
		///サンプラー作成
		CreateSampler();
		///シェーダーリソースビュー作成
		CreateShaderResourceView();
		///コンスタントバッファ作成
		CreateConstantBuffer();
		///パイプラインステート作成
		CreatePipelineState();
		///コマンドリスト作成
		CreateCommandList();
		//コンスタントバッファの更新
		UpdateConstantBuffer();

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
		vector<Matrix4X4> MatVec;
		for (size_t i = 0; i < m_CubeObjectVec.size(); i++) {
			Matrix4X4 World;
			//ワールド行列の決定
			World.AffineTransformation(
				m_CubeObjectVec[i].m_Scale,			//スケーリング
				Vector3(0, 0, 0),		//回転の中心（重心）
				m_CubeObjectVec[i].m_Quaternion,		//回転角度
				m_CubeObjectVec[i].m_Posision		//位置
			);
			//転置する
			World.Transpose();
			MatVec.push_back(World);
		}
		//メッシュの頂点の変更
		m_InstanceMatrixMesh->UpdateVirtex(MatVec);
	}
	void CubeObjectGroup::OnDraw() {
		UpdateConstantBuffer();
		DrawObject();
	}

}
//end basecross
