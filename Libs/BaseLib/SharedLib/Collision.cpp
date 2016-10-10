/*!
@file Collision.cpp
@brief 衝突判定コンポーネント実体
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Collision::Impl {
		bool m_Fixed;		//静止オブジェクトかどうか
		bool m_SendEventActive;	//衝突したときにSendイベントを発生するかどうか
		bool m_PostEventActive;	//衝突したときにPostイベントを発生するかどうか
		float m_PostDispatchTime;		//Postを発行する場合の遅延時間
		wstring m_EventString;	//衝突したときのイベントキー

		weak_ptr<GameObject> m_SubDispatchObject;	//衝突した事を知らせるサブオブジェクト
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//判定から除外するグループ

		IsHitAction m_IsHitAction;	//衝突した時の動作定義
		bool m_IsOnObject;		//乗っているオブジェクトがあるかどうか

		vector<shared_ptr<GameObject>> m_HitObjectVec;	//ヒットしたオブジェクト
		vector<shared_ptr<Collision>> m_OnObjectVec;	//乗っているオブジェクト

		Impl() :
			m_Fixed(false),
			m_SendEventActive(false),
			m_PostEventActive(false),
			m_PostDispatchTime(0),
			m_EventString(L"CollisionEvent"),
			m_IsHitAction(IsHitAction::AutoOnObjectRepel),
			m_IsOnObject(false)
		{
		}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	用途: 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Collision::Collision(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Collision::~Collision() {}

	bool Collision::GetFixed() const {
		return pImpl->m_Fixed;
	}
	bool Collision::IsFixed() const {
		return pImpl->m_Fixed;
	}
	void Collision::SetFixed(bool b) {
		pImpl->m_Fixed = b;
	}

	//操作
	void Collision::OnUpdate() {
		//Collisionが有効かどうか
		if (!IsUpdateActive()) {
			return;
		}
		//Fixedかどうか
		if (IsFixed()) {
			return;
		}
		auto ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();

		for (auto ObjPtr : ObjVec) {
			if ((GetGameObject() != ObjPtr) && ObjPtr->IsUpdateActive()) {
				//相手のCollisionを取得
				auto DestCollisionPtr = ObjPtr->GetComponent<Collision>(false);
				if (DestCollisionPtr) {
					if (!DestCollisionPtr->IsUpdateActive()) {
						//相手のCollisionが無効
						continue;
					}
					//乗ってるオブジェクトを書き出す
					AddOnObjectTest(DestCollisionPtr);
				}
			}
		}



		for (auto ObjPtr : ObjVec) {
			if ((GetGameObject() != ObjPtr) && ObjPtr->IsUpdateActive()) {
				//相手のCollisionを取得
				auto DestCollisionPtr = ObjPtr->GetComponent<Collision>(false);
				if (DestCollisionPtr) {
					if (!DestCollisionPtr->IsUpdateActive()) {
						//相手のCollisionが無効
						continue;
					}
					if (!ChkOnObjectTest(DestCollisionPtr)) {
						//相手に乗ってなければ衝突判定
						CollisionTestBase(DestCollisionPtr);
					}
				}
			}
		}
	}

	void Collision::AddHitObject(const shared_ptr<GameObject>& DestObject) {
		pImpl->m_HitObjectVec.push_back(DestObject);
	}
	void Collision::ClearHitObject() {
		pImpl->m_HitObjectVec.clear();
		pImpl->m_OnObjectVec.clear();
		SetOnObject(false);
	}
	vector<shared_ptr<GameObject>>& Collision::GetHitObjectVec() {
		return pImpl->m_HitObjectVec;
	}

	IsHitAction Collision::GetIsHitAction() const {
		return pImpl->m_IsHitAction;

	}
	void Collision::SetIsHitAction(IsHitAction HitAction) {
		pImpl->m_IsHitAction = HitAction;
	}


	bool Collision::IsOnObject() {
		return pImpl->m_IsOnObject;
	}

	void Collision::SetOnObject(bool b) {
		pImpl->m_IsOnObject = b;
	}


	void Collision::SetDestRotGravityBase(const shared_ptr<Collision>& DestColl) {
		auto DestCollisionSpherePtr = dynamic_pointer_cast<CollisionSphere>(DestColl);
		auto DestCollisionObbPtr = dynamic_pointer_cast<CollisionObb>(DestColl);
		if (DestCollisionSpherePtr) {
			SetDestRotGravity(DestCollisionSpherePtr);
		}
		else if (DestCollisionObbPtr) {
			SetDestRotGravity(DestCollisionObbPtr);
		}
	}


	bool Collision::AddOnObjectTest(const shared_ptr<Collision>& DestColl) {
		auto DestCollisionSpherePtr = dynamic_pointer_cast<CollisionSphere>(DestColl);
		auto DestCollisionObbPtr = dynamic_pointer_cast<CollisionObb>(DestColl);
		if (DestCollisionSpherePtr) {
			if (OnObjectTest(DestCollisionSpherePtr)) {
				pImpl->m_OnObjectVec.push_back(DestColl);
			}
		}
		else if (DestCollisionObbPtr) {
			if (OnObjectTest(DestCollisionObbPtr)) {
				pImpl->m_OnObjectVec.push_back(DestColl);
			}
		}
		return false;

	}

	bool Collision::ChkOnObjectTest(const shared_ptr<Collision>& DestColl) {
		for (auto& v : pImpl->m_OnObjectVec) {
			if (DestColl == v) {
				return true;
			}
		}
		return false;
	}



	bool Collision::OnObjectTestBase(const shared_ptr<Collision>& DestColl) {
		auto DestCollisionSpherePtr = dynamic_pointer_cast<CollisionSphere>(DestColl);
		auto DestCollisionObbPtr = dynamic_pointer_cast<CollisionObb>(DestColl);
		if (DestCollisionSpherePtr) {
			return OnObjectTest(DestCollisionSpherePtr);
		}
		else if (DestCollisionObbPtr) {
			return OnObjectTest(DestCollisionObbPtr);
		}
		return false;
	}




	void Collision::CollisionTestBase(const shared_ptr<Collision>& DestColl) {

		auto DestCollisionSpherePtr = dynamic_pointer_cast<CollisionSphere>(DestColl);
		auto DestCollisionObbPtr = dynamic_pointer_cast<CollisionObb>(DestColl);


		if (DestCollisionSpherePtr) {
			CollisionTest(DestCollisionSpherePtr);
		}
		else if (DestCollisionObbPtr) {
			CollisionTest(DestCollisionObbPtr);
		}
	}




	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl {
		float m_MakedDiameter;					//作成時の直径
		Impl() :
			m_MakedDiameter(1.0f)
		{}
		~Impl() {}

	};


	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionSphere::CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionSphere::~CollisionSphere() {}

	void CollisionSphere::OnCreate() {
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionSphere::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionSphere::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionSphere::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionSphere::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	SPHERE CollisionSphere::GetSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= TransPtr->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.PosInMatrixSt(), MatBase.ScaleInMatrix().x * 0.5f);
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.PosInMatrixSt(), MatBase.ScaleInMatrix().x * 0.5f);
		return Ret;
	}


	void CollisionSphere::SetDestRotGravity(const shared_ptr<CollisionSphere>& DestColl) {
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (PtrGravity) {
			//オブジェクトの上に乗った
			SPHERE  SrcSp = GetSphere();
			SPHERE  DestSp = DestColl->GetSphere();
			//自分と相手の中心同士の法線を求める
			Vector3 RotDevY = SrcSp.m_Center - DestSp.m_Center;
			RotDevY.Normalize();
			Vector3 RotDev;
			if (RotDevY.x == 0 && RotDevY.z == 0) {
				RotDev = Vector3(0, 0, 0);
			}
			else {
				RotDevY *= (PtrGravity->GetDefaultGravity().Length() * RotDevY.y);
				RotDev = PtrGravity->GetDefaultGravity() + RotDevY;
			}
			PtrGravity->SetGravity(RotDev);
			PtrGravity->SetGravityVelocityZero();
		}
	}

	void CollisionSphere::SetDestRotGravity(const shared_ptr<CollisionObb>& DestColl) {
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (PtrGravity) {
			//オブジェクトの上に乗った
			OBB obb = DestColl->GetObb();

			Vector3 RotDevY = obb.m_Rot[1];
			Vector3 RotDev;
			if (RotDevY.x == 0 && RotDevY.z == 0) {
				RotDev = Vector3(0, 0, 0);
			}
			else {
				RotDevY *= (PtrGravity->GetDefaultGravity().Length() * RotDevY.y);
				RotDev = PtrGravity->GetDefaultGravity() + RotDevY;
			}
			PtrGravity->SetGravity(RotDev);
			PtrGravity->SetGravityVelocityZero();

		}
	}



	bool CollisionSphere::OnObjectTest(const shared_ptr<CollisionSphere>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		SPHERE  DestSphere = DestColl->GetSphere();
		auto IsHit = HitTest::SPHERE_SPHERE(SrcSphere, DestSphere);
		if (IsHit) {
			return false;
		}
		auto Len = Vector3EX::Length(SrcSphere.m_Center - DestSphere.m_Center);
		auto MaxLen = (SrcSphere.m_Radius + DestSphere.m_Radius) * 1.1f;
		auto YSpan = SrcSphere.m_Center.y - DestSphere.m_Center.y;
		if (YSpan >= 0.0f && Len <= MaxLen) {
			//乗っている
			SetOnObject(true);
			return true;
		}
		else {
			//乗ってない
			return false;
		}
	}

	bool CollisionSphere::OnObjectTest(const shared_ptr<CollisionObb>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = DestColl->GetObb();
		Vector3 Ret;
		auto IsHit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
		if (IsHit) {
			return false;
		}
		Vector3 BottomPoint(0, 0, 0);
		BottomPoint.y -= SrcSphere.m_Radius;
		BottomPoint.Transform(DestObb.GetRotMatrix());
		BottomPoint += SrcSphere.m_Center;
		float CenterToRetLen = Vector3EX::Length(SrcSphere.m_Center - Ret);
		if (Vector3EX::Length(Ret - BottomPoint) < (SrcSphere.m_Radius * 0.2f)) {
			//乗っている
			SetOnObject(true);
			return true;
		}
		else {
			//乗ってない
			return false;
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		SrcVelocity /= ElapsedTime;

		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		DestVelocity /= ElapsedTime;

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestSphere, 0, ElapsedTime, HitTime)) {
			//もしFixでなければ衝突前まで戻る
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(HitTime);
				AfterCollision(DestColl, HitTime);
			}
			//もし相手がFixでなければ相手側も衝突前まで戻る
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(HitTime);
				DestColl->AfterCollision(GetThis<CollisionSphere>(), HitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}

	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		SrcVelocity /= ElapsedTime;
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;
		DestVelocity /= ElapsedTime;

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)) {
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(HitTime);
				AfterCollision(DestColl, HitTime);

			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(HitTime);
				DestColl->AfterCollision(GetThis<CollisionSphere>(), HitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}


	void CollisionSphere::BackToBefore(float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (SpanTime <= 0) {
			SpanTime = 0;
		}
		if (SpanTime >= ElapsedTime) {
			SpanTime = ElapsedTime;
		}
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = Lerp::CalculateLerp(SrcBeforSphere.m_Center, SrcSphere.m_Center, 0, ElapsedTime, SpanTime, Lerp::Linear);
		PtrTransform->SetPosition(Pos);
	}

	void CollisionSphere::AfterCollision(const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		//衝突後の残った時間で、重力で消された分を移動
		auto LastTime = ElapsedTime - SpanTime;
		bool OnObj = false;
		if (PtrGravity) {
			OnObj = OnObjectTest(DestColl);
			if (OnObj) {
				SetOnObject(OnObj);
				SetDestRotGravity(DestColl);
				PtrGravity->SetGravityVelocityZero();
			}
			else {
				PtrGravity->SetGravityDefault();
				PtrGravity->SetGravityVelocityZero();
			}
		}
		auto PtrRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigidbody) {
			if (OnObj) {
				switch (GetIsHitAction()) {
				case IsHitAction::AutoOnObjectRepel:
					PtrRigidbody->UpdateFromTime(LastTime);
					break;
				case IsHitAction::Stop:
					break;
				}
			}
			else {
				PtrRigidbody->IsHitChangeVelocity(GetThis<CollisionSphere>(), DestColl);
				PtrRigidbody->UpdateFromTime(LastTime);
			}
		}
	}


	void CollisionSphere::AfterCollision(const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		//衝突後の残った時間で、重力で消された分を移動
		auto LastTime = ElapsedTime - SpanTime;
		bool OnObj = false;
		if (PtrGravity) {
			OnObj = OnObjectTest(DestColl);
			if (OnObj) {
				SetOnObject(OnObj);
				SetDestRotGravity(DestColl);
				PtrGravity->SetGravityVelocityZero();
			}
			else {
				PtrGravity->SetGravityDefault();
				PtrGravity->SetGravityVelocityZero();
			}
		}
		auto PtrRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigidbody) {
			if (OnObj) {
				switch (GetIsHitAction()) {
				case IsHitAction::AutoOnObjectRepel:
					PtrRigidbody->UpdateFromTime(LastTime);
					break;
				case IsHitAction::Stop:
					break;
				}
			}
			else {
				PtrRigidbody->IsHitChangeVelocity(GetThis<CollisionSphere>(), DestColl);
				PtrRigidbody->UpdateFromTime(LastTime);
			}
		}

	}



	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		SPHERE DestSphere = DestColl->GetSphere();
		if (HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
			Vector3 Normal = SrcSphere.m_Center - DestSphere.m_Center;
			Normal.Normalize();
			float Span = SrcSphere.m_Radius + DestSphere.m_Radius;
			Normal *= Span;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			Vector3 Pos = DestSphere.m_Center + Normal;
			PtrTransform->SetPosition(Pos);
		}
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = DestColl->GetObb();
		Vector3 Ret;
		bool Ishit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
		if (Ishit) {
			Vector3 Normal = SrcSphere.m_Center - Ret;
			Normal.Normalize();
			Normal *= (SrcSphere.m_Radius * 1.05f);
			Vector3 NewPos = Ret + Normal;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(NewPos);
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct CollisionObb::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionObb::Impl {
		float m_Size;					//作成時のサイズ
		float m_ChkOnUnderLaySize;
		Impl() :
			m_Size(1.0f),
			m_ChkOnUnderLaySize(0.1f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionObb::CollisionObb(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionObb::~CollisionObb() {}

	//初期化
	void CollisionObb::OnCreate() {
		SetDrawActive(false);
	}


	//アクセサ
	float CollisionObb::GetMakedSize() const {
		return pImpl->m_Size;
	}
	void CollisionObb::SetMakedSize(float f) {
		pImpl->m_Size = f;
	}

	OBB CollisionObb::GetObb() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}
	OBB CollisionObb::GetBeforeObb() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetBeforeWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}

	void CollisionObb::SetDestRotGravity(const shared_ptr<CollisionSphere>& DestColl) {
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (PtrGravity) {
			//オブジェクトの上に乗った
			OBB  SrcObb = GetObb();
			SPHERE  DestSp = DestColl->GetSphere();
			//自分と相手との最近接点を求める
			Vector3 Ret;
			HitTest::SPHERE_OBB(DestSp, SrcObb, Ret);
			//最近接点と相手の中心同士の法線を求める
			Vector3 RotDevY = Ret - DestSp.m_Center;
			RotDevY.Normalize();
			Vector3 RotDev;
			if (RotDevY.x == 0 && RotDevY.z == 0) {
				RotDev = Vector3(0, 0, 0);
			}
			else {
				RotDevY *= (PtrGravity->GetDefaultGravity().Length() * RotDevY.y);
				RotDev = PtrGravity->GetDefaultGravity() + RotDevY;
			}
			PtrGravity->SetGravity(RotDev);
			PtrGravity->SetGravityVelocityZero();
		}
	}
	void CollisionObb::SetDestRotGravity(const shared_ptr<CollisionObb>& DestColl) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (PtrGravity) {
			//オブジェクトの上に乗った
			OBB obb = DestColl->GetObb();
			Vector3 RotDevY = obb.m_Rot[1];
			Vector3 RotDev;
			if (RotDevY.x == 0 && RotDevY.z == 0) {
				RotDev = Vector3(0, 0, 0);
			}
			else {
				RotDevY *= (PtrGravity->GetDefaultGravity().Length() * RotDevY.y);
				RotDev = PtrGravity->GetDefaultGravity() + RotDevY;
			}
			PtrGravity->SetGravity(RotDev);
			PtrGravity->SetGravityVelocityZero();
		}
	}


	bool CollisionObb::OnObjectTest(const shared_ptr<CollisionSphere>& DestColl) {
		OBB  SrcObb = GetObb();
		SPHERE  DestSphere = DestColl->GetSphere();
		//自分と相手との最近接点を求める
		Vector3 Ret;
		auto IsHit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
		if (IsHit) {
			return false;
		}
		auto Len = Vector3EX::Length(Ret - DestSphere.m_Center);
		auto MaxLen = DestSphere.m_Radius * 1.1f;
		auto YSpan = (SrcObb.m_Center.y - SrcObb.m_Size.y) - DestSphere.m_Center.y;
		if (YSpan >= 0.0f && Len <= MaxLen) {
			//乗っている
			SetOnObject(true);
			return true;
		}
		else {
			//乗ってない
			return false;
		}
	}

	bool CollisionObb::OnObjectTest(const shared_ptr<CollisionObb>& DestColl) {
		//自分はOBB
		OBB SrcObb = GetObb();
		OBB DestObb = DestColl->GetObb();
		if (HitTest::OBB_OBB(SrcObb, DestObb)) {
			return false;
		}

		Vector3 ChkVec;
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (PtrGravity) {
			//SrcObbから底辺の４点を求める
			//中心からの相対距離で作成する
			vector<Vector3> BottomPoints = {
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
			};
			for (auto& p : BottomPoints) {
				Vector3 StartPoint = p + Vector3(0, 0.1f, 0);
				StartPoint.Transform(DestObb.GetRotMatrix());
				StartPoint += SrcObb.m_Center;
				////
				Vector3 EndPoint = p + Vector3(0, -pImpl->m_ChkOnUnderLaySize, 0);
				EndPoint.Transform(DestObb.GetRotMatrix());
				EndPoint += SrcObb.m_Center;
				//上に乗ってるかどうかを検証
				//レイを打ち込んでみる
				if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)) {
					return true;
				}
			}
		}
		return false;
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforeObb.m_Center;
		SrcVelocity /= ElapsedTime;
		//相手
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		DestVelocity /= ElapsedTime;
		//SPHEREとOBBの扱いが逆になる
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)) {
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(HitTime);
				AfterCollision(DestColl, HitTime);

			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(HitTime);
				DestColl->AfterCollision(GetThis<CollisionObb>(), HitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		SrcVelocity /= ElapsedTime;
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;
		DestVelocity /= ElapsedTime;

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)) {
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(HitTime);
				AfterCollision(DestColl, HitTime);

			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(HitTime);
				DestColl->AfterCollision(GetThis<CollisionObb>(), HitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}



	void CollisionObb::BackToBefore(float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (SpanTime <= 0) {
			SpanTime = 0;
		}
		if (SpanTime >= ElapsedTime) {
			SpanTime = ElapsedTime;
		}
		//すべて移動以外変化なしとする
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = Lerp::CalculateLerp(SrcBeforObb.m_Center, SrcObb.m_Center, 0, ElapsedTime, SpanTime, Lerp::Linear);
		PtrTransform->SetPosition(Pos);

	}
	void CollisionObb::AfterCollision(const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		//衝突後の残った時間で、重力で消された分を移動
		auto LastTime = ElapsedTime - SpanTime;
		bool OnObj = false;
		if (PtrGravity) {
			OnObj = OnObjectTest(DestColl);
			if (OnObj) {
				SetOnObject(true);
				SetDestRotGravity(DestColl);
				PtrGravity->SetGravityVelocityZero();
			}
			else {
				PtrGravity->SetGravityDefault();
				PtrGravity->SetGravityVelocityZero();
			}
		}
		auto PtrRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigidbody) {
			if (OnObj) {
				switch (GetIsHitAction()) {
				case IsHitAction::AutoOnObjectRepel:
					PtrRigidbody->UpdateFromTime(LastTime);
					break;
				case IsHitAction::Stop:
					break;
				}
			}
			else {
				PtrRigidbody->IsHitChangeVelocity(GetThis<CollisionObb>(), DestColl);
				PtrRigidbody->UpdateFromTime(LastTime);
			}
		}
	}

	void CollisionObb::AfterCollision(const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>(false);
		//衝突後の残った時間で、重力で消された分を移動
		auto LastTime = ElapsedTime - SpanTime;
		bool OnObj = false;
		if (PtrGravity) {
			OnObj = OnObjectTest(DestColl);
			if (OnObj) {
				SetOnObject(true);
				SetDestRotGravity(DestColl);
				PtrGravity->SetGravityVelocityZero();
			}
			else {
				PtrGravity->SetGravityDefault();
				PtrGravity->SetGravityVelocityZero();
			}
		}
		auto PtrRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigidbody) {
			if (OnObj) {
				switch (GetIsHitAction()) {
				case IsHitAction::AutoOnObjectRepel:
					PtrRigidbody->UpdateFromTime(LastTime);
					break;
				case IsHitAction::Stop:
					break;
				}
			}
			else {
				PtrRigidbody->IsHitChangeVelocity(GetThis<CollisionObb>(), DestColl);
				PtrRigidbody->UpdateFromTime(LastTime);
			}
		}
	}


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		OBB SrcObb = GetObb();
		SPHERE DestSphere = DestColl->GetSphere();
		Vector3 Ret;
		bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
		if (Ishit) {
			Vector3 NowSpan = Ret - DestSphere.m_Center;
			Vector3 NewSpan = NowSpan;
			NewSpan.Normalize();
			NewSpan *= (DestSphere.m_Radius * 1.05f);
			auto MoveSpan = NewSpan - NowSpan;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			auto Pos = PtrTransform->GetPosition();
			Pos += MoveSpan;
			PtrTransform->SetPosition(Pos);
		}


	}

	void CollisionObb::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		OBB SrcObb = GetObb();
		OBB DestObb = DestColl->GetObb();
		bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
		if (Ishit) {
			Vector3 Ret;
			//SrcのOBBとDestの最近接点を得る
			HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, Ret);
			Vector3 Normal = SrcObb.m_Center - Ret;
			Normal.Normalize();
			//Normalは退避方向
			Normal *= 0.01f;
			SrcObb.m_Center += Normal;
			UINT count = 0;
			while (HitTest::OBB_OBB(SrcObb, DestObb)) {
				SrcObb.m_Center += Normal;
				count++;
				if (count >= 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(SrcObb.m_Center);
		}


	}




}
//end basecross

