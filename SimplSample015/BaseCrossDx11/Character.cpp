/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	平面実体
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vector3(-HelfSize, HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(HelfSize, HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(-HelfSize, -HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(HelfSize, -HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(1.0f, 1.0f)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Matrix4X4 World;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SquareMesh,
			m_TextureResource,
			World,
			false
		);
	}



	//--------------------------------------------------------------------------------------
	///	球実体
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Gravity(0,-9.8f,0),
		m_GravityVelocity(0,0,0),
		m_JumpLock(false)
	{}
	SphereObject::~SphereObject() {}

	SPHERE SphereObject::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = m_Pos;
		sp.m_Radius =  m_Scale.x * 0.5f;
		return sp;
	}

	void SphereObject::CollisionWithBoxes(const Vector3& BeforePos) {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//衝突判定
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxObjectVec()) {
			OBB Obb = v->GetOBB();
			SPHERE Sp = GetSPHERE();
			float HitTime;
			Vector3 CollisionVelosity = (m_Pos - BeforePos) / ElapsedTime;
			if (HitTest::CollisionTestSphereObb(Sp, CollisionVelosity, Obb, 0, ElapsedTime, HitTime)) {
				m_JumpLock = false;
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Posが動いたのでSPHEREを再取得
				Sp = GetSPHERE();
				Vector3 HitPoint;
				//最近接点を得るための判定
				HitTest::SPHERE_OBB(Sp, Obb, HitPoint);
				//衝突法線をHitPointとm_Posから導く
				Vector3 Normal = m_Pos - HitPoint;
				Normal.Normalize();
				if (Vector3EX::AngleBetweenNormals(Normal, Vector3(0, 1, 0)) <= 0.01f) {
					//平面の上
					m_GravityVelocity = Vector3(0, 0, 0);
				}
				else {
					//重力をスライドさせて設定する
					//これで、斜めのボックスを滑り落ちるようになる
					m_GravityVelocity = Vector3EX::Slide(m_GravityVelocity, Normal);
				}
				//速度をスライドさせて設定する
				m_Velocity = Vector3EX::Slide(m_Velocity, Normal);
				//Y方向は重力に任せる
				m_Velocity.y = 0;
				//最後に衝突点から余った時間分だけ新しい値で移動させる
				m_Pos = m_Pos + m_Velocity * SpanTime;
				m_Pos = m_Pos + m_GravityVelocity * SpanTime;
				//もう一度衝突判定
				//m_Posが動いたのでSPHEREを再取得
				Sp = GetSPHERE();
				if (HitTest::SPHERE_OBB(Sp, Obb, HitPoint)) {
					//衝突していたら追い出し処理
					Vector3 EscapeNormal = Sp.m_Center - HitPoint;
					EscapeNormal.Normalize();
					m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
				}
			}
		}
	}



	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division,vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
	}
	void SphereObject::OnUpdate() {
		//1つ前の位置を取っておく
		Vector3 BeforrPos = m_Pos;
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//キーボードとマウスの取得
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();

		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					m_GravityVelocity = Vector3(0, 6.0f, 0);
					m_JumpLock = true;
				}
			}
			if (CntlVec[0].fThumbLX != 0) {
				m_Velocity.x = CntlVec[0].fThumbLX * 5.0f;
			}
			else {
				m_Velocity.x *= 0.1f;
				if (abs(m_Velocity.x) <= 0.01f) {
					m_Velocity.x = 0;
				}
			}
			if (CntlVec[0].fThumbLY != 0) {
				m_Velocity.z = CntlVec[0].fThumbLY * 5.0f;
			}
			else {
				m_Velocity.z *= 0.1f;
				if (abs(m_Velocity.z) <= 0.01f) {
					m_Velocity.z = 0;
				}
			}
		}
		m_Pos += (m_Velocity * ElapsedTime);
		m_GravityVelocity += m_Gravity * ElapsedTime;
		m_Pos += m_GravityVelocity * ElapsedTime;
		if (m_Pos.y <= 0.5f) {
			m_Pos.y = 0.5f;
			m_GravityVelocity = Vector3(0,0,0);
			m_JumpLock = false;
		}
		CollisionWithBoxes(BeforrPos);
	}
	void SphereObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Matrix4X4 World;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	ボックス実体
	//--------------------------------------------------------------------------------------
	BoxObject::BoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace, 
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	BoxObject::~BoxObject() {}

	OBB BoxObject::GetOBB()const {
		Matrix4X4 World;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void BoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void BoxObject::OnUpdate() {
	}

	void BoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Matrix4X4 World;
		//ワールド行列の決定
		World.AffineTransformation(
			m_Scale,			//スケーリング
			Vector3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_BoxMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Scene> PtrScene) :
		m_Scene(PtrScene)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Matrix4X4& WorldMat,
		bool Trace) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		m_DrawObjectVec.push_back(Obj);
	}

	void PNTDrawObject::OnUpdate() {
		m_DrawObjectVec.clear();
	}

	void PNTDrawObject::OnDraw() {
		if (m_Scene.expired()) {
			//シーンが無効ならリターン
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//サンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//行列の定義
		Matrix4X4 View, Proj;
		//ライティング
		Vector4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.Transpose();
		//射影行列の決定
		//転置する
		Proj.Transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		//個別処理
		for (auto& v : m_DrawObjectVec) {
			//転置する
			v.m_WorldMatrix.Transpose();
			//ワールド行列の決定
			sb.World = v.m_WorldMatrix;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//テクスチャの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//ブレンドステート
			if (v.m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
		//後始末
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	ラッピング処理されたスプライト
	//--------------------------------------------------------------------------------------
	WrappedSprite::WrappedSprite(const wstring& TextureFileName, bool Trace, 
		const Vector2& StartScale, const Vector2& StartPos,
		UINT XWrap, UINT YWrap) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(StartScale),
		m_Rot(0),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_TotalTime(0)
	{}
	WrappedSprite::~WrappedSprite() {}
	void WrappedSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横10個ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,0,0,1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(0, 1.0f, 0, 1.0f), Vector2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(0, 0, 1.0f, 1.0f), Vector2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 1.0f, 0, 1.0f), Vector2((float)m_XWrap, (float)m_YWrap)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
	}

	void WrappedSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);

		}
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);

	}


	void WrappedSprite::OnUpdate() {
		m_Rot += 0.01f;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateVertex(ElapsedTime);
	}



	void WrappedSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

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
		SpriteConstantBuffer sb;
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_Trace) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


}
//end basecross
