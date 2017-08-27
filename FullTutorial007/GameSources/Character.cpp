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
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject() {}

	//初期化
	void SeekObject::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//分離行動をつける
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->SetGameObjectGroup(Group);
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();
		//親の影響を受ける
		PtrCol->SetIsHitAction(IsHitAction::Auto);
		PtrCol->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}


	//操作
	void SeekObject::OnUpdate() {
		//ステートによって変わらない行動を実行
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
		auto PtrSep = GetBehavior<SeparationSteering>();
		PtrSep->Execute();
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void SeekObject::OnUpdate2() {
		auto PtrUtil = GetBehavior<UtilBehavior>();
		PtrUtil->RotToHead(0.1f);
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
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrSeek = Obj->GetBehavior<SeekSteering>();
		float f = PtrSeek->Execute(L"Player");
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
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
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto PtrArrive = Obj->GetBehavior<ArriveSteering>();
		if (PtrArrive->Execute(L"Player") >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}




	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	用途: 固定のモデル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticModel::StaticModel(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
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

		Mat4x4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.28f, 0.0f)
		);

		auto PtrColl = AddComponent<CollisionCapsule>();
		PtrColl->SetFixed(true);
	//	PtrColl->SetDrawActive(true);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"MODEL_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"MODEL_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

	}


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
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

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
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
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
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
		AddTag(L"MoveBox");

		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vec3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vec3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
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
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
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
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"WALL_TX");

	}

	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
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
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
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
			{ VertexPositionNormalTexture(Vec3(-1.0f, 0.0f, 0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(0.0f, 0.5f, 0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(0.5f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-1.0f, 0.0f,-0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vec3(0.0f, 0.0f,-0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(0.5f, 1.0f)) },
			{ VertexPositionNormalTexture(Vec3(1.0f, 0.0f, 0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(1.0f, 0.0f,-0.5f), Vec3(0.0f, 1.0f,0.0f), Vec2(1.0f, 1.0f)) },
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
				new_v.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
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
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position) :
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

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"UnevenGroundMesh");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"WALL_TX");

	}




}
//end basecross
