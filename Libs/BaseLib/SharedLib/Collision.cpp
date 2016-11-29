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

		vector<shared_ptr<GameObject>> m_BeforHitObjectVec;	//1つ前にヒットしたオブジェクト
		vector<shared_ptr<GameObject>> m_HitObjectVec;	//ヒットしたオブジェクト
		vector<shared_ptr<GameObject>> m_TempHitObjectVec;	//汎用ヒットしたオブジェクト



		Impl() :
			m_Fixed(false),
			m_SendEventActive(false),
			m_PostEventActive(false),
			m_PostDispatchTime(0),
			m_EventString(L"CollisionEvent"),
			m_IsHitAction(IsHitAction::AutoOnObjectRepel)
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
		auto& ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();
		for (auto& ObjPtr : ObjVec) {
			if ((GetGameObject() != ObjPtr) && ObjPtr->IsUpdateActive()) {
				//すでに同じ相手と衝突してるかどうか
				if (pImpl->m_HitObjectVec.size() > 0) {
					bool chk = false;
					for (auto& v : pImpl->m_HitObjectVec) {
						if (v == ObjPtr) {
							chk = true;
							break;
						}
					}
					if (chk) {
						continue;
					}
				}
				//相手のCollisionを取得
				auto DestCollisionPtr = ObjPtr->GetComponent<Collision>(false);
				if (DestCollisionPtr) {
					if (!DestCollisionPtr->IsUpdateActive()) {
						//相手のCollisionが無効
						continue;
					}
					//衝突判定
					CollisionTestBase(DestCollisionPtr);
				}
			}
		}

	}

	void Collision::AddHitObject(const shared_ptr<GameObject>& DestObject) {
		pImpl->m_HitObjectVec.push_back(DestObject);
	}

	void Collision::SetToBeforHitObject() {
		pImpl->m_BeforHitObjectVec.resize(pImpl->m_HitObjectVec.size());
		pImpl->m_BeforHitObjectVec = pImpl->m_HitObjectVec;
	}

	void Collision::ClearHitObject() {
		pImpl->m_HitObjectVec.clear();
		pImpl->m_TempHitObjectVec.clear();
	}
	vector<shared_ptr<GameObject>>& Collision::GetHitObjectVec() {
		return pImpl->m_HitObjectVec;
	}

	void Collision::FindIfCollisionVector(
		shared_ptr<GameObject> Tgt,
		vector<shared_ptr<GameObject>>& SetVec,
		const vector<shared_ptr<GameObject>>& ScanVec,
		bool IsFind
	) {
		if (IsFind) {
			for (auto& v : ScanVec) {
				if (Tgt == v) {
					//見つかったらセットしてリターン
					SetVec.push_back(Tgt);
					return;
				}
			}
		}
		else {
			for (auto& v : ScanVec) {
				if (Tgt == v) {
					//見つかったらリターン
					return;
				}
			}
			//見つからなかったらセット
			SetVec.push_back(Tgt);
		}
	}



	vector<shared_ptr<GameObject>>& Collision::GetNewHitObjectVec() {
		pImpl->m_TempHitObjectVec.clear();
		for (auto v : pImpl->m_HitObjectVec) {
			FindIfCollisionVector(
				v,
				pImpl->m_TempHitObjectVec,
				pImpl->m_BeforHitObjectVec,
				false
			);
		}
		return pImpl->m_TempHitObjectVec;
	}
	vector<shared_ptr<GameObject>>& Collision::GetExcuteHitObjectVec() {
		pImpl->m_TempHitObjectVec.clear();
		for (auto v : pImpl->m_HitObjectVec) {
			FindIfCollisionVector(
				v,
				pImpl->m_TempHitObjectVec,
				pImpl->m_BeforHitObjectVec,
				true
			);
		}
		return pImpl->m_TempHitObjectVec;
	}
	vector<shared_ptr<GameObject>>& Collision::GetExitHitObjectVec() {
		pImpl->m_TempHitObjectVec.clear();
		for (auto v : pImpl->m_BeforHitObjectVec) {
			FindIfCollisionVector(
				v,
				pImpl->m_TempHitObjectVec,
				pImpl->m_HitObjectVec,
				false
			);
		}
		return pImpl->m_TempHitObjectVec;
	}



	IsHitAction Collision::GetIsHitAction() const {
		return pImpl->m_IsHitAction;

	}
	void Collision::SetIsHitAction(IsHitAction HitAction) {
		pImpl->m_IsHitAction = HitAction;
	}

	void Collision::CollisionTestBase(const shared_ptr<Collision>& DestColl) {
		auto DestCollisionSpherePtr = dynamic_pointer_cast<CollisionSphere>(DestColl);
		auto DestCollisionObbPtr = dynamic_pointer_cast<CollisionObb>(DestColl);
		auto DestCollisionRectPtr = dynamic_pointer_cast<CollisionRect>(DestColl);
		if (DestCollisionSpherePtr) {
			CollisionTest(DestCollisionSpherePtr);
		}
		else if (DestCollisionObbPtr) {
			CollisionTest(DestCollisionObbPtr);
		}
		else if (DestCollisionRectPtr) {
			CollisionTest(DestCollisionRectPtr);
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
	void CollisionSphere::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestSphere, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突前まで戻る
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//もし相手がFixでなければ相手側も衝突前まで戻る
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(DestVelocity, HitTime);
				DestColl->AfterCollision(DestVelocity,GetThis<CollisionSphere>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}

	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(DestVelocity,HitTime);
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionSphere>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereRect(SrcBeforSphere, SpanVelocity, DestRect, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//相手は必ずFIXである。
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			CollisionEscape(DestColl);
		}
	}



	void CollisionSphere::BackToBefore(const Vector3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = SrcBeforSphere.m_Center + TotalVelocoty * SpanTime;
		PtrTransform->SetPosition(Pos);
	}


	void CollisionSphere::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		SPHERE sp2 = DestColl->GetSphere();
		//接点へのベクトル
		Vector3 ContactBase = sp2.m_Center - sp.m_Center;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
			}
		}
	}


	void CollisionSphere::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		OBB obb = DestColl->GetObb();
		Vector3 Ret;
		HitTest::SPHERE_OBB(sp, obb, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - sp.m_Center;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
				break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
			}
		}
	}

	void CollisionSphere::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		Vector3 Ret;
		HitTest::SPHERE_COLRECT(sp, DestRect, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - sp.m_Center;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = true;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
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
			PtrTransform->ResetPosition(Pos);
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
			PtrTransform->ResetPosition(NewPos);
		}
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		Vector3 Ret;
		bool Ishit = HitTest::HitTest::SPHERE_COLRECT(SrcSphere, DestRect, Ret);
		if (Ishit) {
			Vector3 Normal = SrcSphere.m_Center - Ret;
			Normal.Normalize();
			Normal *= (SrcSphere.m_Radius * 1.05f);
			Vector3 NewPos = Ret + Normal;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->ResetPosition(NewPos);
		}
	}


	void CollisionSphere::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SPHERE"));
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

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとOBBの扱いが逆になる
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(DestVelocity, HitTime);
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionObb>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//もし相手がFixでなければ衝相手側に突情報の登録
			if (!DestColl->IsFixed()) {
				DestColl->AddHitObject(GetGameObject());
				DestColl->BackToBefore(DestVelocity, HitTime);
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionObb>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbRect(SrcBeforObb, SpanVelocity, DestRect, 0, ElapsedTime, HitTime)) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			//もしFixでなければ衝突情報の登録
			if (!IsFixed()) {
				AddHitObject(DestColl->GetGameObject());
				BackToBefore(SrcVelocity, HitTime);
				AfterCollision(SrcVelocity, DestColl, AfterHitTime);
			}
			//相手は必ずFIXである。
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			CollisionEscape(DestColl);
		}

	}



	void CollisionObb::BackToBefore(const Vector3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = SrcBeforObb.m_Center + TotalVelocoty * SpanTime;
		PtrTransform->SetPosition(Pos);
	}

	void CollisionObb::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		OBB obb = GetObb();
		SPHERE sp = DestColl->GetSphere();
		Vector3 Ret;
		HitTest::SPHERE_OBB(sp, obb, Ret);
		//接点へのベクトル
		Vector3 ContactBase = sp.m_Center - Ret;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
			}
		}
	}

	void CollisionObb::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		OBB obb = GetObb();
		OBB obb2 = DestColl->GetObb();
		Vector3 Ret;
		//SrcのOBBとDestの最近接点を得る
		HitTest::ClosestPtPointOBB(obb.m_Center, obb2, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - obb.m_Center;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
			}
		}
	}


	void CollisionObb::AfterCollision(const Vector3 TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime){
		OBB obb = GetObb();
		COLRECT rect = DestColl->GetColRect();
		//Rectのベクトル
		Vector3 ContactBase = DestColl->GetColRect().GetPLANE().m_Normal;
	//	Vector3 ContactBase = Vector3(0,1.0,0);
//		ContactBase *= -1.0f;
		ContactBase.Normalize();
		//最近接点から直行線の長さ（内積で求める）
		float Len = Vector3EX::Dot(TotalVelocoty, ContactBase);
		Vector3 Contact = ContactBase * Len;
		//滑る方向は速度から接点へのベクトルを引き算
		Vector3 Slide = TotalVelocoty - Contact;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				PtrGrav->SetGravityVelocityZero();
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrGrav->GetGravityVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrGrav->GetGravityVelocity() - Contact;
				PtrGrav->SetGravityVelocity(Slide);
				horizontal = false;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::AutoOnObjectRepel:
			{
				if (horizontal) {
					//最近接点から直行線の長さ（内積で求める）
					Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
					Contact = ContactBase * Len;
					Slide = PtrRigid->GetVelocity() - Contact;
					PtrRigid->SetVelocity(Slide);
				}
				else {
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::Slide:
				//最近接点から直行線の長さ（内積で求める）
				Len = Vector3EX::Dot(PtrRigid->GetVelocity(), ContactBase);
				Contact = ContactBase * Len;
				Slide = PtrRigid->GetVelocity() - Contact;
				PtrRigid->SetVelocity(Slide);
				break;
			default:
				break;
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
			PtrTransform->ResetPosition(Pos);
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
			PtrTransform->ResetPosition(SrcObb.m_Center);
		}
	}

	void CollisionObb::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		OBB SrcObb = GetObb();
		COLRECT DestRect = DestColl->GetColRect();
		bool Ishit = HitTest::OBB_COLRECT(SrcObb, DestRect);
		if (Ishit) {
			Vector3 Ret;
			//SrcのOBBとDestの最近接点を得る
			Vector3 Normal = DestColl->GetColRect().GetPLANE().m_Normal;
			Normal.Normalize();
			//Normalは退避方向
			Normal *= -0.01f;
			SrcObb.m_Center += Normal;
			UINT count = 0;
			while (HitTest::OBB_COLRECT(SrcObb, DestRect)) {
				SrcObb.m_Center += Normal;
				count++;
				if (count >= 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->ResetPosition(SrcObb.m_Center);
		}
	}


	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));

	}

	//--------------------------------------------------------------------------------------
	//	struct CollisionRect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionRect::Impl {
		float m_Size;					//作成時のサイズ
		Impl() :
			m_Size(1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionRect : public Collision ;
	//	用途: Rect(矩形)衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionRect::CollisionRect(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionRect::~CollisionRect() {}

	//初期化
	void CollisionRect::OnCreate() {
		SetFixed(true),
		SetDrawActive(false);
	}

	//アクセサ
	void CollisionRect::SetFixed(bool b) {
		if (!b) {
			throw BaseException(
				L"CollisionRectはFixed以外は選択できません",
				L"if (!b)",
				L"CollisionRect::SetFixed()"
			);
		}
	}



	float CollisionRect::GetMakedSize() const {
		return pImpl->m_Size;
	}
	void CollisionRect::SetMakedSize(float f) {
		pImpl->m_Size = f;
	}

	COLRECT CollisionRect::GetColRect() const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		COLRECT rect(GetMakedSize(), GetMakedSize(), TrasnsPtr->GetWorldMatrix());
		return rect;
	}

	COLRECT CollisionRect::GetBeforeColRect() const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		COLRECT rect(GetMakedSize(), GetMakedSize(), TrasnsPtr->GetBeforeWorldMatrix());
		return rect;
	}


	void CollisionRect::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SQUARE"));

	}








}
//end basecross

