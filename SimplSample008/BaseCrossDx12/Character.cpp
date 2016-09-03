/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	スプライト描画(親クラス)
	//--------------------------------------------------------------------------------------
	SpriteDraw::SpriteDraw(const wstring& TextureFileName, bool Trace) :
		m_TextureFileName(TextureFileName),
		m_Trace(Trace)
	{}

	void SpriteDraw::OnCreate() {
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
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
	}

	///ルートシグネチャ作成
	void SpriteDraw::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}

	///デスクプリタヒープ作成
	void SpriteDraw::CreateDescriptorHeap() {
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
	void SpriteDraw::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		//ラッピングサンプラー
		DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
	}

	///シェーダーリソースビュー作成
	void SpriteDraw::CreateShaderResourceView() {
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

	///コンスタントバッファ作成
	void SpriteDraw::CreateConstantBufferBase(UINT BuffSize) {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コンスタントバッファリソース（アップロードヒープ）の作成
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(BuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"SpriteDraw::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//コンスタントバッファは256バイトにアラインメント
		cbvDesc.SizeInBytes = (BuffSize + 255) & ~255;
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
			L"SpriteDraw::CreateConstantBuffer()"
		);
	}


	///コマンドリスト作成
	void SpriteDraw::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}


	//--------------------------------------------------------------------------------------
	///	PCTスプライト描画
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const wstring& TextureFileName, bool Trace) :
		SpriteDraw(TextureFileName, Trace)
	{}
	//コンスタントバッファ作成
	void PCTSpriteDraw::CreateConstantBuffer() {
		CreateConstantBufferBase(sizeof(SpriteConstantBuffer));
	}
	///パイプラインステート作成
	void PCTSpriteDraw::CreatePipelineState() {
		CreatePipelineStateBase<VertexPositionColorTexture, VSPCTSprite, PSPCTSprite>();
	}
	///コンスタントバッファ更新
	void PCTSpriteDraw::UpdateConstantBuffer(SpriteConstantBuffer& CBuff) {
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&CBuff),
			sizeof(CBuff));

	}
	///描画処理
	void PCTSpriteDraw::DrawObject(const shared_ptr<MeshResource>& Mesh) {
		DrawObjectBase<VertexPositionColorTexture>(Mesh);
	}

	//--------------------------------------------------------------------------------------
	///	PTスプライト描画
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const wstring& TextureFileName, bool Trace) :
		SpriteDraw(TextureFileName, Trace)
	{}

	///コンスタントバッファ作成
	void PTSpriteDraw::CreateConstantBuffer() {
		CreateConstantBufferBase(sizeof(DiffuseSpriteConstantBuffer));
	}
	///パイプラインステート作成
	void PTSpriteDraw::CreatePipelineState() {
		CreatePipelineStateBase<VertexPositionTexture, VSPTSprite, PSPTSprite>();
	}

	///コンスタントバッファ更新
	void PTSpriteDraw::UpdateConstantBuffer(DiffuseSpriteConstantBuffer& CBuff) {
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&CBuff),
			sizeof(CBuff));

	}
	///描画処理
	void PTSpriteDraw::DrawObject(const shared_ptr<MeshResource>& Mesh) {
		DrawObjectBase<VertexPositionTexture>(Mesh);
	}

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

		//描画オブジェクトの作成
		m_PCTSpriteDraw = ObjectFactory::Create<PCTSpriteDraw>(m_TextureFileName, m_Trace);
		//矩形の大きさ
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_Scale = Vector2(w, h);
		m_SpriteConstantBuffer.World = Matrix4X4EX::Identity();
		//コンスタントバッファ更新
		m_PCTSpriteDraw->UpdateConstantBuffer(m_SpriteConstantBuffer);
	}


	void WallSprite::OnDraw() {
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
		m_SpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_SpriteConstantBuffer.World = World;
		//コンスタントバッファ更新
		m_PCTSpriteDraw->UpdateConstantBuffer(m_SpriteConstantBuffer);
		//描画
		m_PCTSpriteDraw->DrawObject(m_SquareMesh);
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
		//描画オブジェクトの作成
		m_PTSpriteDraw = ObjectFactory::Create<PTSpriteDraw>(m_TextureFileName, m_Trace);
		//矩形の大きさ
		m_Scale = Vector2(512.0f, 128.0f);
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
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//コンスタントバッファ更新
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);

	}

	//頂点の変更
	void SquareSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		//頂点の変更
		vector<VertexPositionTexture> new_vertices;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			VertexPositionTexture v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			new_vertices.push_back(v);
		}
		//メッシュの頂点の変更
		m_SquareMesh->UpdateVirtex(new_vertices);
	}


	void SquareSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateVertex(ElapsedTime);
	}

	void SquareSprite::OnDraw() {

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
		m_DiffuseSpriteConstantBuffer.Emissive = Color4(0, 0, 0, 0.0f);
		m_DiffuseSpriteConstantBuffer.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		m_DiffuseSpriteConstantBuffer.World = World;
		//コンスタントバッファ更新
		m_PTSpriteDraw->UpdateConstantBuffer(m_DiffuseSpriteConstantBuffer);
		//描画
		m_PTSpriteDraw->DrawObject(m_SquareMesh);
	}

}
//end basecross
