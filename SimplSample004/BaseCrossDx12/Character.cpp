/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	四角形スプライトの構造体
	//--------------------------------------------------------------------------------------
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
		//コンスタントバッファは256バイトにアラインメント
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;
		auto Dev = App::GetApp()->GetDeviceResources();
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
	//コンスタントバッファ更新
	void SquareSprite::UpdateConstantBuffer() {
		//行列の定義
		Mat4x4 World, Proj;
		//ワールド行列の決定
		World.affineTransformation2D(
			m_LocalScale,			//スケーリング
			Vec2(0, 0),		//回転の中心（重心）
			m_LocalRot,				//回転角度
			m_LocalPos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;
		//コンスタントバッファの準備
		m_SpriteConstantBuffer.Emissive = Col4(0.0f, 0.0f, 0, 1.0f);
		m_SpriteConstantBuffer.World = World;
		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer),
			sizeof(m_SpriteConstantBuffer));
	}

	//--------------------------------------------------------------------------------------
	///	四角形スプライトのグループ実体
	//--------------------------------------------------------------------------------------
	SquareSpriteGroup::SquareSpriteGroup() :
		ObjectInterface(),
		ShapeInterface(),
		m_TotalTime(0)
	{}
	SquareSpriteGroup::~SquareSpriteGroup() {}

	//コンスタントバッファ更新
	void SquareSpriteGroup::UpdateConstantBuffer() {
		for (auto& v : m_SquareSpriteVec) {
			v.UpdateConstantBuffer();
		}
	}


	///ルートシグネチャ作成
	void SquareSpriteGroup::CreateRootSignature() {
		//コンスタントバッファ付ルートシグネチャ
		m_RootSignature = RootSignature::CreateCbv();
	}
	///デスクプリタヒープ作成
	void SquareSpriteGroup::CreateDescriptorHeap() {
		for (auto& v : m_SquareSpriteVec) {
			v.CreateDescriptorHeap();
		}
	}
	///コンスタントバッファ作成
	void SquareSpriteGroup::CreateConstantBuffer() {

		for (auto& v : m_SquareSpriteVec) {
			v.CreateConstantBuffer();
		}
	}
	///パイプラインステート作成
	void SquareSpriteGroup::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
	}
	///コマンドリスト作成
	void SquareSpriteGroup::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///描画処理
	void SquareSpriteGroup::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コマンドリストのリセット
		CommandList::Reset(m_PipelineState, m_CommandList);
		//メッシュが更新されていればリソース更新
		m_SquareSpriteMesh->UpdateResources<VertexPositionColor>(m_CommandList);
		//ルートシグネチャのセット
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		//ビューポートとシザーレクト
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
		//レンダーターゲットビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//デプスステンシルビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//取得したハンドルをセット
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//三角形描画
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//頂点バッファのセット
		m_CommandList->IASetVertexBuffers(0, 1, &m_SquareSpriteMesh->GetVertexBufferView());
		//インデックスバッファをセット
		m_CommandList->IASetIndexBuffer(&m_SquareSpriteMesh->GetIndexBufferView());
		//各スプライトごとの処理
		for (auto& v : m_SquareSpriteVec) {
			//デスクプリタヒープのセット
			ID3D12DescriptorHeap* ppHeaps[] = { v.m_CbvSrvUavDescriptorHeap.Get() };
			m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
			//GPUデスクプリタヒープハンドルのセット
			for (size_t i = 0; i < v.m_GPUDescriptorHandleVec.size(); i++) {
				m_CommandList->SetGraphicsRootDescriptorTable(i, v.m_GPUDescriptorHandleVec[i]);
			}
			//インデックス描画
			m_CommandList->DrawIndexedInstanced(m_SquareSpriteMesh->GetNumIndicis(), 1, 0, 0, 0);
		}
		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	void SquareSpriteGroup::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, 0), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, 0), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成
		//頂点変更できない
		m_SquareSpriteMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//グループの配列の作成
		m_SquareSpriteVec.assign(100, SquareSprite());
		for (auto& v : m_SquareSpriteVec) {
			v.m_LocalRot = Util::RandZeroToOne(true);
			v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
			v.m_LocalPos = Vec2(0, 0);
			v.m_LocalVelocity = Vec2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
			v.m_LocalGravityVelocity = Vec2(0, 0);
		}
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

	}

	//各オブジェクトの位置等の変更
	void SquareSpriteGroup::UpdateObjects(float ElapsedTime) {
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		h /= 2.0f;
		for (auto& v : m_SquareSpriteVec) {
			if (v.m_LocalPos.y < -h) {
				v.m_LocalRot = Util::RandZeroToOne(true);
				v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
				v.m_LocalPos = Vec2(0, 0);
				v.m_LocalVelocity = Vec2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
				v.m_LocalGravityVelocity = Vec2(0, 0);
			}
			else {
				v.m_LocalRot += v.m_LocalRotVelocity * ElapsedTime;
				v.m_LocalPos += v.m_LocalVelocity * ElapsedTime;
				v.m_LocalGravityVelocity += Vec2(0, -98.0f) * ElapsedTime;
				v.m_LocalPos += v.m_LocalGravityVelocity * ElapsedTime;
			}
		}
	}


	void SquareSpriteGroup::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateObjects(ElapsedTime);
	}

	void SquareSpriteGroup::OnDraw() {
		UpdateConstantBuffer();
		DrawObject();
	}


}
//end basecross
