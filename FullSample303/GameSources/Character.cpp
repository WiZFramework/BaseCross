/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	MySeekSteering行動クラス
	//--------------------------------------------------------------------------------------
	MySeekSteering::MySeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringBehavior(GameObjectPtr)
	{}
	MySeekSteering::~MySeekSteering() {}


	float  MySeekSteering::Execute(const Vec3& TargetPos) {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Vec3 Force = RigidPtr->GetForce();
		Vec3 DesiredVelocity = normalize(TargetPos - TransPtr->GetPosition()) * RigidPtr->GetMaxSpeed();
		Vec3 WorkForce = (DesiredVelocity - RigidPtr->GetVelocity()) * GetWeight();
		Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
		RigidPtr->SetForce(Force);

		auto Pos = TransPtr->GetPosition();
		return length(Pos - TargetPos);
	}

	float MySeekSteering::Execute(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		return Execute(TargetPos);
	}


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
		auto PtrTransform = AddComponent<Transform>();
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
		//Sphereの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetIsHitAction(IsHitAction::Auto);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
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
		//進行方向を向くようにする
		RotToHead();
	}

	//進行方向を向くようにする
	void SeekObject::RotToHead() {
		auto PtrTrans = GetComponent<Transform>();
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		Vec3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.length() > 0.0f) {
			Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			Quat NowQt = PtrTrans->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt = XMQuaternionSlerp(NowQt, Qt, 0.1f);
			PtrTrans->SetQuaternion(NowQt);
		}

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
		auto PtrSeek = Obj->GetBehavior<MySeekSteering>();
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



}
//end basecross
