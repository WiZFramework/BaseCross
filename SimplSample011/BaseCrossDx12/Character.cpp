/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	立方体実体
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const wstring& TextureFileName, bool Trace, const Vector3& Pos, bool Flat) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Flat(Flat)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
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
				if (m_Flat) {
					//フラット表示の場合は法線は頂点方向にする
					Data.normal = Data.position;
					Data.normal.Normalize();
				}
				else {
					//フラット表示しない場合は、法線は面の向き
					Data.normal = FaceNormalVec[i];
				}
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
	}

	void CubeObject::UpdateConstantBuffer() {
		//行列の定義
		Matrix4X4 World, View, Proj;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		World.Transpose();
		//ビュー行列の決定
		View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		Proj.Transpose();
		//ライティング
		Vector4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.Normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Color4(0, 0, 0, 0);
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
			sizeof(m_StaticConstantBuffer));
	}


	///ルートシグネチャ作成
	void CubeObject::CreateRootSignature() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//もしすでに登録されていたらそれを使用
		m_RootSignature = Dev->GetRootSignature(L"SrvSmpCbv");
		if (m_RootSignature != nullptr) {
			return;
		}
		//デスクプリタレンジは3個
		CD3DX12_DESCRIPTOR_RANGE ranges[3];
		//0番目はシェーダリソースビュー
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//1番目はサンプラー
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
		//2番目はコンスタントバッファ
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		//ルートパラメータも3個
		CD3DX12_ROOT_PARAMETER rootParameters[3];
		//0番目はシェーダリソースビューのレンジをピクセルシェーダへ
		rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
		//1番目はサンプラーのレンジをピクセルシェーダへ
		rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
		//2番目はコンスタントバッファのレンジをすべてにシェーダに
		rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);
		//ルートシグネチャの定義
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		//ルートシグネチャ定義の初期化（ルートパラメータを設定）
		rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		//各ブロブ（バイナリイメージ）
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		//ルートシグネチャ定義のシリアライズ
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
			L"ルートシグネチャのシリアライズに失敗しました",
			L"D3D12SerializeRootSignature()",
			L"CubeObject::CreateRootSignature()"
		);
		//ルートシグネチャの作成
		ThrowIfFailed(
			Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(),
				signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)),
			L"ルートシグネチャの作成に失敗しました",
			L"Dev->GetDevice()->CreateRootSignature()",
			L"CubeObject::CreateRootSignature()"
		);
		//ルートシグネチャの登録（Dx12には関係ない）
		Dev->SetRootSignature(L"SrvSmpCbv", m_RootSignature);
	}
	///デスクプリタヒープ作成
	void CubeObject::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//コンスタントバッファとシェーダーリソース用デスクプリタヒープ（合計2個）
		D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
		CbvSrvHeapDesc.NumDescriptors = 2;
		CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&CbvSrvHeapDesc, IID_PPV_ARGS(&m_CbvSrvUavDescriptorHeap)),
			L"CbvSrvUav用のデスクプリタヒープの作成に失敗しました",
			L"Dev->GetDevice()->CreateDescriptorHeap()",
			L"CubeObject::CreateDescriptorHeap()"
		);
		//サンプラーデスクプリタヒープ（1個）
		D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
		SamplerHeapDesc.NumDescriptors = 1;
		SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&SamplerHeapDesc, IID_PPV_ARGS(&m_SamplerDescriptorHeap)),
			L"サンプラー用のデスクプリタヒープの作成に失敗しました",
			L"Dev->GetDevice()->CreateDescriptorHeap()",
			L"CubeObject::CreateDescriptorHeap()"
		);
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
	void CubeObject::CreateSampler() {
		//サンプラーデスクプリタヒープ内のCPU側ハンドルを得る
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		//サンプラー定義
		D3D12_SAMPLER_DESC samplerDesc = {};
		//リニアクランプに設定
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		auto Dev = App::GetApp()->GetDeviceResources();
		//ハンドルとサンプラー定義を結びつける
		Dev->GetDevice()->CreateSampler(&samplerDesc, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void CubeObject::CreateConstantBuffer() {
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
			L"CubeObject::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
		//コンスタントバッファはシェーダリソースビューのあとに設置する
		//シェーダーリソースは1個で、先頭からインクリメントサイズだけ
		//離れた場所がコンスタントバッファのハンドルの場所である
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvHandle);
		//コンスタントバッファのアップロードヒープのマップ
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"コンスタントバッファのマップに失敗しました",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"CubeObject::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void CubeObject::CreateShaderResourceView() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//テクスチャハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//テクスチャのシェーダリソースビューを作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//フォーマット(テクスチャリソースから取得)
		srvDesc.Format = m_TextureResource->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureResource->GetTextureResDesc().MipLevels;
		//シェーダリソースビューの作成
		Dev->GetDevice()->CreateShaderResourceView(
			m_TextureResource->GetTexture().Get(),
			&srvDesc,
			srvHandle);
	}
	///パイプラインステート作成
	void CubeObject::CreatePipelineState() {
		//ラスタライザステートの作成（デフォルト）
		CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
		//裏面カリングに変更
		rasterizerStateDesc.CullMode = D3D12_CULL_MODE_BACK;
		//パイプラインステートの作成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		ZeroMemory(&PineLineDesc, sizeof(PineLineDesc));
		PineLineDesc.InputLayout = { VertexPositionNormalTexture::GetVertexElement(), VertexPositionNormalTexture::GetNumElements() };
		PineLineDesc.pRootSignature = m_RootSignature.Get();
		PineLineDesc.VS =
		{
			reinterpret_cast<UINT8*>(VSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
			VSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferSize()
		};
		PineLineDesc.PS =
		{
			reinterpret_cast<UINT8*>(PSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
			PSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferSize()
		};
		PineLineDesc.RasterizerState = rasterizerStateDesc;
		PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		PineLineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		PineLineDesc.SampleMask = UINT_MAX;
		PineLineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		PineLineDesc.NumRenderTargets = 1;
		PineLineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		PineLineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		PineLineDesc.SampleDesc.Count = 1;
		//もし透明ならブレンドステート差し替え
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
		auto Dev = App::GetApp()->GetDeviceResources();
		//まずCullBackのパイプラインステートを作成
		ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&PineLineDesc, IID_PPV_ARGS(&m_CullBackPipelineState)),
			L"CullBackパイプラインステートの作成に失敗しました",
			L"Dev->GetDevice()->CreateGraphicsPipelineState()",
			L"CubeObject::CreatePipelineState()"
		);
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		//続いてCullFrontのパイプラインステートを作成
		ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&PineLineDesc, IID_PPV_ARGS(&m_CullFrontPipelineState)),
			L"CullFrontパイプラインステートの作成に失敗しました",
			L"Dev->GetDevice()->CreateGraphicsPipelineState()",
			L"CubeObject::CreatePipelineState()"
		);
	}
	///コマンドリスト作成
	void CubeObject::CreateCommandList() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コマンドリストは裏面カリングに初期化
		ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			Dev->GetCommandAllocator().Get(),
			m_CullBackPipelineState.Get(),
			IID_PPV_ARGS(&m_CommandList)),
			L"コマンドリストの作成に失敗しました",
			L"Dev->GetDevice()->CreateCommandList()",
			L"CubeObject::CreateCommandList()"
		);
		ThrowIfFailed(m_CommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"commandList->Close()",
			L"CubeObject::CreateCommandList()"
		);
	}

	///描画処理
	void CubeObject::DrawObject(){
		auto Dev = App::GetApp()->GetDeviceResources();
		//コマンドリストのリセット
		if (m_Trace) {
			ThrowIfFailed(m_CommandList->Reset(Dev->GetCommandAllocator().Get(), m_CullFrontPipelineState.Get()),
				L"コマンドリストのリセットに失敗しました",
				L"m_CommandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CubeObject::OnDraw()"
			);
		}
		else {
			ThrowIfFailed(m_CommandList->Reset(Dev->GetCommandAllocator().Get(), m_CullBackPipelineState.Get()),
				L"コマンドリストのリセットに失敗しました",
				L"m_CommandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CubeObject::OnDraw()"
			);
		}
		//メッシュが更新されていればリソース更新
		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		//テクスチャが更新されていればリソース更新
		m_TextureResource->UpdateResources(m_CommandList);
		//ルートシグネチャのセット
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		//デスクプリタヒープのセット
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU側デスクプリタハンドルのセット
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		//ビューポートのセット
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		//シザーレクトのセット
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
		//レンダーターゲットビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//デプスステンシルビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//取得したハンドルをセット
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//描画方式（三角形リスト）をセット
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//頂点バッファをセット
		m_CommandList->IASetVertexBuffers(0, 1, &m_CubeMesh->GetVertexBufferView());
		//インデックスバッファをセット
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());
		//描画
		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			//透明の場合は、CullBackに変更して2回目描画
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);
		}
		//コマンドリストのクローズ
		ThrowIfFailed(m_CommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"m_CommandList->Close()",
			L"CubeObject::OnDraw()"
		);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}


	void CubeObject::OnCreate() {
		//メッシュの作成
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
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
	void CubeObject::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}
	void CubeObject::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		DrawObject();
	}





}
//end basecross
