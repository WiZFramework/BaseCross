/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//初期化
	void MultiSpark::OnCreate() {
	}


	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.5f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	///	ライン
	//--------------------------------------------------------------------------------------
	ActionLine::ActionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndtPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_EndPos(EndtPos)
	{}

	//初期化
	void ActionLine::OnCreate() {
		GetStage()->SetSharedGameObject(L"ActionLine", GetThis<ActionLine>());

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f));
		PtrTrans->SetQuaternion(Quat());
		PtrTrans->SetPosition(Vec3(0.0f));

		//描画コンポーネント
		auto PtrDraw = AddComponent<PCStaticDraw>();
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(m_StartPos,  Col4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(m_EndPos,  Col4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};
		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void ActionLine::OnUpdate() {


		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = m_StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = m_EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

	}


	//--------------------------------------------------------------------------------------
	//	タイリングするプレート
	//--------------------------------------------------------------------------------------
	//構築と破棄
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{
		try {
			if (m_UPic <= 0.0f || m_VPic <= 0.0f) {
				throw BaseException(
					L"UVに0以下は渡せません",
					L"if (m_UPic <= 0.0f || m_VPic <= 0.0f)",
					L"TilingPlate::TilingPlate()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	TilingPlate::~TilingPlate() {

	}
	//初期化
	void TilingPlate::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);
		AddComponent<CollisionRect>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"SKY_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"WALL_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}

	//--------------------------------------------------------------------------------------
	//	敵
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	Enemy::~Enemy(){}
	//初期化
	void Enemy::OnCreate() {
		//頂点表示用のメッシュ
		m_BallMesh = MeshResource::CreateSphere(0.2f, 9);

		GetStage()->SetSharedGameObject(L"Enemy", GetThis<Enemy>());
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_Scale);	
		Ptr->SetRotation(m_Rotation);
		Ptr->SetPosition(m_Position);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"CHECKER_TX");

		auto MeshRes = PtrDraw->GetMeshResource();
		auto& v_vec = MeshRes->GetVerteces();
		if (v_vec.size() == 0 || v_vec.size() % 3) {
			throw BaseException(
				L"メッシュの頂点が３で割り切れません",
				L"if (v_vec.size() == 0 || v_vec.size() % 3)",
				L"Enemy::OnCreate()"
			);

		}
		for (size_t i = 0; i < v_vec.size(); i++) {
			m_LocalTriangles.push_back(v_vec[i].position);
		}

	}


	void Enemy::OnDraw() {
		GameObject::OnDraw();

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//テクスチャとサンプラー
		//シェーダーリソースもクリア
		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//サンプラーもクリア
		RenderStatePtr->SetSamplerAllClear(pID3D11DeviceContext);
		//頂点シェーダ
		pID3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//個別処理
		SimpleConstants SmCb;
		//コンスタントバッファの作成
		//カメラを得る
		auto CameraPtr = OnGetDrawCamera();
		//ビューと射影行列を得る
		auto ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		auto ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//SmCb.World = World;
		SmCb.View = ViewMat;
		SmCb.Projection = ProjMat;
		//エミッシブ
		SmCb.Emissive = Col4(0, 0, 0, 1.0);
		//デフィーズ
		SmCb.Diffuse = Col4(1, 1, 1, 1.0);
		//スペキュラー
		SmCb.Specular = Col4(0.4, 0.4, 0.4, 1.0);
		//ライティング
		auto StageLight = OnGetDrawLight();
		SmCb.LightDir = StageLight.m_Directional;
		SmCb.LightDir.w = 1.0f;
		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//テクスチャ
		SmCb.ActiveFlg.x = 0;

		//ストライドとオフセット
		UINT stride = m_BallMesh->GetNumStride();
		UINT offset = 0;
		//描画方法のセット
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//頂点バッファのセット
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_BallMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(m_BallMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画コンポーネントの設定
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		auto MeshRes = PtrDraw->GetMeshResource();
		Mat4x4 WorldMatrix = GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : m_LocalTriangles) {
			Mat4x4 World;
			World.translation(v);
			World *= WorldMatrix;
			World.transpose();
			SmCb.World = World;
			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			//頂点シェーダに渡す
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//描画
			pID3D11DeviceContext->DrawIndexed(m_BallMesh->GetNumIndicis(), 0, 0);
		}
		//後始末
		Dev->InitializeStates();

	}



}
//end basecross
