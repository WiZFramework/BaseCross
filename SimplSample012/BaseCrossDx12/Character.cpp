/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	球実体
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos) :
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos)
	{}
	SphereObject::~SphereObject() {}

	void SphereObject::CreateBuffers() {
		float Radius = 0.5f;
		//緯度方向
		UINT Vertical = m_Division;
		//経度方向
		UINT Horizontal = m_Division * 2;

		vector<VertexPositionNormalTexture> vertices;
		for (UINT i = 0; i <= Vertical; i++)
		{
			float v = 1 - (float)i / Vertical;

			float Latitude = (i * XM_PI / Vertical) - XM_PIDIV2;
			float Dy = sin(Latitude);
			float Dxz = cos(Latitude);

			for (UINT j = 0; j <= Horizontal; j++)
			{
				float u = 1 - (float)j / Horizontal;

				float Longitude = j * XM_2PI / Horizontal;
				float Dx = sin(Longitude) * Dxz;
				float Dz = cos(Longitude) * Dxz;
				Vec3 normal(Dx, Dy, Dz);
				VertexPositionNormalTexture Data;
				Data.position = normal * Radius;
				Data.normal = normal;
				Data.normal.normalize();
				Data.textureCoordinate = Vec2(u, v);
				vertices.push_back(Data);
			}
		}
		UINT Stride = Horizontal + 1;
		vector<uint16_t> indices;
		for (UINT i = 0; i < Vertical; i++)
		{
			for (UINT j = 0; j <= Horizontal; j++)
			{
				UINT NextI = i + 1;
				UINT NextJ = (j + 1) % Stride;
				indices.push_back((uint16_t)(i * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + j));

				indices.push_back((uint16_t)(NextI * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + NextJ));
			}
		}
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void SphereObject::UpdateConstantBuffer() {
		//行列の定義
		Mat4x4 World, View, Proj;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		World.transpose();
		//ビュー行列の決定
		View = XMMatrixLookAtLH(Vec3(0, 2.0, -5.0f), Vec3(0, 0, 0), Vec3(0, 1.0f, 0));
		//転置する
		View.transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.transpose();
		//ライティング
		Vec4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
			sizeof(m_StaticConstantBuffer));
	}

	///ルートシグネチャ作成
	void SphereObject::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///デスクプリタヒープ作成
	void SphereObject::CreateDescriptorHeap() {
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
	void SphereObject::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void SphereObject::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コンスタントバッファは256バイトにアラインメント
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;
		//コンスタントバッファリソース（アップロードヒープ）の作成
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"SphereObject::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
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
			L"SphereObject::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void SphereObject::CreateShaderResourceView() {
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
	void SphereObject::CreatePipelineState() {
		//パイプラインステートの作成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, PineLineDesc);
		//ブレンドステートとラスタライザ差し替え
		if (m_Trace) {
			D3D12_BLEND_DESC blend_desc;
			D3D12_RENDER_TARGET_BLEND_DESC Target;
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_ONE;
			Target.DestBlendAlpha = D3D12_BLEND_ZERO;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			PineLineDesc.BlendState = blend_desc;
		}

		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;

		m_CullFrontPipelineState = PipelineState::CreateDirect(PineLineDesc);

		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///コマンドリスト作成
	void SphereObject::CreateCommandList() {
		//コマンドリストは裏面カリングに初期化
		m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
		CommandList::Close(m_CommandList);
	}

	///描画処理
	void SphereObject::DrawObject() {
		//コマンドリストのリセット
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		m_SphereMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
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
		m_CommandList->IASetIndexBuffer(&m_SphereMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &m_SphereMesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(m_SphereMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(m_SphereMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}



	void SphereObject::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
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
	void SphereObject::OnUpdate() {
		Quat QtSpan;
		QtSpan.rotation(0.02f, Vec3(0, 1.0f, 0));
		m_Qt *= QtSpan;
		m_Qt.normalize();
	}
	void SphereObject::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		DrawObject();
	}



}
//end basecross
