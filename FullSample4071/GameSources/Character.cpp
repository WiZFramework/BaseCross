/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	カスタムシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	CustomShadowmap::CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr) {}

	CustomShadowmap::~CustomShadowmap() {}

	//操作
	void CustomShadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrMeshResource = GetMeshResource();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//ステージから0番目のライトを取り出す
		auto DrawLight = PtrGameObject->OnGetDrawLight();
		//ステージからカメラを取り出す
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();


		Mat4x4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();


		//ライトの取得
		Mat4x4 LightView, LightProj;

		Vec3 LightDir = -1.0 * DrawLight.m_Directional;
		Vec3 LightAt = PtrCamera->GetAt();
		Vec3 LightEye = LightAt + (LightDir * GetLightHeight());

		//ライトのビューと射影を計算
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());
		CustomShadowmapConstantBuffer Cb;

		Cb.mWorld[0] = transpose(RealWorldMatrix);
		Mat4x4 Left;
		Left.translation(Vec3(-5.0f, 0, 0));
		Left = RealWorldMatrix * Left;
		Cb.mWorld[1] = transpose(Left);
		Mat4x4 Right;
		Right.translation(Vec3(5.0f, 0, 0));
		Right = RealWorldMatrix * Right;
		Cb.mWorld[2] = transpose(Right);
		Cb.mView = transpose(LightView);
		Cb.mProj = transpose(LightProj);
		//これより描画処理
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBCustomShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
		//インプットレイアウトのセット
		pID3D11DeviceContext->IASetInputLayout(VSCustomShadowmap::GetPtr()->GetInputLayout());
		//ストライドとオフセット
		UINT stride = PtrMeshResource->GetNumStride();
		UINT offset = 0;

		auto GameObj = dynamic_pointer_cast<CustomDrawOctahedron>(GetGameObject());
		//頂点バッファをセット

		pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//頂点シェーダーのセット
		pID3D11DeviceContext->VSSetShader(VSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);
		//ジオメトリシェーダのセット
		pID3D11DeviceContext->GSSetShader(GSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);

		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//ピクセルシェーダはセットしない！
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBCustomShadowmap::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//コンスタントバッファをピクセルシェーダにセット
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		//描画
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();


	}

	//--------------------------------------------------------------------------------------
	//	ジオメトリシェーダを使った独自の描画コンポーネント
	//--------------------------------------------------------------------------------------
	CustomPNCStaticDraw::CustomPNCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr) {}
	//操作
	void CustomPNCStaticDraw::OnDraw() {
		auto PtrGameObject = GetGameObject();
		//メッシュがなければ描画しない
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"メッシュが作成されていません",
				L"if (!MeshRes)",
				L"CustomPNCStaticDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//カメラを得る
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//カメラの取得
		Mat4x4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		CustomDrawConstantBuffer cb1;
		//行列の設定(転置する)
		cb1.World[0] = transpose(PtrT->GetWorldMatrix());
		Mat4x4 Left;
		Left.translation(Vec3(-5.0f, 0, 0));
		Left = PtrT->GetWorldMatrix() * Left;
		cb1.World[1] = transpose(Left);
		Mat4x4 Right;
		Right.translation(Vec3(5.0f, 0, 0));
		Right = PtrT->GetWorldMatrix() * Right;
		cb1.World[2] = transpose(Right);
		cb1.View = transpose(View);
		cb1.Projection = transpose(Proj);
		//ライトの設定
		auto PtrLight = PtrGameObject->OnGetDrawLight();
		cb1.LightDir = PtrLight.m_Directional;
		cb1.LightDir.w = 1.0f;

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//テクスチャを取得
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//半透明処理
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->GSSetShader(GSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSCustomDraw::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBCustomDraw::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	//	独自の描画コンポーネントで描画する8面体
	//--------------------------------------------------------------------------------------
	//頂点の変更
	void CustomDrawOctahedron::UpdateCustomMesh() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		vector<VertexPositionNormalColor> newvec;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vec3 Pos = m_BackupVertices[i].position;
			if (Pos.y > 0) {
				//サインを使っていったり来たりするようにする
				Pos.y += sin(m_TotalTime);
			}
			newvec.push_back(
				VertexPositionNormalColor(
					Pos,
					m_BackupVertices[i].normal,
					m_BackupVertices[i].color
				)
			);
		}
		auto PtrDraw = GetComponent<CustomPNCStaticDraw>();
		PtrDraw->UpdateVertices(newvec);
	}

	//構築と破棄
	CustomDrawOctahedron::CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, 
		const Vec3& StartScale, const Vec3& StartRotation, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawOctahedron::~CustomDrawOctahedron() {
	}
	//初期化
	void CustomDrawOctahedron::OnCreate() {
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//正8面体
		MeshUtill::CreateOctahedron(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalColor new_v;
			new_v.position = vertices[i].position;
			new_v.normal = vertices[i].normal;
			new_v.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			m_BackupVertices.push_back(new_v);
		}
		auto PtrDraw = AddComponent<CustomPNCStaticDraw>();
		PtrDraw->CreateOriginalMesh(m_BackupVertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<CustomShadowmap>();
		ShadowPtr->SetMeshResource(PtrDraw->GetMeshResource());
		//透明処理（描画順制御のため）
		SetAlphaActive(true);
	}
	void CustomDrawOctahedron::OnUpdate() {
		UpdateCustomMesh();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quat Span;
		Span.rotationRollPitchYawFromVector(Vec3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}

}
//end basecross
