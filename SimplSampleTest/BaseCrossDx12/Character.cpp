/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	NormalTextureBox::NormalTextureBox(const Vector3& Pos) :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Qt(),
		m_Pos(Pos)
	{
		m_Qt.Identity();
	}
	NormalTextureBox::~NormalTextureBox() {}

	///ルートシグネチャ作成
	void NormalTextureBox::CreateRootSignature() {
		//ルートシグネチャ
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///デスクプリタヒープ作成
	void NormalTextureBox::CreateDescriptorHeap() {
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
	void NormalTextureBox::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///コンスタントバッファ作成
	void NormalTextureBox::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コンスタントバッファリソース（アップロードヒープ）の作成
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(StaticConstantBuffer)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"NormalTextureBox::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//コンスタントバッファは256バイトにアラインメント
		cbvDesc.SizeInBytes = (sizeof(StaticConstantBuffer) + 255) & ~255;
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
			L"NormalTextureBox::CreateConstantBuffer()"
		);
	}
	///シェーダーリソースビュー（テクスチャ）作成
	void NormalTextureBox::CreateShaderResourceView() {
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
		srvDesc.Format = m_WallTex->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_WallTex->GetTextureResDesc().MipLevels;
		//シェーダリソースビュー
		Dev->GetDevice()->CreateShaderResourceView(
			m_WallTex->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///パイプラインステート作成
	void NormalTextureBox::CreatePipelineState() {
		//パイプラインステートの作成
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, PineLineDesc);
		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///コマンドリスト作成
	void NormalTextureBox::CreateCommandList() {
		//コマンドリストは裏面カリングに初期化
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void NormalTextureBox::UpdateConstantBuffer() {
		//行列の定義
		//ワールド行列の決定
		m_ConstantBufferData.World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		//転置する
		m_ConstantBufferData.World.Transpose();
		//ビュー行列の決定
		m_ConstantBufferData.View.LookAtLH(Vector3(0, 2.0, -5.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
		//転置する
		m_ConstantBufferData.View.Transpose();
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		m_ConstantBufferData.Projection.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//転置する
		m_ConstantBufferData.Projection.Transpose();
		//ライティング
		m_ConstantBufferData.LightDir = Vector4(0.5f, -1.0f, 0.5f, 1.0f);
		//エミッシブ
		m_ConstantBufferData.Emissive = Color4(0, 0, 0, 0);
		//ディフューズ
		m_ConstantBufferData.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_ConstantBufferData),
			sizeof(m_ConstantBufferData));
	}


	///描画処理
	void NormalTextureBox::DrawObject() {
		CommandList::Reset(m_PipelineState, m_CommandList);
		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		m_WallTex->UpdateResources(m_CommandList);

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
		m_CommandList->IASetVertexBuffers(0, 1, &m_CubeMesh->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}



	void NormalTextureBox::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallTex = TextureResource::CreateTextureResource(strTexture);
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
	void NormalTextureBox::OnUpdate() {
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		m_Qt *= QtSpan;
		m_Qt.Normalize();
	}



	void NormalTextureBox::OnDraw() {
		UpdateConstantBuffer();
		DrawObject();

	}


}
//end basecross
