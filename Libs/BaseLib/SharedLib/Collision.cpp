/*!
@file Collision.cpp
@brief 衝突判定コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Collision::Impl {
		bool m_Fixed;		//静止オブジェクトかどうか
		float m_EscapeAdjustedValue;	//エスケープ処理の調整値

		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//判定から除外するグループ
		//判定から除外するタグ
		set<wstring> m_ExcludeCollisionTags;

		IsHitAction m_IsHitAction;	//衝突した時の動作定義

		vector<shared_ptr<GameObject>> m_BeforeHitObjectVec;	//1つ前にヒットしたオブジェクト
		vector<shared_ptr<GameObject>> m_HitObjectVec;	//ヒットしたオブジェクト
		vector<shared_ptr<GameObject>> m_TempHitObjectVec;	//汎用ヒットしたオブジェクト

		inline  bsm::Vec3 Slide(const bsm::Vec3& Vec, const bsm::Vec3& Norm) {
			//thisと法線から直行線の長さ（内積で求める）
			float Len = bsm::dot(Vec, Norm);
			//その長さに伸ばす
			bsm::Vec3 Contact = Norm * Len;
			//スライドする方向は現在のベクトルから引き算
			return (Vec - Contact);
		}


		Impl() :
			m_Fixed(false),
			m_EscapeAdjustedValue(0.0f),
			m_IsHitAction(IsHitAction::Auto)
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
	float Collision::GetEscapeAdjustedValue() const {
		return pImpl->m_EscapeAdjustedValue;
	}
	void Collision::SetEscapeAdjustedValue(float f) {
		pImpl->m_EscapeAdjustedValue = f;
	}


	shared_ptr<GameObjectGroup> Collision::GetExcludeCollisionGroup() const {
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void Collision::SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_ExcludeCollisionGroup = Group;
	}


	bool Collision::FindExcludeCollisionTag(const wstring& tagstr) const {
		if (pImpl->m_ExcludeCollisionTags.find(tagstr) == pImpl->m_ExcludeCollisionTags.end()) {
			return false;
		}
		return true;
	}
	void  Collision::AddExcludeCollisionTag(const wstring& tagstr) {
		if (tagstr == L"") {
			//空白なら例外
			throw BaseException(
				L"設定するタグが空です",
				L"if (tagstr == L"")",
				L"Collision::AddExcludeCollisionTag()"
			);
		}
		pImpl->m_ExcludeCollisionTags.insert(tagstr);

	}
	void  Collision::RemoveExcludeCollisionTag(const wstring& tagstr) {
		pImpl->m_ExcludeCollisionTags.erase(tagstr);
	}



	bool Collision::IsExcludeCollisionObject(const shared_ptr<GameObject>& Obj) const {
		auto& tagset = Obj->GetTagSet();
		auto it = tagset.begin();
		while (it != tagset.end()) {
			if (FindExcludeCollisionTag(*it)) {
				return true;
			}
			it++;
		}
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			auto Vec = shptr->GetGroupVector();
			for (auto& v : Vec) {
				auto shobj = v.lock();
				if (shobj && (shobj == Obj)) {
					return true;
				}
			}
		}
		return false;
	}




	//操作
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

	bool Collision::IsHitObject(const shared_ptr<GameObject>& Obj) const {
		if (!pImpl->m_HitObjectVec.empty()) {
			for (auto& v : pImpl->m_HitObjectVec) {
				if (v == Obj) {
					return true;
				}
			}
		}
		return false;
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

	void Collision::AfterCollision(const shared_ptr<Collision>& DestColl, const bsm::Vec3& SrcVelocity, const bsm::Vec3& DestVelocity, 
		const bsm::Vec3& HitNormal,float AfterHitTime) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//親の速度
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		auto DestVelo = PtrDestTransform->GetWorldMatrix().transInMatrix() 
			- PtrDestTransform->GetBeforeWorldMatrix().transInMatrix();

		//スライドする方向を計算
		bsm::Vec3 Slide = pImpl->Slide(SrcVelocity, HitNormal);
		PtrTransform->SetToBefore();
		if (GetIsHitAction() != IsHitAction::Stop && GetIsHitAction() != IsHitAction::None) {
			auto WorldPos = PtrTransform->GetWorldPosition() + Slide * AfterHitTime;
			PtrTransform->SetWorldPosition(WorldPos);
		}

		bool horizontal = false;
		if (GetGameObject()->FindBehavior<Gravity>()) {
			auto Grav = GetGameObject()->GetBehavior<Gravity>()->GetGravity();
			Grav.normalize();
			bsm::Vec3 v = (bsm::Vec3)XMVector3AngleBetweenNormals(Grav, HitNormal);
			if (v.x <= XM_PIDIV4) {
				horizontal = true;
			}
		}

		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>(false);
		if (PtrRigid) {
			switch (GetIsHitAction()) {
			case IsHitAction::Slide:
			{
				auto DestVelocity = PtrDestTransform->GetVelocity();
				Slide = pImpl->Slide(SrcVelocity, HitNormal);
				PtrRigid->SetVelocity(Slide);
				//重力は0にする
				PtrRigid->SetGravityVelocityZero();
			}
			break;
			case IsHitAction::Auto:
				if (horizontal) {
					//乗っているときはスライドさせる
					Slide = pImpl->Slide(SrcVelocity, HitNormal);
					PtrRigid->SetVelocity(Slide);
					//何かに乗ったときは重力は0にする
					PtrRigid->SetGravityVelocityZero();
				}
				else {
					//乗ってないときは反発
					auto DestRigid = DestColl->GetGameObject()->GetComponent<Rigidbody>(false);
					float ResultPower = -(1.0f + PtrRigid->GetReflection());
					if (DestRigid) {
						bsm::Vec3 RelativeVelo = SrcVelocity - DestVelocity;
						ResultPower = (-(1.0f + PtrRigid->GetReflection()) * bsm::dot(RelativeVelo, HitNormal)) /
							(bsm::dot(HitNormal, HitNormal) * (1 / PtrRigid->GetMass() + 1 / DestRigid->GetMass()));
					}
					else {
						bsm::Vec3 RelativeVelo = SrcVelocity;
						ResultPower = (-(1.0f + PtrRigid->GetReflection()) * bsm::dot(RelativeVelo, HitNormal)) /
							(bsm::dot(HitNormal, HitNormal) * (1 / PtrRigid->GetMass()));
					}
					auto Velo = PtrRigid->GetVelocity();
					Velo += (HitNormal * ResultPower) / PtrRigid->GetMass();
					PtrRigid->SetVelocity(Velo);
				}
				break;
			case IsHitAction::Stop:
				{
					//速度は0にする
					PtrRigid->SetVelocityZero();
					//重力は0にする
					PtrRigid->SetGravityVelocityZero();
				}
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
			m_CalcScaling(CalcScaling::YScale)
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
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
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
		//Sphere同士はCillisionWrappedSphere()は呼び出さない
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//球の場合は、すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionSphere, CollisionSphere>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionSphere>(), DestColl);
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {

			AfterCollisionTmpl_1<CollisionSphere, CollisionCapsule>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionSphere>(), DestColl);
		}
	}



	void CollisionSphere::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionSphere, CollisionObb>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionSphere>(), DestColl);
		}
	}
	void CollisionSphere::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手まずはAABBで調査
		AABB WrapAABB = DestColl->GetWrappedAABB();
		//ラッピングAABBは静止している
		bsm::Vec3 SpanVelocity = SrcVelocity;
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
				AfterCollisionTmpl_2<CollisionSphere, CollisionTriangles>
					(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionSphere>(), DestColl);
				//1つの三角形と衝突したら処理終了
				break;
			}
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereRect(SrcBeforSphere, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_2<CollisionSphere, CollisionRect>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionSphere>(), DestColl);
		}
	}



	void CollisionSphere::BackToBefore(const bsm::Vec3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Pos = SrcBeforSphere.m_Center + TotalVelocoty * SpanTime;
		PtrTransform->ResetWorldPosition(Pos);
	}


	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		SPHERE sp2 = DestColl->GetSphere();
		//接点へのベクトル
		Ret = sp2.m_Center - sp.m_Center;
		Ret.normalize();
	}


	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		CAPSULE cap = DestColl->GetCapsule();
		bsm::Vec3 RetPoint;
		HitTest::SPHERE_CAPSULE(sp, cap, RetPoint);
		//接点へのベクトル
		Ret = RetPoint - sp.m_Center;
		Ret.normalize();
	}


	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		OBB obb = DestColl->GetObb();
		bsm::Vec3 RetPoint;
		HitTest::SPHERE_OBB(sp, obb, RetPoint);
		//接点へのベクトル
		Ret = RetPoint - sp.m_Center;
		Ret.normalize();
	}

	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Ret = -trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
		Ret.normalize();
	}

	void CollisionSphere::GetHitNormal(const shared_ptr<CollisionRect>& DestColl, bsm::Vec3& Ret) const {
		SPHERE sp = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		//面のベクトル
		PLANE p = DestRect.GetPLANE();
		Ret = p.m_Normal;
		Ret.normalize();
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		SPHERE DestSphere = DestColl->GetSphere();
		if (HitTest::SPHERE_SPHERE(SrcSphere, DestSphere)) {
			bsm::Vec3 Normal = SrcSphere.m_Center - DestSphere.m_Center;
			Normal.normalize();
			float Span = SrcSphere.m_Radius + DestSphere.m_Radius;
			Normal *= Span;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			bsm::Vec3 Pos = DestSphere.m_Center + Normal;
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Pos);
		}
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		CAPSULE DestCap = DestColl->GetCapsule();
		bsm::Vec3 Ret;
		bool Ishit = HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret);
		if (Ishit) {
			bsm::Vec3 span = SrcSphere.m_Center - Ret;
			span.normalize();
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}


	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = DestColl->GetObb();
		bsm::Vec3 Ret;
		bool Ishit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
		if (Ishit) {
			bsm::Vec3 span = SrcSphere.m_Center - Ret;
			span.normalize();
			span *= SrcSphere.m_Radius;
			span += Ret;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(span);
		}
	}

	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		bsm::Vec3 Ret;
		bool Ishit = HitTest::HitTest::SPHERE_TRIANGLE(SrcSphere, trivec[pImpl->m_IsHitVolumeIndex], Ret);
		if (Ishit) {
			bsm::Vec3 span = bsm::cross(trivec[pImpl->m_IsHitVolumeIndex].m_B - trivec[pImpl->m_IsHitVolumeIndex].m_A,
				trivec[pImpl->m_IsHitVolumeIndex].m_C - trivec[pImpl->m_IsHitVolumeIndex].m_A);
			span.normalize();
			span *= 0.0005f;
			int count = 0;
			auto Center = SrcSphere.m_Center;
			while (1) {
				Center += span;
				SrcSphere.m_Center = Center;
				if (!HitTest::SPHERE_TRIANGLE(SrcSphere, trivec[pImpl->m_IsHitVolumeIndex], Ret)) {
					break;
				}
				count++;
				if (count > 100) {
					break;
				}
			}
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}

	}


	void CollisionSphere::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		SPHERE SrcSphere = GetSphere();
		COLRECT DestRect = DestColl->GetColRect();
		bsm::Vec3 Ret;
		bool Ishit = HitTest::HitTest::SPHERE_COLRECT(SrcSphere, DestRect, Ret);
		if (Ishit) {
			auto Norm = SrcSphere.m_Center - Ret;
			Norm.normalize();
			Norm *= SrcSphere.m_Radius;
			Norm += Ret;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Norm);
		}
	}

	SPHERE  CollisionSphere::GetEnclosingSphere()const {
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcSphere, SrcBeforSphere);
		return Src;
	}

	bool CollisionSphere::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionSphere::GetWrappingAABB()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.GetWrappedAABB();
	}

	bsm::Vec3 CollisionSphere::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	void CollisionSphere::OnDraw() {
		GenericDraw Draw;
		bsm::Mat4x4 MeshToTransformMatrix;

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Scale = PtrTransform->GetScale();
		bsm::Vec3 CollScale(Scale.x, Scale.x, Scale.x);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			CollScale = bsm::Vec3(Scale.y, Scale.y, Scale.y);
			break;
		case CalcScaling::ZScale:
			CollScale = bsm::Vec3(Scale.z, Scale.z, Scale.z);
			break;
		default:
			break;
		}
		bsm::Vec3 ColcScale(CollScale.x / Scale.x, CollScale.y / Scale.y, CollScale.z / Scale.z);
		bsm::Mat4x4 mat;
		mat.scale(ColcScale);

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

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		CAPSULE m_BeforeWorldCapsule;
		CAPSULE m_WorldCapsule;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;

		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f),
			m_IsHitVolumeIndex(0),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldCapsule(),
			m_WorldCapsule(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)

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
		if (pImpl->m_FirstCalc || !PtrT->IsSameWorldMatrix(pImpl->m_WorldMatrix)) {
			pImpl->m_WorldMatrix = PtrT->GetWorldMatrix();
			pImpl->m_WorldCapsule = CAPSULE(
				pImpl->m_MakedDiameter * 0.5f,
				bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
				bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
				pImpl->m_WorldMatrix);
			pImpl->m_FirstCalc = false;
		}
		return pImpl->m_WorldCapsule;
	}

	CAPSULE CollisionCapsule::GetBeforeCapsule() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		if (pImpl->m_FirstBeforeCalc || !PtrT->IsSameBeforeWorldMatrix(pImpl->m_BeforeWorldMatrix)) {
			pImpl->m_BeforeWorldMatrix = PtrT->GetBeforeWorldMatrix();
			pImpl->m_BeforeWorldCapsule = CAPSULE(
				pImpl->m_MakedDiameter * 0.5f,
				bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
				bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
				pImpl->m_BeforeWorldMatrix);
			pImpl->m_FirstBeforeCalc = false;
		}
		return pImpl->m_BeforeWorldCapsule;
	}

	void CollisionCapsule::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionCapsule>());
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとCAPSULEの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcBeforCapsule, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionCapsule, CollisionSphere>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionCapsule>(), DestColl);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionCapsule
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestBeforeCapsule, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionCapsule, CollisionCapsule>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionCapsule>(), DestColl);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionObb
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionCapsule, CollisionObb>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionCapsule>(), DestColl);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手まずはAABBで調査
		AABB WrapAABB = DestColl->GetWrappedAABB();
		//ラッピングAABBは静止している
		bsm::Vec3 SpanVelocity = SrcVelocity;
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
				AfterCollisionTmpl_2<CollisionCapsule, CollisionTriangles>
					(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionCapsule>(), DestColl);
				//1つの三角形と衝突したら処理終了
				break;
			}
		}


	}


	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//相手のCollisionRect
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleRect(SrcBeforCapsule, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_2<CollisionCapsule, CollisionRect>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionCapsule>(), DestColl);
		}
	}

	void CollisionCapsule::BackToBefore(const bsm::Vec3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Pos = SrcBeforCapsule.GetCenter() + TotalVelocoty * SpanTime;
		PtrTransform->ResetWorldPosition(Pos);
	}

	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE cap = GetCapsule();
		SPHERE sp = DestColl->GetSphere();
		bsm::Vec3 RetVec;
		HitTest::SPHERE_CAPSULE(sp, cap, RetVec);
		//接点へのベクトル
		Ret = sp.m_Center - RetVec;
		Ret.normalize();
	}


	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();
		CAPSULE DestCap = DestColl->GetCapsule();
		bsm::Vec3 RetVec1, RetVec2;
		HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, RetVec1, RetVec2);
		//接点へのベクトル
		//DestCapの線分とRetVec1の線分上の最近接点とRetVec1の法線
		bsm::Vec3 Start = DestCap.m_PointBottom;
		bsm::Vec3 End = DestCap.m_PointTop;
		float t;
		bsm::Vec3 RetVec;
		HitTest::ClosetPtPointSegment(RetVec1,
			Start, End, t, RetVec);
		Ret = RetVec - RetVec1;
		Ret.normalize();
	}

	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();
		OBB DestObb = DestColl->GetObb();
		bsm::Vec3 RetVec;
		HitTest::CAPSULE_OBB(SrcCap, DestObb, RetVec);
		//接点へのベクトル
		float t;
		bsm::Vec3 SegPoint;
		HitTest::ClosetPtPointSegment(RetVec, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
		Ret = RetVec - SegPoint;
		Ret.normalize();
	}


	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();

		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		Ret = -trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
		Ret.normalize();
	}

	void CollisionCapsule::GetHitNormal(const shared_ptr<CollisionRect>& DestColl, bsm::Vec3& Ret) const {
		CAPSULE SrcCap = GetCapsule();
		COLRECT DestRect = DestColl->GetColRect();
		PLANE p = DestRect.GetPLANE();
		Ret = p.m_Normal;
		Ret.normalize();
	}


	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		SPHERE DestSphere = DestColl->GetSphere();
		bsm::Vec3 Ret;
		if (HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
			int count = 0;
			bsm::Vec3 span = DestSphere.m_Center - Ret;
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		CAPSULE DestCap = DestColl->GetCapsule();
		bsm::Vec3 Ret1,Ret2;
		if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1,Ret2)) {
			int count = 0;
			//接点へのベクトル
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret1, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
			bsm::Vec3 span = SegPoint - Ret1;
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		OBB DestObb = DestColl->GetObb();
		bsm::Vec3 Ret;
		if (HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
			int count = 0;
			//接点へのベクトル
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
			bsm::Vec3 span = SegPoint - Ret;
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}

	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		auto Center = SrcCap.GetCenter();
		vector<TRIANGLE> trivec;
		DestColl->GetTriangles(trivec);
		bsm::Vec3 Ret;
		bool Ishit = HitTest::HitTest::CAPSULE_TRIANGLE(SrcCap, trivec[pImpl->m_IsHitVolumeIndex], Ret);
		if (Ishit) {
			bsm::Vec3 span = trivec[pImpl->m_IsHitVolumeIndex].GetNormal();
			span.normalize();
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}


	void CollisionCapsule::CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {
		CAPSULE SrcCap = GetCapsule();
		COLRECT DestRect = DestColl->GetColRect();
		bsm::Vec3 Ret;
		if (HitTest::CAPSULE_COLRECT(SrcCap, DestRect, Ret)) {
			PLANE p = DestRect.GetPLANE();
			bsm::Vec3 span = p.m_Normal;
			span.normalize();
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}

	}

	SPHERE  CollisionCapsule::GetEnclosingSphere()const {
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcCapsule.GetWrappedSPHERE(), SrcBeforCapsule.GetWrappedSPHERE());
		return Src;
	}

	bool CollisionCapsule::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionCapsule::GetWrappingAABB()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetWrappedAABB();
	}

	bsm::Vec3 CollisionCapsule::GetCenterPosition()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetCenter();
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

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		OBB m_BeforeWorldObb;
		OBB m_WorldObb;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;


		Impl() :
			m_Size(1.0f),
			m_ChkOnUnderLaySize(0.1f),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldObb(),
			m_WorldObb(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)
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
		if (pImpl->m_FirstCalc || !PtrT->IsSameWorldMatrix(pImpl->m_WorldMatrix)) {
			pImpl->m_WorldMatrix = PtrT->GetWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_WorldMatrix;
			pImpl->m_WorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstCalc = false;
		}
		return pImpl->m_WorldObb;
	}
	OBB CollisionObb::GetBeforeObb() const {
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		if (pImpl->m_FirstBeforeCalc || !PtrT->IsSameBeforeWorldMatrix(pImpl->m_BeforeWorldMatrix)) {
			pImpl->m_BeforeWorldMatrix = PtrT->GetBeforeWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_BeforeWorldMatrix;
			pImpl->m_BeforeWorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstBeforeCalc = false;
		}
		return pImpl->m_BeforeWorldObb;
	}

	void CollisionObb::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionObb>());
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {

			AfterCollisionTmpl_1<CollisionObb, CollisionSphere>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionObb>(), DestColl);
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {

		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//相手
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		//SPHEREとOBBの扱いが逆になる
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforeCapsule, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionObb, CollisionCapsule>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionObb>(), DestColl);
		}
	}


	void CollisionObb::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {

		if (!CillisionWrappedSphere(DestColl)) {
			//Sphereの重なりがないなら終了
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		//相手
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_1<CollisionObb, CollisionObb>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionObb>(), DestColl);
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {
		//OBBと三角形配列は未対応

	}


	void CollisionObb::CollisionTest(const shared_ptr<CollisionRect>& DestColl) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//移動以外変化なし
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		//相手
		COLRECT DestRect = DestColl->GetColRect();
		COLRECT DestBeforeRect = DestColl->GetBeforeColRect();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbRect(SrcBeforObb, SpanVelocity, DestBeforeRect, 0, ElapsedTime, HitTime)) {
			AfterCollisionTmpl_2<CollisionObb, CollisionRect>
				(SrcVelocity, DestVelocity, ElapsedTime, HitTime, GetThis<CollisionObb>(), DestColl);
		}
	}



	void CollisionObb::BackToBefore(const bsm::Vec3 TotalVelocoty, float SpanTime) {
		//すべて移動以外変化なしとする
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Pos = SrcBeforObb.m_Center + TotalVelocoty * SpanTime;
		PtrTransform->ResetWorldPosition(Pos);
	}

	void CollisionObb::GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		SPHERE sp = DestColl->GetSphere();
		bsm::Vec3 RetVec;
		HitTest::SPHERE_OBB(sp, obb, RetVec);
		//接点へのベクトル
		//逆になる
		Ret = sp.m_Center - RetVec;
		Ret.normalize();
	}


	void CollisionObb::GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		CAPSULE cap = DestColl->GetCapsule();
		bsm::Vec3 RetVec;
		HitTest::CAPSULE_OBB(cap, obb, RetVec);
		//接点へのベクトル
		//逆になる
		Ret = cap.GetCenter() - RetVec;
		Ret.normalize();
	}


	void CollisionObb::GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		OBB obb2 = DestColl->GetObb();
		bsm::Vec3 RetVec;
		//SrcのOBBとDestの最近接点を得る
		HitTest::ClosestPtPointOBB(obb.m_Center, obb2, RetVec);
		//接点へのベクトル
		Ret = RetVec - obb.m_Center;
		Ret.normalize();
	}



	void CollisionObb::GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, bsm::Vec3& Ret) const {
		//OBBと三角形配列は未対応
	}

	void CollisionObb::GetHitNormal(const shared_ptr<CollisionRect>& DestColl, bsm::Vec3& Ret) const {
		OBB obb = GetObb();
		COLRECT rect = DestColl->GetColRect();
		//Rectのベクトル
		Ret = DestColl->GetColRect().GetPLANE().m_Normal;
		Ret.normalize();
	}


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {
		OBB SrcObb = GetObb();
		SPHERE DestSphere = DestColl->GetSphere();
		bsm::Vec3 Ret;
		bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
		if (Ishit) {
			bsm::Vec3 NowSpan = Ret - DestSphere.m_Center;
			bsm::Vec3 NewSpan = NowSpan;
			NewSpan.normalize();
			NewSpan *= (DestSphere.m_Radius);
			auto MoveSpan = NewSpan - NowSpan;
			auto PtrTransform = GetGameObject()->GetComponent<Transform>();
			auto Pos = PtrTransform->GetWorldMatrix().transInMatrix();
			Pos += MoveSpan;
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Pos);
		}
	}


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {
		OBB SrcObb = GetObb();
		auto Center = SrcObb.m_Center;
		CAPSULE DestCapsule = DestColl->GetCapsule();
		bsm::Vec3 Ret;
		bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb,Ret);
		if (Ishit) {
			int count = 0;
			//接点へのベクトル
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(Ret, DestCapsule.m_PointBottom, DestCapsule.m_PointTop, t, SegPoint);
			bsm::Vec3 span = Ret - SegPoint;
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}


	void CollisionObb::CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {
		OBB SrcObb = GetObb();
		OBB DestObb = DestColl->GetObb();
		bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
		if (Ishit) {
			bsm::Vec3 Ret;
			//SrcのOBBとDestの最近接点を得る
			HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, Ret);
			bsm::Vec3 span = SrcObb.m_Center - Ret;
			span.normalize();
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
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
			bsm::Vec3 span = DestColl->GetColRect().GetPLANE().m_Normal;
			span.normalize();
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
			//エスケープはリセット
			PtrTransform->ResetWorldPosition(Center);
		}
	}

	SPHERE  CollisionObb::GetEnclosingSphere()const {
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcObb.GetWrappedSPHERE(), SrcBeforeObb.GetWrappedSPHERE());
		return Src;
	}

	bool CollisionObb::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionObb::GetWrappingAABB()const {
		OBB SrcObb = GetObb();
		return SrcObb.GetWrappedAABB();
	}

	bsm::Vec3 CollisionObb::GetCenterPosition()const {
		OBB SrcObb = GetObb();
		return SrcObb.m_Center;
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
		vector<TRIANGLE> m_WorldTriangles;
		vector<TRIANGLE> m_BeforWorldTriangles;

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		bool m_FirstBeforeCalc;
		bool m_FirstCalc;

		AABB m_WrappedAABB;
		wstring m_WireFrameMeshKey;
		Impl() :
			m_WorldTriangles(),
			m_BeforWorldTriangles(),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true),
			m_WrappedAABB(bsm::Vec3(0, 0, 0), bsm::Vec3(0, 0, 0))
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
		if (pImpl->m_FirstCalc || !TrasnsPtr->IsSameWorldMatrix(pImpl->m_WorldMatrix)) {
			pImpl->m_WorldMatrix = TrasnsPtr->GetWorldMatrix();
			if (!pImpl->m_MakedTriangles.empty()) {
				pImpl->m_WorldTriangles.clear();
				for (auto& v : pImpl->m_MakedTriangles) {
					TRIANGLE set(v.m_A, v.m_B, v.m_C, pImpl->m_WorldMatrix);
					pImpl->m_WorldTriangles.push_back(set);
				}
			}
			pImpl->m_FirstCalc = false;
		}
		if (!pImpl->m_WorldTriangles.empty()) {
			trivec.resize(pImpl->m_WorldTriangles.size());
			trivec = pImpl->m_WorldTriangles;
		}
	}

	void CollisionTriangles::GetBeforeTriangles(vector<TRIANGLE>& trivec) const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		if (pImpl->m_FirstBeforeCalc || !TrasnsPtr->IsSameBeforeWorldMatrix(pImpl->m_BeforeWorldMatrix)) {
			pImpl->m_BeforeWorldMatrix = TrasnsPtr->GetBeforeWorldMatrix();
			if (!pImpl->m_MakedTriangles.empty()) {
				pImpl->m_BeforWorldTriangles.clear();
				for (auto& v : pImpl->m_MakedTriangles) {
					TRIANGLE set(v.m_A, v.m_B, v.m_C, pImpl->m_BeforeWorldMatrix);
					pImpl->m_BeforWorldTriangles.push_back(set);
				}
			}
			pImpl->m_FirstBeforeCalc = false;
		}
		if (!pImpl->m_BeforWorldTriangles.empty()) {
			trivec.resize(pImpl->m_BeforWorldTriangles.size());
			trivec = pImpl->m_BeforWorldTriangles;
		}
	}

	AABB CollisionTriangles::GetWrappedAABB()const {
		auto TrasnsPtr = GetGameObject()->GetComponent<Transform>();
		if (
			pImpl->m_FirstBeforeCalc || pImpl->m_FirstCalc ||
			!TrasnsPtr->IsSameBeforeWorldMatrix(pImpl->m_BeforeWorldMatrix) ||
			!TrasnsPtr->IsSameWorldMatrix(pImpl->m_WorldMatrix)
			) {
			Vec3 min_v(0, 0, 0);
			Vec3 max_v(0, 0, 0);
			vector<TRIANGLE> trivec;
			trivec.clear();
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
			pImpl->m_WrappedAABB = AABB(min_v, max_v);
		}
		return pImpl->m_WrappedAABB;
	}



	void CollisionTriangles::CollisionCall(const shared_ptr<Collision>& Src){
		Src->CollisionTest(GetThis<CollisionTriangles>());
	}

	SPHERE  CollisionTriangles::GetEnclosingSphere()const {
		AABB aabb = GetWrappedAABB();
		SPHERE src;
		src.m_Radius = bsm::length(aabb.m_Max - aabb.m_Min);
		src.m_Center = aabb.GetCenter();
		return src;
	}

	bool CollisionTriangles::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		//CollisionTrianglesは常にヒットしない
		return false;
	}

	AABB CollisionTriangles::GetWrappingAABB()const {
		bsm::Vec3 min_v(0, 0, 0);
		bsm::Vec3 max_v(0, 0, 0);
		vector<TRIANGLE> trivec;
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

	bsm::Vec3 CollisionTriangles::GetCenterPosition()const {
		return GetWrappedAABB().GetCenter();
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

	SPHERE  CollisionRect::GetEnclosingSphere()const {
		COLRECT SrcColRect = GetColRect();
		COLRECT SrcBeforeColRect = GetBeforeColRect();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcColRect.GetWrappedSPHERE(), SrcBeforeColRect.GetWrappedSPHERE());
		return Src;
	}

	bool CollisionRect::HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2) {
		return false;
	}

	AABB CollisionRect::GetWrappingAABB()const {
		COLRECT SrcColRect = GetColRect();
		return SrcColRect.GetWrappedAABB();
	}

	bsm::Vec3 CollisionRect::GetCenterPosition()const {
		COLRECT SrcColRect = GetColRect();
		return SrcColRect.m_Center;
	}



	void CollisionRect::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SQUARE"));

	}








}
//end basecross

