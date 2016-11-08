/*!
@file Enemy.cpp
@brief 敵など実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class HeadEnemy : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	HeadEnemy::HeadEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(1.0f)
	{
	}
	HeadEnemy::~HeadEnemy() {}

	//初期化
	void HeadEnemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.25f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.125f);
		//Rigidbodyをつける
		auto PtrRigid = AddComponent<Rigidbody>();
		//横部分のみ反発
		//反発係数は0.5（半分）
		PtrRigid->SetReflection(0.5f);
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Seekは無効にしておく
		PtrSeek->SetUpdateActive(false);
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//SPの衝突判定をつける
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		PtrColl->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//透明処理をする
		SetAlphaActive(true);
		m_BehaviorMachine = ObjectFactory::Create<BehaviorMachine<HeadEnemy>>(GetThis<HeadEnemy>());
		m_BehaviorMachine->AddBehavior<HeadStop>(L"Stop");
		m_BehaviorMachine->AddBehavior<HeadJumpSeek>(L"JumpSeek");
		m_BehaviorMachine->Reset(L"Stop");
	}

	//ユーティリティ関数群
	Vector3 HeadEnemy::GetPlayerPosition() const {
		//もしプレイヤーが初期化化されてない場合には、Vector3(0,m_BaseY,0)を返す
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer) {
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		}
		return PlayerPos;
	}
	float HeadEnemy::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}

	//操作
	void HeadEnemy::OnUpdate() {
		m_BehaviorMachine->Update();
	}

	void HeadEnemy::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
	}

	//回転を進行方向に向かせる
	void HeadEnemy::RotToFront() {
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


	void HeadEnemy::OnLastUpdate() {
		m_BehaviorMachine->Update2();
	}

	//--------------------------------------------------------------------------------------
	//	静止行動
	//--------------------------------------------------------------------------------------
	void HeadStop::Enter(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrRigidbody = Obj->GetComponent<Rigidbody>();
		PtrRigidbody->SetVelocity(0, 0, 0);
	}
	void HeadStop::Execute(const shared_ptr<HeadEnemy>& Obj) {
		if (Obj->GetPlayerLength() < 3.0f) {
			Obj->GetBehaviorMachine()->Push(L"JumpSeek");
		}
	}

	void HeadStop::WakeUp(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrRigidbody = Obj->GetComponent<Rigidbody>();
		PtrRigidbody->SetVelocity(0, 0, 0);
	}

	void HeadStop::Exit(const shared_ptr<HeadEnemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	ジャンプして追跡行動
	//--------------------------------------------------------------------------------------
	void HeadJumpSeek::Enter(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetDecl(2.0f);
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 2.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void HeadJumpSeek::Execute(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Obj->GetPlayerPosition());
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			if (Obj->GetPlayerLength() >= 3.0f) {
				Obj->GetBehaviorMachine()->Pop();
			}
			else if (Obj->GetPlayerLength() >= 2.0f) {
				//ジャンプスタート
				Vector3 JumpVec(0.0f, 2.0f, 0);
				PtrGravity->StartJump(JumpVec);
			}
		}
	}

	void HeadJumpSeek::Execute2(const shared_ptr<HeadEnemy>& Obj) {
		Obj->RotToFront();
	}

	void HeadJumpSeek::Exit(const shared_ptr<HeadEnemy>& Obj) {
		auto PtrArrive = Obj->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
		auto PtrTrans = Obj->GetComponent<Transform>();
		auto Pos = PtrTrans->GetPosition();
		Pos.y = 0.125f;
		PtrTrans->SetPosition(Pos);
	}

	//--------------------------------------------------------------------------------------
	//	もぞもぞした敵
	//--------------------------------------------------------------------------------------

	//ユーティリティ関数群
	Vector3 MozoEnemy::GetPlayerPosition() const {
		//もしプレイヤーが初期化化されてない場合には、Vector3(0,m_BaseY,0)を返す
		Vector3 PlayerPos;
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		return PlayerPos;
	}
	float MozoEnemy::GetPlayerLength() const {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}


	//構築と破棄
	MozoEnemy::MozoEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y)
	{
	}
	MozoEnemy::~MozoEnemy() {}

	void MozoEnemy::CreateMeshRes() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"MOZO_SPHERE")) {
			App::GetApp()->RegisterResource(L"MOZO_SPHERE", MeshResource::CreateSphere(1.0f, 6));
		}
	}


	//初期化
	void MozoEnemy::OnCreate() {
		CreateMeshRes();
		m_MyLight.SetPositionToDirectional(-0.00001f, 1.0f, -0.00001f);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.05f);
		//Rigidbodyをつける
		auto PtrRigid = AddComponent<Rigidbody>();
		//横部分のみ反発
		//反発係数は0.5（半分）
		PtrRigid->SetReflection(1.0f);
		//SPの衝突判定をつける
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"MOZO_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"MOZO_SPHERE");
		PtrDraw->SetTextureResource(L"MOZO_TX");


		//透明処理をする
		SetAlphaActive(true);
		m_BehaviorMachine = ObjectFactory::Create<BehaviorMachine<MozoEnemy>>(GetThis<MozoEnemy>());
		m_BehaviorMachine->AddBehavior<MozoStart>(L"Start");
		m_BehaviorMachine->AddBehavior<MozoDefault>(L"Default");
		m_BehaviorMachine->Reset(L"Start");

	}

	const Light& MozoEnemy::OnGetDrawLight()const{
		return m_MyLight;
	}



	//操作
	void MozoEnemy::OnUpdate() {
		m_BehaviorMachine->Update();
	}

	void MozoEnemy::OnLastUpdate() {
		m_BehaviorMachine->Update2();
	}


	//--------------------------------------------------------------------------------------
	//	スタート行動
	//--------------------------------------------------------------------------------------
	void MozoStart::Enter(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrTransform = Obj->GetComponent<Transform>();
		PtrTransform->SetPosition(Obj->GetStartPosition());
		PtrTransform->SetScale(0.1f, 0.1f, 0.1f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetToBefore();
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(0, 0, 0);

		auto PtrGrav = Obj->GetComponent<Gravity>();
		PtrGrav->SetUpdateActive(false);

	}
	void MozoStart::Execute(const shared_ptr<MozoEnemy>& Obj) {
		auto PlayerPos = Obj->GetPlayerPosition();
		auto Pos = Obj->GetComponent<Transform>()->GetPosition();
		Vector2 PlayerVec2(PlayerPos.x, PlayerPos.z);
		Vector2 Vec2(Pos.x, Pos.z);
		if (Vector2EX::Length(PlayerVec2 - Vec2) <= 0.5f) {
			auto PtrGrav = Obj->GetComponent<Gravity>();
			PtrGrav->SetUpdateActive(true);
		}
	}
	void MozoStart::Execute2(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrGrav = Obj->GetComponent<Gravity>();
		if (PtrGrav->IsUpdateActive()) {
			auto PtrCol = Obj->GetComponent<CollisionSphere>();
			for (auto& v : PtrCol->GetHitObjectVec()) {
				Obj->GetBehaviorMachine()->Push(L"Default");
				return;
			}
		}
	}

	void MozoStart::Exit(const shared_ptr<MozoEnemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	デフォルト行動
	//--------------------------------------------------------------------------------------
	void MozoDefault::Enter(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		float x = (Util::RandZeroToOne() - 0.5f);
		if (x < 0.0f) {
			x -= 0.5f;
		}
		else {
			x += 0.5f;
		}
		x *= 1.0f;
		float z = (Util::RandZeroToOne() - 0.5f);
		if (z < 0.0f) {
			z -= 0.5f;
		}
		else {
			z += 0.5f;
		}
		z *= 1.0f;
		PtrRigid->SetVelocity(x, 0, z);

	}
	void MozoDefault::Execute(const shared_ptr<MozoEnemy>& Obj) {
		auto Pos = Obj->GetComponent<Transform>()->GetPosition();
		auto StartPos = Obj->GetStartPosition();
		Vector2 Vec2Start(StartPos.x, StartPos.z);
		Vector2 Vec2(Pos.x, Pos.z);


		if (Vector2EX::Length(Vec2Start - Vec2) >= 3.0f) {
			Obj->GetBehaviorMachine()->Reset(L"Start");
		}
	}

	void MozoDefault::Sleep(const shared_ptr<MozoEnemy>& Obj) {
	}

	void MozoDefault::Exit(const shared_ptr<MozoEnemy>& Obj) {
		auto PtrRigid = Obj->GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(0, 0, 0);
	}
	void MozoDefault::WakeUp(const shared_ptr<MozoEnemy>& Obj) {
	}



	//--------------------------------------------------------------------------------------
	//	class FixedCylinder : public GameObject;
	//	用途: 固定のシリンダー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedCylinder::FixedCylinder(const shared_ptr<Stage>& StagePtr,
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
	FixedCylinder::~FixedCylinder() {}

	//初期化
	void FixedCylinder::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetFixed(true);
		PtrColl->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"MOZO_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}



}
// end basecross
