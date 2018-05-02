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
	//	用途: 剣
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Sword::Sword(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const wstring& Tag
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Tag(Tag),
		m_Strength(5.0f)
	{
	}

	Sword::~Sword() {

	}
	//初期化
	void Sword::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		AddTag(m_Tag);
		//衝突判定をつける(CAPSULEを取り出すためのみ)
		auto PtrCol = AddComponent<CollisionCapsule>();
		PtrCol->SetIsHitAction(IsHitAction::None);
		if (m_Tag == L"PlayerSword") {
			PtrCol->AddExcludeCollisionTag(L"Player");
		}
		else if (m_Tag == L"EnemySword") {
			PtrCol->AddExcludeCollisionTag(L"Enemy");
		}
		//	PtrCol->SetDrawActive(true);
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		//描画するテクスチャを設定
		if (m_Tag == L"PlayerSword") {
			PtrDraw->SetTextureResource(L"BLUE_TX");
		}
		else if (m_Tag == L"EnemySword") {
			PtrDraw->SetTextureResource(L"RED_TX");
		}
	}

	//操作
	void Sword::OnUpdate() {
		float Val = m_Strength / 100.0f * 2.0f;
		Col4 Col = Col4(Val, Val, Val, 0);
		//描画コンポーネントの設定
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		PtrDraw->SetEmissive(Col);
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
	//	敵の箱
	//--------------------------------------------------------------------------------------
	//構築と破棄
	EnemyBox::EnemyBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	EnemyBox::~EnemyBox(){}
	//初期化
	void EnemyBox::OnCreate() {
		GetStage()->SetSharedGameObject(L"EnemyBox", GetThis<EnemyBox>());
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

	bool EnemyBox::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint) {
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		TRIANGLE tri;
		return PtrDraw->HitTestStaticMeshSegmentTriangles(StartPos, EndPos, HitPoint, tri);
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

	bool BoneChara::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint) {
		auto PtrDraw = GetComponent<BcPNTBoneModelDraw>();
		TRIANGLE tri;
		return PtrDraw->HitTestSkinedMeshSegmentTriangles(StartPos, EndPos, HitPoint, tri);

	}


}
//end basecross
