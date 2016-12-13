/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//初期化
	void SeekObject::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//反発係数は0.5（半分）
		PtrRegid->SetReflection(0.5f);
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();


		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		//Obbの衝突判定をつける
//		auto PtrColl = AddComponent<CollisionObb>();
		//CollisionSphereの衝突判定をつける
		auto PtrColl = AddComponent<CollisionSphere>();
		//親の影響を受ける
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParentSlide);
		//Y方向のスケーリングで球を計算
		PtrColl->SetCalcScaling(CalcScaling::YScale);
		PtrColl->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//最初は元気な行動
		m_SeekBehavior = FineSeekBehavior::Instance();
		m_FineTiredTime = 0;
		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//ユーティリティ関数群
	Vector3 SeekObject::GetPlayerPosition() const {
		//もしプレイヤーが初期化化されてない場合には、Vector3(0,m_BaseY,0)を返す
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer) {
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}
	//操作
	void SeekObject::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}
	void SeekObject::OnLastUpdate() {
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

	//--------------------------------------------------------------------------------------
	//	Seekオブジェクトの元気な行動
	//--------------------------------------------------------------------------------------
	shared_ptr<FineSeekBehavior> FineSeekBehavior::Instance() {
		static shared_ptr<FineSeekBehavior> instance(new FineSeekBehavior);
		return instance;
	}

	void FineSeekBehavior::FarEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void FineSeekBehavior::FarExecute(const shared_ptr<SeekObject>& Obj){
		auto PtrSeek = Obj->GetComponent<SeekSteering>();

		//５秒経過したら疲れた行動になる
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Obj->m_FineTiredTime += ElapsedTime;
		if (Obj->m_FineTiredTime >= 3.0f) {
			Obj->m_FineTiredTime = 0;
			Obj->m_SeekBehavior = TiredSeekBehavior::Instance();
			PtrSeek->SetWeight(0.2f);
			auto PtrDraw = Obj->GetComponent<PNTStaticDraw>();
			PtrDraw->SetEmissive(Color4(0.5f, 0.0, 0.0, 1.0f));
		}

		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() <= Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FineSeekBehavior::FarExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}
	void FineSeekBehavior::NearEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void FineSeekBehavior::NearExecute(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() > Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}

	void FineSeekBehavior::NearExit(const shared_ptr<SeekObject>& Obj){
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		//Arriveコンポーネントを無効にする
		PtrArrive->SetUpdateActive(false);
	}

	//--------------------------------------------------------------------------------------
	//	Seekオブジェクトの疲れた行動
	//--------------------------------------------------------------------------------------
	shared_ptr<TiredSeekBehavior> TiredSeekBehavior::Instance() {
		static shared_ptr<TiredSeekBehavior> instance(new TiredSeekBehavior);
		return instance;
	}

	void TiredSeekBehavior::FarEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void TiredSeekBehavior::FarExecute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();

		//５秒経過したら元気な行動になる
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Obj->m_FineTiredTime += ElapsedTime;
		if (Obj->m_FineTiredTime >= 3.0f) {
			Obj->m_FineTiredTime = 0;
			Obj->m_SeekBehavior = FineSeekBehavior::Instance();
			PtrSeek->SetWeight(1.0f);
			auto PtrDraw = Obj->GetComponent<PNTStaticDraw>();
			PtrDraw->SetEmissive(Color4(0.0f, 0.0, 0.0, 0.0f));
		}


		PtrSeek->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() <= Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void TiredSeekBehavior::FarExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}
	void TiredSeekBehavior::NearEnter(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
	}
	void TiredSeekBehavior::NearExecute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		if (Obj->GetPlayerLength() > Obj->m_StateChangeSize) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}

	void TiredSeekBehavior::NearExit(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		//Arriveコンポーネントを無効にする
		PtrArrive->SetUpdateActive(false);
	}


	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->FarEnter(Obj);
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->FarExecute(Obj);
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->FarExit(Obj);
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->NearEnter(Obj);
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->NearExecute(Obj);
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
		Obj->GetSeekBehavior()->NearExit(Obj);
	}

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	用途: 固定のモデル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticModel::StaticModel(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	StaticModel::~StaticModel() {}

	//初期化
	void StaticModel::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		Matrix4X4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f,0, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f)
		);


		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"Chara_Rst_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Chara_Rst_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);
		PtrDraw->SetLighting(ShaderLighting::Midium);
		PtrDraw->SetEmissive(Color4(0.4, 0.4, 0.4, 0));
		PtrDraw->SetDiffuse(Color4(0.6, 0.6, 0.6, 1));

	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//初期化
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox() {}

	//初期化
	void MoveBox::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		Group->IntoGroup(GetThis<GameObject>());


	}

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject() {}
	//初期化
	void SphereObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphereの衝突判定をつける
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

	}

	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	CapsuleObject::~CapsuleObject() {}
	//初期化
	void CapsuleObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//Capsuleの衝突判定をつける
		auto CollPtr = AddComponent<CollisionCapsule>();
		CollPtr->SetFixed(true);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"WALL_TX");

	}

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	用途: でこぼこ床のデータ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	UnevenGroundData::UnevenGroundData(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	UnevenGroundData::~UnevenGroundData() {}
	//初期化
	void UnevenGroundData::OnCreate() {
		vector<VertexPositionNormalTexture> vertices =
		{
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(0.0f, 0.5f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.5f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(-1.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(0.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(0.5f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(1.0f, 0.0f, 0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(1.0f, 0.0f,-0.5f), Vector3(0.0f, 1.0f,0.0f), Vector2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2 ,
			1, 4, 5 ,
			3, 1, 5
		};
		wstring CheckStr = L"UnevenGroundMesh";
		//リソースが登録されているか確認し、登録されてなければ作成
		//こうしておくとほかのステージでもメッシュを使える
		if (!App::GetApp()->CheckResource<MeshResource>(CheckStr)) {
			//頂点を使って表示用メッシュリソースの作成
			auto mesh = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, false);
			//表示用メッシュをリソースに登録
			App::GetApp()->RegisterResource(CheckStr, mesh);
			//ワイアフレーム表示用のメッシュの作成
			vector<VertexPositionColor> new_vertices;
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_v;
				new_v.position = vertices[i].position;
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				new_vertices.push_back(new_v);
			}
			//ワイアフレームメッシュをリソースに登録
			App::GetApp()->RegisterResource(L"UnevenGroundWireMesh", MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
		//三角形衝突判定用の三角形の配列を作成
		//こちらはステージ単位
		size_t i = 0;
		while (i < indices.size()) {
			TRIANGLE tr;
			tr.m_A = vertices[indices[i]].position;
			tr.m_B = vertices[indices[i + 1]].position;
			tr.m_C = vertices[indices[i + 2]].position;
			m_Triangles.push_back(tr);
			i += 3;
		}

		//自分自身をシェアオブジェクトに登録
		GetStage()->SetSharedGameObject(L"UnevenGroundData", GetThis<UnevenGroundData>());

	}



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	用途: でこぼこ床
	//--------------------------------------------------------------------------------------
	UnevenGround::UnevenGround(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	UnevenGround::~UnevenGround() {}
	//初期化
	void UnevenGround::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		auto DataPtr = GetStage()->GetSharedGameObject<UnevenGroundData>(L"UnevenGroundData");
		//三角形コリジョン
		auto PtrTriangle = AddComponent<CollisionTriangles>();
		PtrTriangle->SetMakedTriangles(DataPtr->GetTriangles());
		PtrTriangle->SetWireFrameMesh(L"UnevenGroundWireMesh");
		PtrTriangle->SetDrawActive(true);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"UnevenGroundMesh");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"WALL_TX");

	}



}
//end basecross
