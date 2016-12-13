/*!
@file Collision.cpp
@brief 衝突判定コンポーネント実体
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

		vector<shared_ptr<GameObject>> m_BeforeHitObjectVec;	//1つ前にヒットしたオブジェクト
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
					//衝突判定(相手に呼んでもらう。ダブルデスパッチ呼び出し)
					DestCollisionPtr->CollisionCall(GetThis<Collision>());
				}
			}
		}

	}

	void Collision::AddHitObject(const shared_ptr<GameObject>& DestObject) {
		pImpl->m_HitObjectVec.push_back(DestObject);
	}

	void Collision::SetToBeforeHitObject() {
		pImpl->m_BeforeHitObjectVec.resize(pImpl->m_HitObjectVec.size());
		pImpl->m_BeforeHitObjectVec = pImpl->m_HitObjectVec;
	}

	void Collision::ClearHitObject() {
		pImpl->m_HitObjectVec.clear();
		pImpl->m_TempHitObjectVec.clear();
	}
	vector<shared_ptr<GameObject>>& Collision::GetHitObjectVec() {
		return pImpl->m_HitObjectVec;
	}

	vector<shared_ptr<GameObject>>& Collision::GetBeforeHitObjectVec() {
		return pImpl->m_BeforeHitObjectVec;
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
				pImpl->m_BeforeHitObjectVec,
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
				pImpl->m_BeforeHitObjectVec,
				true
			);
		}
		return pImpl->m_TempHitObjectVec;
	}
	vector<shared_ptr<GameObject>>& Collision::GetExitHitObjectVec() {
		pImpl->m_TempHitObjectVec.clear();
		for (auto v : pImpl->m_BeforeHitObjectVec) {
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

	void Collision::AfterCollisionSub(const shared_ptr<Collision>& DestColl, const Vector3& ContactBase) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//親の速度
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		auto DestVelo = PtrDestTransform->GetPosition() - PtrDestTransform->GetBeforePosition();

		//RigidbodyとGravityはそれぞれの速度の分散を設定する
		auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);

		Vector3 Slide(0, 0, 0);
		bool horizontal = false;
		if (PtrGrav) {
			auto Grav = PtrGrav->GetGravity();
			Grav.Normalize();
			if (Vector3EX::AngleBetweenNormals(Grav, ContactBase) <= 0.01f) {
				auto GravVelo = PtrGrav->GetGravityVelocity();
				float dot = DestVelo.Dot(Grav);
				if (dot > 0) {
					auto DotVec = Grav * dot;
					GravVelo -= DotVec;
					PtrGrav->SetGravityVelocity(GravVelo);
				}
				else {
					PtrGrav->SetGravityVelocityZero();
				}
				horizontal = true;
			}
			else {
				//最近接点から直行線の長さ（内積で求める）
				Slide = Vector3EX::Slide(PtrGrav->GetGravityVelocity(), ContactBase);
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
					Slide = Vector3EX::Slide(PtrRigid->GetVelocity(), ContactBase);
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
				Slide = Vector3EX::Slide(PtrRigid->GetVelocity(), ContactBase);
				PtrRigid->SetVelocity(Slide);
				break;
			case IsHitAction::AutoOnParent:
			{
				if (horizontal) {
					//乗っているときはスライドさせる
					Slide = Vector3EX::Slide(PtrRigid->GetVelocity(), ContactBase);
					PtrRigid->SetVelocity(Slide);
					//位置を親に合わせる
					auto Pos = PtrTransform->GetPosition();
					Pos += DestVelo;
					PtrTransform->SetPosition(Pos);
				}
				else {
					//乗ってないときは反発
					auto Ref = Vector3EX::Reflect(PtrRigid->GetVelocity(), ContactBase);
					//反発係数
					Ref *= PtrRigid->GetReflection();
					PtrRigid->SetVelocity(Ref);
				}
			}
			break;
			case IsHitAction::AutoOnParentSlide:
			{
				if (horizontal) {
					//乗っているときはスライドさせる
					Slide = Vector3EX::Slide(PtrRigid->GetVelocity(), ContactBase);
					PtrRigid->SetVelocity(Slide);
					//位置を親に合わせる
					auto Pos = PtrTransform->GetPosition();
					Pos += DestVelo;
					PtrTransform->SetPosition(Pos);
				}
				else {
					//乗ってないときはスライド
					Slide = Vector3EX::Slide(PtrRigid->GetVelocity(), ContactBase);
					PtrRigid->SetVelocity(Slide);
				}
			}
			break;
			default:
				break;
			}
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl {
		float m_MakedDiameter;					//作成時の直径
		//配列ボリュームと衝突時に衝突した配列を特定するインデックス
		size_t m_IsHitVolumeIndex;
		CalcScaling m_CalcScaling;
		Impl() :
			m_MakedDiameter(1.0f),
			m_IsHitVolumeIndex(0),
			m_CalcScaling(CalcScaling::XScale)
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

	CalcScaling CollisionSphere::GetCalcScaling() const {
		return pImpl->m_CalcScaling;

	}
	void CollisionSphere::SetCalcScaling(CalcScaling s) {
		pImpl->m_CalcScaling = s;
	}


	SPHERE CollisionSphere::GetSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= TransPtr->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.PosInMatrixSt(), MatBase.ScaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.ScaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.ScaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.PosInMatrixSt(), MatBase.ScaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.ScaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.ScaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}

	void CollisionSphere::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionSphere>());
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

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
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
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestCap, 0, ElapsedTime, HitTime)) {
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
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionSphere>(), AfterHitTime);
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
	void CollisionSphere::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手まずはAABBで調査
		AABB WrapAABB = DestColl->GetWrapedAABB();
		//ラッピングAABBは静止している
		Vector3 SpanVelocity = SrcVelocity;
		float HitTime = 0;
		if (!HitTest::CollisionTestSphereAabb(SrcBeforSphere, SpanVelocity, WrapAABB, 0, ElapsedTime, HitTime)) {
			//ラッピングAABBと衝突していなければこれ以上検査しない
			return;
		}
		SpanVelocity = SrcVelocity - DestVelocity;
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		for (size_t i = 0; i < trivec.size();i++) {
			HitTime = 0;
			if (HitTest::CollisionTestSphereTriangle(SrcBeforSphere, SpanVelocity, trivec[i], 0, ElapsedTime, HitTime)) {
				//衝突したインデックスを保存
				pImpl->m_IsHitVolumeIndex = i;
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
				//1つの三角形と衝突したら処理終了
				break;
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


	void CollisionSphere::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		SPHERE sp2 = DestColl->GetSphere();
		//接点へのベクトル
		Vector3 ContactBase = sp2.m_Center - sp.m_Center;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();

		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionSphere::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		CAPSULE cap = DestColl->GetCapsule();
		Vector3 Ret;
		HitTest::SPHERE_CAPSULE(sp, cap, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - sp.m_Center;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();

		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}



	void CollisionSphere::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		OBB obb = DestColl->GetObb();
		Vector3 Ret;
		HitTest::SPHERE_OBB(sp, obb, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - sp.m_Center;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();

		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionSphere::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionTriangles>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();

		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Vector3 Normal = trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
		Vector3 ContactBase = Normal;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();


		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}


	void CollisionSphere::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime) {
		SPHERE sp = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		//面のベクトル
		PLANE p = DestRect.GetPLANE();
		Vector3 ContactBase = p.m_Normal;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();


		auto Pos = sp.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
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

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		CAPSULE DestCap = DestColl->GetCapsule();
		Vector3 Ret;
		bool Ishit = HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret);
		if (Ishit) {
			Vector3 span = SrcSphere.m_Center - Ret;
			span.Normalize();
			span *= 0.02f;
			int count = 0;
			auto Center = SrcSphere.m_Center;
			while (1) {
				Center += span;
				SrcSphere.m_Center = Center;
				if (!HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}


	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = DestColl->GetObb();
		Vector3 Ret;
		bool Ishit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
		if (Ishit) {
			Vector3 span = SrcSphere.m_Center - Ret;
			span.Normalize();
			span *= 0.02f;
			int count = 0;
			auto Center = SrcSphere.m_Center;
			while (1) {
				Center += span;
				SrcSphere.m_Center = Center;
				if (!HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Vector3 Ret;
		bool Ishit = HitTest::HitTest::SPHERE_TRIANGLE(SrcSphere, trivec[pImpl->m_IsHitVolumeIndex], Ret);
		if (Ishit) {
			Vector3 span = Vector3EX::Cross(trivec[pImpl->m_IsHitVolumeIndex].m_B - trivec[pImpl->m_IsHitVolumeIndex].m_A,
				trivec[pImpl->m_IsHitVolumeIndex].m_C - trivec[pImpl->m_IsHitVolumeIndex].m_A);
			span.Normalize();
			span *= 0.02f;
			int count = 0;
			auto Center = SrcSphere.m_Center;
			while (1) {
				Center += span;
				SrcSphere.m_Center = Center;
				if (!HitTest::SPHERE_TRIANGLE(SrcSphere, trivec[pImpl->m_IsHitVolumeIndex], Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}

	}


	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		Vector3 Ret;
		bool Ishit = HitTest::HitTest::SPHERE_COLRECT(SrcSphere, DestRect, Ret);
		if (Ishit) {
			auto p = DestRect.GetPLANE();
			Vector3 span = p.m_Normal;
			span.Normalize();
			span *= -0.02f;
			int count = 0;
			auto Center = SrcSphere.m_Center;
			while (1) {
				Center += span;
				SrcSphere.m_Center = Center;
				if (!HitTest::SPHERE_COLRECT(SrcSphere, DestRect, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			//相手がRectの場合はリセット
			PtrTransform->ResetPosition(Center);
		}
	}


	void CollisionSphere::OnDraw() {
		GenericDraw Draw;
		Matrix4X4 MeshToTransformMatrix;

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Scale = PtrTransform->GetScale();
		Vector3 CollScale(Scale.x, Scale.x, Scale.x);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			CollScale = Vector3(Scale.y, Scale.y, Scale.y);
			break;
		case CalcScaling::ZScale:
			CollScale = Vector3(Scale.z, Scale.z, Scale.z);
			break;
		default:
			break;
		}
		Vector3 ColcScale(CollScale.x / Scale.x, CollScale.y / Scale.y, CollScale.z / Scale.z);
		Matrix4X4 mat;
		mat.ScalingFromVector(ColcScale);

		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SPHERE"),
			mat);
	}

	//--------------------------------------------------------------------------------------
	//	struct CollisionCapsule::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionCapsule::Impl {
		float m_MakedDiameter;			//作成時の直径
		float m_MakedHeight;			//作成時高さ
		//配列ボリュームと衝突時に衝突した配列を特定するインデックス
		size_t m_IsHitVolumeIndex;
		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f),
			m_IsHitVolumeIndex(0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionCapsule::CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionCapsule::~CollisionCapsule() {}

	//初期化
	void CollisionCapsule::OnCreate() {
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionCapsule::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionCapsule::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionCapsule::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionCapsule::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	float CollisionCapsule::GetMakedHeight() const {
		return pImpl->m_MakedHeight;
	}
	void CollisionCapsule::SetMakedHeight(float f) {
		pImpl->m_MakedHeight = f;
	}

	CAPSULE CollisionCapsule::GetCapsule() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//このオブジェクトのCAPSULEを作成
		CAPSULE Ret(
			pImpl->m_MakedDiameter * 0.5f, 
			Vector3(0, pImpl->m_MakedHeight * -0.5f, 0),
			Vector3(0, pImpl->m_MakedHeight * 0.5f, 0),
			PtrT->GetWorldMatrix());
		return Ret;
	}

	CAPSULE CollisionCapsule::GetBeforeCapsule() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//このオブジェクトのCAPSULEを作成
		CAPSULE Ret(
			pImpl->m_MakedDiameter * 0.5f,
			Vector3(0, pImpl->m_MakedHeight * -0.5f, 0),
			Vector3(0, pImpl->m_MakedHeight * 0.5f, 0),
			PtrT->GetBeforeWorldMatrix());
		return Ret;
	}

	void CollisionCapsule::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionCapsule>());
	}



	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとCAPSULEの扱いが逆になる
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcCapsule, 0, ElapsedTime, HitTime)) {
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
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionCapsule>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionCapsule
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestCapsule, 0, ElapsedTime, HitTime)) {
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
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionCapsule>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionObb
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)) {
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
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionCapsule>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手まずはAABBで調査
		AABB WrapAABB = DestColl->GetWrapedAABB();
		//ラッピングAABBは静止している
		Vector3 SpanVelocity = SrcVelocity;
		float HitTime = 0;
		if (!HitTest::CollisionTestCapsuleAabb(SrcBeforCapsule, SpanVelocity, WrapAABB, 0, ElapsedTime, HitTime)) {
			//ラッピングAABBと衝突していなければこれ以上検査しない
			return;
		}
		SpanVelocity = SrcVelocity - DestVelocity;
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		for (size_t i = 0; i < trivec.size(); i++) {
			HitTime = 0;
			if (HitTest::CollisionTestCapsuleTriangle(SrcBeforCapsule, SpanVelocity, trivec[i], 0, ElapsedTime, HitTime)) {
				//衝突したインデックスを保存
				pImpl->m_IsHitVolumeIndex = i;
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
				//1つの三角形と衝突したら処理終了
				break;
			}
		}


	}


	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		Vector3 SrcVelocity = PtrTransform->GetVelocity();
		Vector3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionRect
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleRect(SrcBeforCapsule, SpanVelocity, DestRect, 0, ElapsedTime, HitTime)) {
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
				DestColl->AfterCollision(DestVelocity, GetThis<CollisionCapsule>(), AfterHitTime);
			}
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			else {
				CollisionEscape(DestColl);
			}
		}
	}

	void CollisionCapsule::BackToBefore(const Vector3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = SrcBeforCapsule.GetCenter() + TotalVelocoty * SpanTime;
		PtrTransform->SetPosition(Pos);
	}

	void CollisionCapsule::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		CAPSULE cap = GetCapsule();
		SPHERE sp = DestColl->GetSphere();
		Vector3 RetVec;
		HitTest::SPHERE_CAPSULE(sp, cap, RetVec);
		//接点へのベクトル
		Vector3 ContactBase = sp.m_Center - RetVec;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = cap.GetCenter() + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);
		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionCapsule::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) {
		CAPSULE SrcCap = GetCapsule();
		CAPSULE DestCap = DestColl->GetCapsule();
		Vector3 RetVec1, RetVec2;
		HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, RetVec1, RetVec2);
		//接点へのベクトル
		//DestCapの線分とRetVec1の線分上の最近接点とRetVec1の法線
		Vector3 Start = DestCap.m_PointBottom;
		Vector3 End = DestCap.m_PointTop;
		float t;
		Vector3 RetVec;
		HitTest::ClosetPtPointSegment(RetVec1,
			Start, End, t, RetVec);
		Vector3 ContactBase = RetVec - RetVec1;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = SrcCap.GetCenter() + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);
		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionCapsule::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		CAPSULE SrcCap = GetCapsule();
		OBB DestObb = DestColl->GetObb();
		Vector3 RetVec;
		HitTest::CAPSULE_OBB(SrcCap, DestObb, RetVec);
		//接点へのベクトル
		float t;
		Vector3 SegPoint;
		HitTest::ClosetPtPointSegment(RetVec, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
		Vector3 ContactBase = RetVec - SegPoint;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = SrcCap.GetCenter() + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);
		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionCapsule::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionTriangles>& DestColl, float SpanTime) {
		CAPSULE SrcCap = GetCapsule();

		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Vector3 Normal = trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
		Vector3 ContactBase = Normal;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();


		auto Pos = SrcCap.GetCenter() + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);


	}



	void CollisionCapsule::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime) {
		CAPSULE SrcCap = GetCapsule();
		COLRECT DestRect = DestColl->GetColRect();
		PLANE p = DestRect.GetPLANE();
		Vector3 ContactBase = p.m_Normal;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = SrcCap.GetCenter() + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);
		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		SPHERE DestSphere = DestColl->GetSphere();
		Vector3 Ret;
		if (HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
			int count = 0;
			Vector3 span = DestSphere.m_Center - Ret;
			span *= 0.02f;
			while (1) {
				Center += span;
				SrcCap.SetCenter(Center);
				if (!HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		CAPSULE DestCap = DestColl->GetCapsule();
		Vector3 Ret1,Ret2;
		if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1,Ret2)) {
			int count = 0;
			//接点へのベクトル
			float t;
			Vector3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret1, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
			Vector3 span = SegPoint - Ret1;
			span *= 0.02f;
			while (1) {
				Center += span;
				SrcCap.SetCenter(Center);
				if (!HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		OBB DestObb = DestColl->GetObb();
		Vector3 Ret;
		if (HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
			int count = 0;
			//接点へのベクトル
			float t;
			Vector3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
			Vector3 span = SegPoint - Ret;
			span *= 0.02f;
			while (1) {
				Center += span;
				SrcCap.SetCenter(Center);
				if (!HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Vector3 Ret;
		bool Ishit = HitTest::HitTest::CAPSULE_TRIANGLE(SrcCap, trivec[pImpl->m_IsHitVolumeIndex], Ret);
		if (Ishit) {
			Vector3 span = trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
			span.Normalize();
			span *= 0.02f;
			int count = 0;
			while (1) {
				Center += span;
				SrcCap.SetCenter(Center);
				if (!HitTest::CAPSULE_TRIANGLE(SrcCap, trivec[pImpl->m_IsHitVolumeIndex], Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}


	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		COLRECT DestRect = DestColl->GetColRect();
		Vector3 Ret;
		if (HitTest::CAPSULE_COLRECT(SrcCap, DestRect, Ret)) {
			PLANE p = DestRect.GetPLANE();
			Vector3 span = p.m_Normal;
			span.Normalize();
			span *= -0.02f;
			int count = 0;
			auto Center = SrcCap.GetCenter();
			while (1) {
				Center += span;
				SrcCap.SetCenter(Center);
				if (!HitTest::CAPSULE_COLRECT(SrcCap, DestRect, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			//相手がRectの場合はリセット
			PtrTransform->ResetPosition(Center);
		}

	}

	void CollisionCapsule::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CAPSULE"));
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

	void CollisionObb::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionObb>());
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

	void CollisionObb::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
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
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		//SPHEREとOBBの扱いが逆になる
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforeCapsule, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)) {
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

	void CollisionObb::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {
		//OBBと三角形配列は未対応

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

	void CollisionObb::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) {
		OBB obb = GetObb();
		SPHERE sp = DestColl->GetSphere();
		Vector3 Ret;
		HitTest::SPHERE_OBB(sp, obb, Ret);
		//接点へのベクトル
		//逆になる
		Vector3 ContactBase = sp.m_Center - Ret;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();

		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionObb::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) {
		OBB obb = GetObb();
		CAPSULE cap = DestColl->GetCapsule();
		Vector3 Ret;
		HitTest::CAPSULE_OBB(cap, obb, Ret);
		//接点へのベクトル
		//逆になる
		Vector3 ContactBase = cap.GetCenter() - Ret;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();

		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}


	void CollisionObb::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) {
		OBB obb = GetObb();
		OBB obb2 = DestColl->GetObb();
		Vector3 Ret;
		//SrcのOBBとDestの最近接点を得る
		HitTest::ClosestPtPointOBB(obb.m_Center, obb2, Ret);
		//接点へのベクトル
		Vector3 ContactBase = Ret - obb.m_Center;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);

		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);
	}

	void CollisionObb::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionTriangles>& DestColl, float SpanTime) {
		//OBBと三角形配列は未対応

	}



	void CollisionObb::AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime){
		OBB obb = GetObb();
		COLRECT rect = DestColl->GetColRect();
		//Rectのベクトル
		Vector3 ContactBase = DestColl->GetColRect().GetPLANE().m_Normal;
		ContactBase.Normalize();
		//スライドする方向を計算
		Vector3 Slide = Vector3EX::Slide(TotalVelocoty, ContactBase);
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Pos = obb.m_Center + Slide * SpanTime;
		PtrTransform->SetToBefore();
		PtrTransform->SetPosition(Pos);
		//GravityとRigidbodyの後処理（Collision共通）
		AfterCollisionSub(DestColl, ContactBase);

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


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		OBB SrcObb = GetObb();
		auto Center = SrcObb.m_Center;
		CAPSULE DestCapsule = DestColl->GetCapsule();
		Vector3 Ret;
		bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb,Ret);
		if (Ishit) {
			int count = 0;
			//接点へのベクトル
			float t;
			Vector3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret, DestCapsule.m_PointBottom, DestCapsule.m_PointTop, t, SegPoint);
			Vector3 span = Ret - SegPoint;
			span *= 0.02f;
			while (1) {
				Center += span;
				SrcObb.m_Center = Center;
				if (!HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
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
			Vector3 span = SrcObb.m_Center - Ret;
			span.Normalize();
			span *= 0.02f;
			auto Center = SrcObb.m_Center;
			int count = 0;
			while (1) {
				Center += span;
				SrcObb.m_Center = Center;
				if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->SetPosition(Center);
		}
	}

	void CollisionObb::CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {
		//OBBと三角形配列は未対応

	}


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		OBB SrcObb = GetObb();
		COLRECT DestRect = DestColl->GetColRect();
		bool Ishit = HitTest::OBB_COLRECT(SrcObb, DestRect);
		if (Ishit) {
			Vector3 span = DestColl->GetColRect().GetPLANE().m_Normal;
			span.Normalize();
			span *= -0.02f;
			auto Center = SrcObb.m_Center;
			int count = 0;
			while (1) {
				Center += span;
				SrcObb.m_Center = Center;
				if (!HitTest::OBB_COLRECT(SrcObb, DestRect)) {
					break;
				}
				count++;
				if (count > 50) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			PtrTransform->ResetPosition(Center);
		}
	}


	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));

	}

	//--------------------------------------------------------------------------------------
	//	struct CollisionTriangles::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionTriangles::Impl {
		vector<TRIANGLE> m_MakedTriangles;
		wstring m_WireFrameMeshKey;
		Impl()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionTriangles : public Collision ;
	//	用途: 三角形の配列衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionTriangles::CollisionTriangles(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionTriangles::~CollisionTriangles() {}

	//初期化
	void CollisionTriangles::OnCreate() {
		SetFixed(true),
		SetDrawActive(false);
	}

	//アクセサ
	void CollisionTriangles::SetFixed(bool b) {
		if (!b) {
			throw BaseException(
				L"CollisionTrianglesはFixed以外は選択できません",
				L"if (!b)",
				L"CollisionTriangles::SetFixed()"
			);
		}
		else {
			Collision::SetFixed(true);
		}
	}

	const vector<TRIANGLE>& CollisionTriangles::GetMakedTriangles() const {
		return pImpl->m_MakedTriangles;
	}

	void CollisionTriangles::SetMakedTriangles(const vector<TRIANGLE>& trivec) {
		pImpl->m_MakedTriangles.resize(trivec.size());
		pImpl->m_MakedTriangles = trivec;
	}

	void CollisionTriangles::SetWireFrameMesh(const wstring& Key) {
		pImpl->m_WireFrameMeshKey = Key;

	}


	void CollisionTriangles::GetTriangles(vector<TRIANGLE>& trivec) const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		auto mat = TrasnsPtr->GetWorldMatrix();
		if (!pImpl->m_MakedTriangles.empty()) {
			trivec.clear();
			for (auto& v : pImpl->m_MakedTriangles) {
				TRIANGLE set(v.m_A, v.m_B, v.m_C, mat);
				trivec.push_back(set);
			}
		}
	}

	void CollisionTriangles::GetBeforeTriangles(vector<TRIANGLE>& trivec) const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		auto mat = TrasnsPtr->GetBeforeWorldMatrix();
		if (!pImpl->m_MakedTriangles.empty()) {
			trivec.clear();
			for (auto& v : pImpl->m_MakedTriangles) {
				TRIANGLE set(v.m_A, v.m_B, v.m_C, mat);
				trivec.push_back(set);
			}
		}
	}

	AABB CollisionTriangles::GetWrapedAABB() {
		Vector3 min_v(0, 0, 0);
		Vector3 max_v(0, 0, 0);
		vector<TRIANGLE> trivec;
		GetBeforeTriangles(trivec);
		if (!trivec.empty()) {
			bool first = true;
			for (auto& v : trivec) {
				if (first) {
					min_v = v.m_A;
					max_v = v.m_A;
					first = false;
				}
				HitTest::ChkSetMinMax(v.m_A, min_v, max_v);
				HitTest::ChkSetMinMax(v.m_B, min_v, max_v);
				HitTest::ChkSetMinMax(v.m_C, min_v, max_v);
			}
		}
		trivec.clear();
		GetTriangles(trivec);
		if (!trivec.empty()) {
			for (auto& v : trivec) {
				HitTest::ChkSetMinMax(v.m_A, min_v, max_v);
				HitTest::ChkSetMinMax(v.m_B, min_v, max_v);
				HitTest::ChkSetMinMax(v.m_C, min_v, max_v);
			}
		}
		return AABB(min_v, max_v);
	}



	void CollisionTriangles::CollisionCall(const shared_ptr<Collision>& Src){
		Src->CollisionTest(GetThis<CollisionTriangles>());
	}

	void CollisionTriangles::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(pImpl->m_WireFrameMeshKey));
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
		else {
			Collision::SetFixed(true);
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

	void CollisionRect::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionRect>());
	}



	void CollisionRect::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SQUARE"));

	}








}
//end basecross

