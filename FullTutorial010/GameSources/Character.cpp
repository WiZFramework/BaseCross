/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	用途: 剣
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Sword::Sword(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const wstring& Tag
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Tag(Tag)
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

	//構築と破棄
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Position,
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
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position,
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
	// 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//初期化
	void MultiSpark::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
	}


	void MultiSpark::InsertSpark(const Vector3& Pos) {
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.3f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.2f - 0.1f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.2f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.2f - 0.1f;
			rParticleSprite.m_LocalScale = Vector3(0.5f, 0.5f, 0.5f);
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Color4(1.0f, 0.5f, 0.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	//複数の炎クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//初期化
	void MultiFire::OnCreate() {
		//加算描画処理をする
		SetAddType(true);
	}

	void MultiFire::InsertFire(const Vector3& Pos) {
		auto ParticlePtr = InsertParticle(16);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"FIRE_TX");
		ParticlePtr->SetMaxTime(4.0f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.01f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vector3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//色の指定
			rParticleSprite.m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}


	//--------------------------------------------------------------------------------------
	//	敵
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_Scale(Scale),
		m_StartRotation(Rotation),
		m_StartPosition(Position),
		m_MaxHP(100.0f),
		m_HP(100.0f)
	{
	}
	Enemy::~Enemy() {}

	//初期化
	void Enemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPosition);
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_StartRotation);
		//Rigidbodyをつける
		auto PtrRigid = AddComponent<Rigidbody>();
		//パス検索
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			throw BaseException(
				L"セルマップが不定です",
				L"if (!MapPtr) ",
				L" Enemy::OnCreate()"
			);
		}
		auto PathPtr = AddComponent<PathSearch>(MapPtr);

		//衝突判定をつける
		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetIsHitAction(IsHitAction::Slide);
//		PtrColl->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//透明処理をする
		SetAlphaActive(true);

		//セル検索の行動の構築
		m_CellSearchBehavior.reset(new CellSearchBehavior<Enemy>(GetThis<Enemy>(), m_CelMap));
		//戦いの行動の構築
		m_FightBehavior.reset(new FightBehavior<Enemy>(GetThis<Enemy>()));

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//更新
	void Enemy::OnUpdate() {
		if (GetStateMachine()->GetCurrentState() != EnemyDieState::Instance()){
			//ステートによって変わらない行動を実行
			auto PtrGrav = GetBehavior<Gravity>();
			PtrGrav->Execute();
		}
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void Enemy::OnUpdate2() {
		if (GetStateMachine()->GetCurrentState() == EnemySwordState::Instance()
			|| GetStateMachine()->GetCurrentState() == EnemySwordHit::Instance()) {
			RotToPlayer();
		}
		else if (GetStateMachine()->GetCurrentState() != EnemyDieState::Instance()) {
			//進行方向を向くようにする
			RotToHead();
		}
	}

	void Enemy::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		auto SrcColl = GetComponent<CollisionCapsule>();
		//敵が何かに当たった
		for (auto&v : OtherVec) {
			if (v->FindTag(L"PlayerSword")) {
				//剣に当たった
				auto DestColl = v->GetComponent<CollisionCapsule>();
				//剣のコリジョンを一時的に無効にする
				DestColl->SetUpdateActive(false);
				//スパークの放出
				auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark", false);
				if (PtrSpark) {
					Vector3 Pos1, Pos2;
					HitTest::CAPSULE_CAPSULE(SrcColl->GetCapsule(), DestColl->GetCapsule(), Pos1, Pos2);
					PtrSpark->InsertSpark(Pos1);
				}
				//HPを下げる
				m_HP -= 5.0f;
				if (m_HP <= 0.0f) {
					m_HP = 0.0f;
					//ファイアの放出
					auto PtriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
					if (PtriFire) {
						PtriFire->InsertFire(GetComponent<Transform>()->GetPosition());
					}
					//倒れる
					GetStateMachine()->ChangeState(EnemyDieState::Instance());
				}
				else {
					auto HaveSordPtr = v->GetComponent<Transform>()->GetParent();
					m_FightBehavior->SetHitBase(HaveSordPtr->GetComponent<Transform>()->GetPosition());
					GetStateMachine()->ChangeState(EnemySwordHit::Instance());
				}
				return;
			}
		}
	}



	//進行方向を向くようにする
	void Enemy::RotToHead() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	//プレイヤー方向を向くようにする
	void Enemy::RotToPlayer() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Vector3 Velocity = PlayerPos - Pos;
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（5分の1）
			NowQt.Slerp(NowQt, Qt, 0.2f);
			PtrTransform->SetQuaternion(NowQt);
		}

	}





	//--------------------------------------------------------------------------------------
	///	デフォルトステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefault)

	void EnemyDefault::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->HaveSword(L"EnemySword");
	}

	void EnemyDefault::Execute(const shared_ptr<Enemy>& Obj) {
		auto PlayerPtr = Obj->GetStage()->GetSharedGameObject<Player>(L"Player");
		//プレイヤーが倒れていたら自分の位置に戻る
		if (PlayerPtr->GetStateMachine()->GetCurrentState() == PlayerDieState::Instance()) {
			if (Obj->GetCellSearchBehavior()->SeekBehavior(Obj->GetStartPosition()) == CellSearchFlg::Arrived) {
				Obj->GetCellSearchBehavior()->ArriveBehavior(Obj->GetStartPosition());
			}
		}
		else {
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = Obj->GetStartPosition().y;
			if (Obj->GetCellSearchBehavior()->SeekBehavior(PlayerPos) == CellSearchFlg::Failed) {
				if (Obj->GetCellSearchBehavior()->SeekBehavior(Obj->GetStartPosition()) == CellSearchFlg::Arrived) {
					Obj->GetCellSearchBehavior()->ArriveBehavior(Obj->GetStartPosition());
				}
			}
			else {
				auto EnemyPos = Obj->GetComponent<Transform>()->GetPosition();
				if (Vector3EX::Length(EnemyPos - PlayerPos) <= 1.8f) {
					Obj->GetStateMachine()->ChangeState(EnemySwordState::Instance());
				}
			}
		}


	}
	void EnemyDefault::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	敵が剣を振るステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(EnemySwordState)

	void EnemySwordState::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->StartShakeSword();
	}

	void EnemySwordState::Execute(const shared_ptr<Enemy>& Obj) {
		auto PlayerPtr = Obj->GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Obj->GetCellSearchBehavior()->SeekBehavior(PlayerPos);
		if (Obj->GetFightBehavior()->RotationShakeSword()) {
			//半周回転させたらデフォルトステートに移行
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySwordState::Exit(const shared_ptr<Enemy>& Obj) {
		//何もしない
	}



	//--------------------------------------------------------------------------------------
	///	プレイヤーの剣に当たったステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(EnemySwordHit)

	void EnemySwordHit::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->HitSwordBehavior();
	}

	void EnemySwordHit::Execute(const shared_ptr<Enemy>& Obj) {
		if (Obj->GetFightBehavior()->HitAfterdBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySwordHit::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	倒されたステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDieState)

	void EnemyDieState::Enter(const shared_ptr<Enemy>& Obj) {
		Obj->GetFightBehavior()->DieStartBehavior();
	}

	void EnemyDieState::Execute(const shared_ptr<Enemy>& Obj) {
		if (Obj->GetFightBehavior()->DieBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemyDieState::Exit(const shared_ptr<Enemy>& Obj) {
		//HPをもとに戻す
		Obj->SetHP(Obj->GetMaxHP());
	}



	//--------------------------------------------------------------------------------------
	//	HPのスクエアの領域
	//--------------------------------------------------------------------------------------
	//構築と破棄
	HPSquareBase::HPSquareBase(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	HPSquareBase::~HPSquareBase() {}

	void HPSquareBase::OnCreate() {
		auto PtrDraw = AddComponent<PCStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PC_SQUARE");
		PtrDraw->SetDiffuse(Color4(0, 0, 0, 1));

	}

	//--------------------------------------------------------------------------------------
	//	HPのスクエア
	//--------------------------------------------------------------------------------------
	//構築と破棄
	HPSquare::HPSquare(const shared_ptr<Stage>& StagePtr, const Color4& Col) :
		GameObject(StagePtr),
		m_Color(Col)
	{
	}
	HPSquare::~HPSquare() {}

	void HPSquare::OnCreate() {
		auto PtrDraw = AddComponent<PCStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PC_SQUARE");
		PtrDraw->SetDiffuse(m_Color);
		SetAlphaActive(true);

	}




}
//end basecross
