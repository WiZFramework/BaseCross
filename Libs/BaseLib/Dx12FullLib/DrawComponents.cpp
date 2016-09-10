/*!
@file DrawComponents.cpp
@brief 描画コンポーネント実体
*/

#include "stdafx.h"

namespace basecross {


	//これより個別シェーダー
	IMPLEMENT_DX12SHADER(VSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCDirect.cso")
	IMPLEMENT_DX12SHADER(PSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCDirect.cso")

	IMPLEMENT_DX12SHADER(VSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTDirect.cso")
	IMPLEMENT_DX12SHADER(PSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTDirect.cso")

	IMPLEMENT_DX12SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX12SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")

	IMPLEMENT_DX12SHADER(VSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")


	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		Matrix4X4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix() {}
	};

	//--------------------------------------------------------------------------------------
	///	描画コンポーネントの親クラス実体
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}
	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}
	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}

	const Matrix4X4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const Matrix4X4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース


		Impl()
		{}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);


	//--------------------------------------------------------------------------------------
	//	シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap() {}

	float Shadowmap::GetLightHeight() { return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear() { return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar() { return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth() { return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight() { return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }

	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const {
		if (!pImpl->m_MeshResource.expired()) {
			return pImpl->m_MeshResource.lock();
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
				);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
				);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr) {
		pImpl->m_MeshResource = MeshResourcePtr;
	}




	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {

		//メッシュリソース
		weak_ptr<MeshResource> m_MeshResource;
		//テクスチャ
		weak_ptr<TextureResource> m_TextureResource;
		bool m_OwnShadowActive;
		bool m_Trace;

		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///コンスタントバッファ
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		///コンスタントバッファのオブジェクト側変数
		StaticConstantBuffer m_StaticConstantBuffer;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		///パイプラインステート
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;

		ComPtr<ID3D12PipelineState> m_CullBackPipelineState;
		ComPtr<ID3D12PipelineState> m_CullFrontPipelineState;
		///コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		///各初期化関数
		///ルートシグネチャ作成
		void CreateRootSignature();
		///デスクプリタヒープ作成
		void CreateDescriptorHeap();
		///サンプラー作成
		void CreateSampler();
		///シェーダーリソースビュー作成
		void CreateShaderResourceView();
		///コンスタントバッファ作成
		void CreateConstantBuffer();
		///パイプラインステート作成
		void CreatePipelineState();
		///コマンドリスト作成
		void CreateCommandList();
		//コンスタントバッファ更新
		void UpdateConstantBuffer();
		///描画処理
		void DrawObject();

		///プロパティ変更
		void RefreshTrace();


		Impl() :
			m_OwnShadowActive(false), m_Trace(false)
		{
			//各行列をIdentityに初期化
			ZeroMemory(&m_StaticConstantBuffer, sizeof(m_StaticConstantBuffer));
			m_StaticConstantBuffer.World = Matrix4X4EX::Identity();
			m_StaticConstantBuffer.View = Matrix4X4EX::Identity();
			m_StaticConstantBuffer.Projection = Matrix4X4EX::Identity();
			m_StaticConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0.0f, 0.0f);
			m_StaticConstantBuffer.Diffuse = Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f);


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
		~Impl() {}
	};


	///ルートシグネチャ作成
	void PNTStaticDraw::Impl::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///デスクプリタヒープ作成
	void PNTStaticDraw::Impl::CreateDescriptorHeap() {
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
	void PNTStaticDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void PNTStaticDraw::Impl::CreateConstantBuffer() {
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
			L"PNTStaticDraw::Impl::CreateConstantBuffer()"
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
			L"PNTStaticDraw::Impl::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void PNTStaticDraw::Impl::CreateShaderResourceView() {
		auto ShPtr = m_TextureResource.lock();
		if (!ShPtr) {
			return;
		}
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
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//シェーダリソースビュー
		Dev->GetDevice()->CreateShaderResourceView(
			ShPtr->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///パイプラインステート作成
	void PNTStaticDraw::Impl::CreatePipelineState() {
		//パイプラインステートの作成
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, m_PineLineDesc);
		m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void PNTStaticDraw::Impl::RefreshTrace() {
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
			m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

	}


	///コマンドリスト作成
	void PNTStaticDraw::Impl::CreateCommandList() {
		//コマンドリストは裏面カリングに初期化
		m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
		CommandList::Close(m_CommandList);
	}

	void PNTStaticDraw::Impl::UpdateConstantBuffer() {
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
			sizeof(m_StaticConstantBuffer));
	}


	///描画処理
	void PNTStaticDraw::Impl::DrawObject() {
		auto ShMesh = m_MeshResource.lock();
		auto ShTex = m_TextureResource.lock();
		if (!ShMesh || !ShTex) {
			return;
		}

		//コマンドリストのリセット
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		ShTex->UpdateResources(m_CommandList);

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
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}





	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	用途: PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//メッシュがなければリターン
		auto shptr = pImpl->m_MeshResource.lock();
		if (!shptr) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (!shptr)",
				L"PNTStaticDraw::GetMeshResource()"
			);
		}
		return shptr;
	}

	void PNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
		pImpl->CreateShaderResourceView();
	}
	void PNTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
		pImpl->CreateShaderResourceView();
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr){
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	Color4 PNTStaticDraw::GetEmissive() const {
		return pImpl->m_StaticConstantBuffer.Emissive;
	}
	void PNTStaticDraw::SetEmissive(const Color4& col) {
		pImpl->m_StaticConstantBuffer.Emissive = col;
	}

	bool PNTStaticDraw::GetAlphaActive() const {
		return pImpl->m_Trace;

	}
	void PNTStaticDraw::SetAlphaActive(bool b) {
		if (b != pImpl->m_Trace) {
			pImpl->m_Trace = b;
			pImpl->RefreshTrace();
		}
	}

	bool PNTStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}


	void PNTStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		SetAlphaActive(GetGameObject()->GetAlphaActive());
		auto PtrMeshResource = GetMeshResource();
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//ワールド行列の決定
		pImpl->m_StaticConstantBuffer.World.AffineTransformation(
			PtrTrans->GetScale(),			//スケーリング
			PtrTrans->GetPivot(),		//回転の中心（重心）
			PtrTrans->GetQuaternion(),				//回転角度
			PtrTrans->GetPosition()				//位置
		);
		//転置する
		pImpl->m_StaticConstantBuffer.World.Transpose();
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		pImpl->m_StaticConstantBuffer.View = StageView->GetTargetCamera()->GetViewMatrix();
		//転置する
		pImpl->m_StaticConstantBuffer.View.Transpose();
		//射影行列の決定
		pImpl->m_StaticConstantBuffer.Projection = StageView->GetTargetCamera()->GetProjMatrix();
		//転置する
		pImpl->m_StaticConstantBuffer.Projection.Transpose();



		//ライティング
		auto StageLight = PtrStage->GetLight();
		pImpl->m_StaticConstantBuffer.LightDir = StageLight->GetTargetLight().m_Directional;
		//エミッシブ
		pImpl->m_StaticConstantBuffer.Emissive = GetEmissive();
		//ディフューズ
		pImpl->m_StaticConstantBuffer.Diffuse = StageLight->GetTargetLight().m_DiffuseColor;
		//更新
		pImpl->UpdateConstantBuffer();
		pImpl->DrawObject();
	}

	void PNTStaticDraw::OnDraw() {
		DrawNotShadow();
	}



}
//end basecross
