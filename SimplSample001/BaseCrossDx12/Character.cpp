/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	三角形オブジェクト
	//--------------------------------------------------------------------------------------
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface()
	{
	}
	TriangleObject::~TriangleObject() {}

	///ルートシグネチャ作成
	void TriangleObject::CreateRootSignature() {
		m_RootSignature = RootSignature::CreateSimple();
	}

	///パイプラインステート作成
	void TriangleObject::CreatePipelineState() {
		//パイプラインステートの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState 
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCDirect, PSPCDirect>(m_RootSignature, PineLineDesc);
	}

	///コマンドリスト作成
	void TriangleObject::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}



	void TriangleObject::OnCreate() {
		//頂点を作成するための配列
		vector<VertexPositionColor> vertices = {
			{ Vector3(0.0f, 0.5f , 0.0f),Color4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3(0.5f, -0.5f, 0.5f),Color4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3(-0.5f, -0.5f, 0.5f),Color4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices,false);
		CreateRootSignature();
		CreatePipelineState();
		CreateCommandList();
	}
	void TriangleObject::OnUpdate() {
		//何もしない
	}

	///描画処理
	void TriangleObject::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//コマンドリストのリセット
		CommandList::Reset(m_PipelineState, m_CommandList);
		//メッシュが更新されていればリソース更新
		m_TriangleMesh->UpdateResources<VertexPositionColor>(m_CommandList);

		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		//レンダーターゲットビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//デプスステンシルビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//取得したハンドルをセット
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_TriangleMesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(m_TriangleMesh->GetNumVertices(), 1, 0, 0);

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}

	void TriangleObject::OnDraw() {
		//描画
		DrawObject();
	}


}
//end basecross
