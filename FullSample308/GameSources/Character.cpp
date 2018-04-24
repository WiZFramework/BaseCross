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
		ParticlePtr->SetMaxTime(0.1f);
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

		AddTag(L"Enemy");


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
	}

	bool Enemy::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint) {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		auto PtrTrans = GetComponent<Transform>();
		TRIANGLE tri;
		return PtrDraw->GetMeshResource()->IsHitSegmentTriangles(PtrTrans->GetWorldMatrix(), StartPos, EndPos, HitPoint, tri);
	}

	vector<Vec3>&  Enemy::WorldTriangles() {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		auto PtrTrans = GetComponent<Transform>();
		PtrDraw->GetMeshResource()->GetDrawingPositions(m_WorldTriangles, PtrTrans->GetWorldMatrix());
		return m_WorldTriangles;
	}


	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	BoneChara::~BoneChara() {}

	//初期化
	void BoneChara::OnCreate() {

		AddTag(L"Enemy");
		GetStage()->SetSharedGameObject(L"BoneChara", GetThis<BoneChara>());

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTBoneModelDraw>();
		PtrDraw->SetFogEnabled(true);
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"Chara_R_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		PtrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");

		//透明処理
		SetAlphaActive(true);
	}
	//更新
	void BoneChara::OnUpdate() {
		//アニメーションを更新する
		auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		PtrDraw->UpdateAnimation(ElapsedTime);

	}

	//--------------------------------------------------------------------------------------
	///	Bone三角形オブジェクト
	//--------------------------------------------------------------------------------------
	BoneTriangles::BoneTriangles(const shared_ptr<Stage>& StagePtr, const shared_ptr<BoneChara>& boneChara):
		GameObject(StagePtr),
		m_BoneChara(boneChara)
	{}

	void BoneTriangles::OnCreate() {
		GetStage()->SetSharedGameObject(L"BoneTriangles", GetThis<BoneTriangles>());
		auto ShBone = m_BoneChara.lock();
		if (ShBone) {
			auto PtrBoneTrans = ShBone->GetComponent<Transform>();
			auto PtrTrans = GetComponent<Transform>();
			PtrTrans->SetScale(PtrBoneTrans->GetScale());
			PtrTrans->SetQuaternion(PtrBoneTrans->GetQuaternion());
			PtrTrans->SetPosition(PtrBoneTrans->GetPosition());

			auto PtrBoneDraw = ShBone->GetComponent<BcPNTBoneModelDraw>();
			PtrBoneDraw->GetSkinedPositions(m_TempPosvec);
			if (m_TempPosvec.size() % 3) {
				return;
			}
			//描画コンポーネント
			auto PtrDraw = AddComponent<PCStaticDraw>();
			vector<uint16_t> indices;
			for (auto& v : m_TempPosvec) {
				VertexPositionColor poscol;
				poscol.position = v;
				poscol.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				m_BackupVertices.push_back(poscol);
			}
			for (size_t i = 0; i < m_BackupVertices.size(); i++) {
				indices.push_back((uint16_t)i);
			}
			m_TriangleMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
			PtrDraw->SetMeshResource(m_TriangleMesh);
			PtrDraw->SetRasterizerState(RasterizerState::Wireframe);
		}
	}
	void BoneTriangles::OnUpdate() {
		auto ShBone = m_BoneChara.lock();
		if (ShBone) {
			auto PtrBoneDraw = ShBone->GetComponent<BcPNTBoneModelDraw>();
			PtrBoneDraw->GetSkinedPositions(m_TempPosvec);
			m_BackupVertices.clear();
			for (auto& v : m_TempPosvec) {
				VertexPositionColor poscol;
				poscol.position = v;
				poscol.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				m_BackupVertices.push_back(poscol);
			}
			m_TriangleMesh->UpdateVirtexBuffer(m_BackupVertices);
		}
	}

	bool BoneTriangles::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint) {
		auto PtrTrans = GetComponent<Transform>();
		Mat4x4 Worldmat = PtrTrans->GetWorldMatrix();
		vector<Vec3> ChkPosvec(m_TempPosvec.size());


		for (size_t i = 0; i < m_TempPosvec.size();i++) {
			ChkPosvec[i] = m_TempPosvec[i] * Worldmat;
		}

		for (size_t i = 0; i < m_TempPosvec.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = ChkPosvec[i];
			tri.m_B = ChkPosvec[i + 1];
			tri.m_C = ChkPosvec[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				auto HitTrianglesPtr = GetStage()->GetSharedGameObject<HitTriangles>(L"HitTriangles");
				HitTrianglesPtr->AddHitTriangle(tri);
				return true;
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------
	///	ヒット時の三角形オブジェクト
	//--------------------------------------------------------------------------------------
	HitTriangles::HitTriangles(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr)
	{}

	void HitTriangles::OnCreate() {
		SetDrawLayer(2);
		GetStage()->SetSharedGameObject(L"HitTriangles", GetThis<HitTriangles>());
		vector<VertexPositionColor> vertices = {
			{ Vec3(0.0f, 0.5f , 0.0f),Col4(1.0f) },
			{ Vec3(0.5f, -0.5f, 0.5f),Col4(1.0f) },
			{ Vec3(-0.5f, -0.5f, 0.5f),Col4(1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, true);
	}

	void HitTriangles::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime > 0.0f) {
				v.m_LastTime -= ElapsedTime;
			}
		}
	}

	void HitTriangles::AddHitTriangle(const TRIANGLE& tri) {
		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime <= 0.0f) {
				v.m_Triangle = tri;
				v.m_LastTime = 1.0f;
				return;
			}
		}
		DrawTriangle d_tri(tri, 1.0f);
		m_DrawTriangleVec.push_back(d_tri);
	}

	void HitTriangles::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
//		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
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
		SmCb.Emissive = Col4(1, 1, 1, 1.0);
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
		UINT stride = m_TriangleMesh->GetNumStride();
		UINT offset = 0;
		//描画方法のセット
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//頂点バッファのセット
//		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
//		pID3D11DeviceContext->IASetIndexBuffer(m_BallMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		for (auto& v : m_DrawTriangleVec) {
			if (v.m_LastTime > 0.0f) {
				vector<VertexPositionColor> vertices = {
					{ v.m_Triangle.m_A,Col4(0.0f,0.0f,1.0f,1.0f) },
					{ v.m_Triangle.m_B,Col4(0.0f,0.0f,1.0f,1.0f) },
					{ v.m_Triangle.m_C,Col4(0.0f,0.0f,1.0f,1.0f) }
				};
				m_TriangleMesh->UpdateVirtexBuffer(vertices);

				//頂点バッファのセット
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				Mat4x4 World;
				World.identity();
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
				pID3D11DeviceContext->Draw(3, 0);
			}
		}
		//後始末
		Dev->InitializeStates();
		m_DrawTriangleVec.clear();
	}




}
//end basecross
