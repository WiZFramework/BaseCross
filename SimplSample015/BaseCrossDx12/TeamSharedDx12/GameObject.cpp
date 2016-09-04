/*!
@file GameObject.cpp
@brief コンポーネント、ゲームオブジェクト実体
*/

#include "stdafx.h"
#include "../Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	ゲームオブジェクトクラス
	//--------------------------------------------------------------------------------------
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const{
		auto it = m_CompMap.find(TypeIndex);
		if (it != m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr){
		if (!SearchComponent(TypeIndex)){
			//そのコンポーネントがまだなければ新規登録
			m_CompOrder.push_back(TypeIndex);
		}
		//mapに追加もしくは更新
		m_CompMap[TypeIndex] = Ptr;
		Ptr->SetGameObject(GetThis<GameObject>());
	}

	GameObject::GameObject(const shared_ptr<Scene>& ScenePtr) :
		m_Scene(ScenePtr)
	{}

	GameObject::~GameObject(){}

	void GameObject::ComponentPreparation(){
		auto TMptr = GetComponent<Transform>();
		TMptr->SetToBefore();
	}


	void GameObject::ComponentUpdate(){
		auto TMptr = GetComponent<Transform>();
		auto GravityPtr = GetComponent<Gravity>(false);
		//マップを検証してUpdate
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				it2->second->OnUpdate();
			}
			it++;
		}
		if (GravityPtr) {
			//GravityPtrがあればUpdate()
			GravityPtr->OnUpdate();
		}
		//TransformのUpdate
		TMptr->OnUpdate();
	}

	void GameObject::ComponentDraw(){
		//GravityやTransformはDrawを呼ばない
		//マップを検証してUpdate
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				it2->second->OnDraw();
			}
			it++;
		}

	}




	//--------------------------------------------------------------------------------------
	///	コンポーネント親クラス
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		m_GameObject(GameObjectPtr)
	{}

	Component::~Component(){}

	shared_ptr<GameObject> Component::GetGameObject() const{
		if (m_GameObject.expired()){
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (pImpl->m_GameObject.expired())",
				L"Component::GetGameObject()"
				);
		}
		else{
			return m_GameObject.lock();
		}
	}

	void Component::SetGameObject(const shared_ptr<GameObject>& GameObjectPtr){
		m_GameObject = GameObjectPtr;
	}

	//--------------------------------------------------------------------------------------
	///	重力コンポーネント
	//--------------------------------------------------------------------------------------
	void Gravity::StartJump(const Vector3& StartVec, float EscapeSpan){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		SetGravityVelocity(StartVec);
		SetGravityDefault();
		//ジャンプして親オブジェクトボリュームから脱出できないとき対応
		Vector3 EscapeVec = StartVec;
		EscapeVec *= EscapeSpan;
		Position += EscapeVec;
		//位置を設定
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::UpdateFromTime(float CalcTime){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		//自由落下加速度を計算
		m_GravityVelocity += m_Gravity * CalcTime;
		Position += m_GravityVelocity * CalcTime;
		if (Position.y <= m_BaseY){
			Position.y = m_BaseY;
			SetGravityVelocityZero();
			SetGravityZero();
		}
		//位置を設定
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	///ルートシグネチャ作成
	void PNTStaticDraw::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///デスクプリタヒープ作成
	void PNTStaticDraw::CreateDescriptorHeap() {
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
	void PNTStaticDraw::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void PNTStaticDraw::CreateConstantBuffer() {
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
			L"PNTStaticDraw::CreateConstantBuffer()"
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
			L"PNTStaticDraw::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void PNTStaticDraw::CreateShaderResourceView() {
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
	void PNTStaticDraw::CreatePipelineState() {
		//パイプラインステートの作成
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, m_PineLineDesc);
		m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
	}

	void PNTStaticDraw::RefreshTrace() {
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
	void PNTStaticDraw::CreateCommandList() {
		//コマンドリストは裏面カリングに初期化
		m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
		CommandList::Close(m_CommandList);
	}

	void PNTStaticDraw::UpdateConstantBuffer(PNTStaticConstantBuffer& Buff) {
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&Buff),
			sizeof(Buff));
	}
	void PNTStaticDraw::DrawObject() {
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




	void PNTStaticDraw::OnCreate() {
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

		//各行列をIdentityに初期化
		PNTStaticConstantBuffer sb;
		sb.World = Matrix4X4EX::Identity();
		sb.View = Matrix4X4EX::Identity();
		sb.Projection = Matrix4X4EX::Identity();
		sb.Emissive = Color4(0.0f, 0.0f, 0.0f, 0.0f);
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);

		//コンスタントバッファの更新
		UpdateConstantBuffer(sb);
	}

	void PNTStaticDraw::OnDraw(){
		auto GObject = GetGameObject();

		//行列の定義
		Matrix4X4 World, View, Proj;
		//ライティング
		Vector4 LightDir;
		auto PtrTrans = GObject->GetComponent<Transform>();
		World = PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		GObject->GetScene()->GetViewProjMatrix(View, Proj);
		GObject->GetScene()->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.Transpose();
		//射影行列の決定
		//転置する
		Proj.Transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);

		UpdateConstantBuffer(sb);
		DrawObject();

	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//メッシュがなければリターン
		auto shptr = m_MeshResource.lock();
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
		m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		m_TextureResource = TextureRes;
		CreateShaderResourceView();
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = m_TextureResource.lock();
		if (!shptr) {
			return nullptr;
		}
		return m_TextureResource.lock();
	}

	void PNTStaticDraw::SetTrace(bool b) {
		if (b != m_Trace) {
			m_Trace = b;
			RefreshTrace();
		}
	}





}
//end basecross
