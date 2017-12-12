/*!
@file PhysicsManager.cpp
@brief 物理計算用マネージャなど実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算用マネージャ
	//--------------------------------------------------------------------------------------
	PhysicsManager::PhysicsManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		BasePhysics()
	{
	}
	PhysicsManager::~PhysicsManager() {}

	shared_ptr<PhysicsBox> PhysicsManager::AddSingleBox(const PsBoxParam& param, const wstring& indexKey) {
		if (!m_BoxMeshRes) {
			//ワイアフレーム用メッシュの作成（変更できない）
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(2.0f, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_BoxMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		return BasePhysics::AddSingleBox(param, indexKey);

	}

	shared_ptr<PhysicsSphere> PhysicsManager::AddSingleSphere(const PsSphereParam& param, const wstring& indexKey) {
		if (!m_SphereMeshRes) {
			//ワイアフレーム用メッシュの作成（変更できない）
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(2.0f,9, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_SphereMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		return BasePhysics::AddSingleSphere(param, indexKey);
	}



	void PhysicsManager::OnUpdate() {
		if (IsUpdateActive()) {
			BasePhysics::Update();
		}
	}


	void PhysicsManager::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		bsm::Mat4x4 World, ViewMat, ProjMat;
		World = world;
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//エミッシブ
		sb.Emissive = Col4(0, 0, 0, 0);
		//デフィーズはすべて通す
		sb.Diffuse = Col4(1, 1, 1, 1);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, res->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(res->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//ブレンドステート
		//透明処理しない
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ラスタライザステート(ワイアフレーム)
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		pD3D11DeviceContext->DrawIndexed(res->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();

	}


	void PhysicsManager::OnDraw() {
		if (!IsDrawActive()) {
			return;
		}
		for (auto i = 0; i < GetNumBodies(); i++) {
			//行列の定義
			bsm::Mat4x4 World, Local;
			//bsm::Vec3 Pos;
			//bsm::Quat Qt;
			PsBodyStatus Status;
			GetBodyStatus(i, Status);
			//ワールド行列の決定
			World.affineTransformation(
				bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
				bsm::Vec3(0, 0, 0),		//回転の中心（重心）
				Status.m_Orientation,				//回転角度
				Status.m_Position			//位置
			);
			for (auto j = 0; j <  GetNumShapes(i); j++) {
				bsm::Vec3 LocalPos;
				bsm::Quat LocalQt;
				GetShapeOffsetQuatPos(i, j, LocalQt, LocalPos);
				switch (GetShapeType(i,j)) {
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeBox:
				{
					bsm::Vec3 scale = GetShapeBoxScale(i, j);
					Local.affineTransformation(
						scale,			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(m_BoxMeshRes, DrawWorld);
				}
				break;
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeSphere:
				{
					auto rad = GetShapeSphereRadius(i, j);
					Local.affineTransformation(
						bsm::Vec3(rad),			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(m_SphereMeshRes, DrawWorld);

				}
				break;
				}
			}
		}

	}



}
