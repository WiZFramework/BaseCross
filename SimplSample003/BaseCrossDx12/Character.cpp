/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	四角形スプライト実体
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite() :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(128.0f, 128.0f),
		m_Rot(0),
		m_Pos(0, 0),
		m_PosSpan(3.0f, 0)
	{}
	SquareSprite::~SquareSprite() {}

	//コンスタントバッファ更新
	void SquareSprite::UpdateConstantBuffer() {
		//行列の定義
		Matrix4X4 World, Proj;
		//ワールド行列の決定
		World.AffineTransformation2D(
			m_Scale,			//スケーリング
			Vector2(0, 0),		//回転の中心（重心）
			m_Rot,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.OrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
		m_SpriteConstantBuffer.World = World;
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer),
			sizeof(m_SpriteConstantBuffer));
	}



	///ルートシグネチャ作成
	void SquareSprite::CreateRootSignature() {
		//コンスタントバッファ付ルートシグネチャ
		m_RootSignature = RootSignature::CreateCbv();
	}
	///デスクプリタヒープ作成
	void SquareSprite::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrvデスクプリタヒープ(コンスタントバッファのみ)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//GPU側デスクプリタヒープのハンドルの配列の作成
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///コンスタントバッファ作成
	void SquareSprite::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コンスタントバッファは256バイトにアラインメント
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"SquareSprite::CreateConstantBuffer()"
		);
		//コンスタントバッファのビューを作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
		//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//コンスタントバッファのアップロードヒープのマップ
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"コンスタントバッファのマップに失敗しました",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"SquareSprite::CreateConstantBuffer()"
		);
	}
	///パイプラインステート作成
	void SquareSprite::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
	}
	///コマンドリスト作成
	void SquareSprite::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///描画処理
	void SquareSprite::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コマンドリストのリセット
		CommandList::Reset(m_PipelineState, m_CommandList);
		//メッシュが更新されていればリソース更新
		m_SquareMesh->UpdateResources<VertexPositionColor>(m_CommandList);
		//ルートシグネチャのセット
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		//デスクプリタヒープのセット
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPUデスクプリタヒープハンドルのセット
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		//ビューポートのセット
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		//シザー矩形（クリップ矩形）のセット
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		//レンダーターゲットビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//デプスステンシルビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//取得したハンドルをセット
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//描画方法のセット（三角形リスト）
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//頂点バッファのセット
		m_CommandList->IASetVertexBuffers(0, 1, &m_SquareMesh->GetVertexBufferView());
		//インデックスバッファをセット
		m_CommandList->IASetIndexBuffer(&m_SquareMesh->GetIndexBufferView());
		//インデックス描画
		m_CommandList->DrawIndexedInstanced(m_SquareMesh->GetNumIndicis(), 1, 0, 0, 0);

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());


	}


	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		///ルートシグネチャ作成
		CreateRootSignature();
		///デスクプリタヒープ作成
		CreateDescriptorHeap();
		///コンスタントバッファ作成
		CreateConstantBuffer();
		///パイプラインステート作成
		CreatePipelineState();
		///コマンドリスト作成
		CreateCommandList();
		//コンスタントバッファの更新
		UpdateConstantBuffer();
	}

	void SquareSprite::OnUpdate() {
		if (m_PosSpan.x > 0) {
			m_Rot -= 0.05f;
		}
		else {
			m_Rot += 0.05f;
		}
		m_Pos += m_PosSpan;
		if (abs(m_Pos.x) > 400.0f) {
			m_PosSpan *= -1.0f;
		}
	}

	void SquareSprite::OnDraw() {
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//描画
		DrawObject();
	}


}
//end basecross
