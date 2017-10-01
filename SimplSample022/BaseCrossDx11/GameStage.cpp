/*!
@file GameStage.h
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	コリジョン速度計算用
	//--------------------------------------------------------------------------------------
	void Rigidbody::Move(float Spantime) {
		m_Pos += m_Velocity * Spantime;
	}

	bool Rigidbody::CollisionSPHERE(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
			case CollType::typeSPHERE:
			{
				if (HitTest::CollisionTestSphereSphere(GetSPHERE(),
					m_Velocity - Dest.m_Velocity,
					Dest.GetSPHERE(),
					0, ElapsedTime, state.m_HitTime)) {
					SPHERE SrcSp = GetSPHERE();
					SPHERE DestSp = Dest.GetSPHERE();
					SrcSp.m_Center += m_Velocity * state.m_HitTime;
					DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = SrcSp.m_Center - DestSp.m_Center;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = DestSp.m_Center - SrcSp.m_Center;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
			case CollType::typeCAPSULE:
			{
				if (HitTest::CollisionTestSphereCapsule(GetSPHERE(), m_Velocity - Dest.m_Velocity,
					Dest.GetCAPSULE(),
					0, ElapsedTime, state.m_HitTime)) {
					SPHERE SrcSp = GetSPHERE();
					SrcSp.m_Center += m_Velocity * state.m_HitTime;
					CAPSULE DestCap = Dest.GetCAPSULE();
					Vec3 CapCenter = DestCap.GetCenter();
					CapCenter + Dest.m_Velocity * state.m_HitTime;
					DestCap.SetCenter(CapCenter);
					Vec3 Ret;
					HitTest::SPHERE_CAPSULE(SrcSp, DestCap, Ret);
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = SrcSp.m_Center - Ret;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = Ret - SrcSp.m_Center;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
			case CollType::typeOBB:
			{
				if (HitTest::CollisionTestSphereObb(GetSPHERE(),
					m_Velocity - Dest.m_Velocity,
					Dest.GetOBB(),
					0, ElapsedTime, state.m_HitTime)) {
					SPHERE SrcSp = GetSPHERE();
					SrcSp.m_Center += m_Velocity * state.m_HitTime;
					OBB DestObb = Dest.GetOBB();
					DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
					Vec3 Ret;
					HitTest::SPHERE_OBB(SrcSp, DestObb, Ret);
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = SrcSp.m_Center - Ret;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = Ret - SrcSp.m_Center;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
		}
		return false;
	}

	bool Rigidbody::CollisionCAPSULE(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
			case CollType::typeSPHERE:
			{
				if (HitTest::CollisionTestSphereCapsule(Dest.GetSPHERE(),
					Dest.m_Velocity - m_Velocity,
					GetCAPSULE(),
					0, ElapsedTime, state.m_HitTime)) {
					CAPSULE SrcCap = GetCAPSULE();
					Vec3 CapCenter = SrcCap.GetCenter();
					CapCenter + m_Velocity * state.m_HitTime;
					SrcCap.SetCenter(CapCenter);
					SPHERE DestSp = Dest.GetSPHERE();
					DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
					Vec3 Ret;
					HitTest::SPHERE_CAPSULE(DestSp, SrcCap, Ret);
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = Ret - DestSp.m_Center;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = DestSp.m_Center - Ret;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
			case CollType::typeCAPSULE:
			{
				if (HitTest::CollisionTestCapsuleCapsule(GetCAPSULE(),
					m_Velocity - Dest.m_Velocity,
					Dest.GetCAPSULE(),
					0, ElapsedTime, state.m_HitTime)) {
					CAPSULE SrcCap = GetCAPSULE();
					Vec3 CapCenter = SrcCap.GetCenter();
					CapCenter + m_Velocity * state.m_HitTime;
					SrcCap.SetCenter(CapCenter);

					CAPSULE DestCap = Dest.GetCAPSULE();
					CapCenter = DestCap.GetCenter();
					CapCenter + Dest.m_Velocity * state.m_HitTime;
					DestCap.SetCenter(CapCenter);

					Vec3 RetVec1, RetVec2;
					HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, RetVec1, RetVec2);

					//接点へのベクトル
					//DestCapの線分とRetVec1の線分上の最近接点とRetVec1の法線
					float t;
					Vec3 SegVecVec1, SegVecVec2;
					HitTest::ClosetPtPointSegment(RetVec1, DestCap.m_PointBottom, DestCap.m_PointTop, t, SegVecVec1);
					HitTest::ClosetPtPointSegment(RetVec2, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegVecVec2);
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = SegVecVec1 - RetVec1;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = SegVecVec2 - RetVec2;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
			case CollType::typeOBB:
			{
				if (HitTest::CollisionTestCapsuleObb(GetCAPSULE(),
					m_Velocity - Dest.m_Velocity,
					Dest.GetOBB(),
					0, ElapsedTime, state.m_HitTime)) {
					Vec3 RetVec;
					CAPSULE SrcCap = GetCAPSULE();
					Vec3 CapCenter = SrcCap.GetCenter();
					CapCenter + m_Velocity * state.m_HitTime;
					SrcCap.SetCenter(CapCenter);
					OBB DestObb = Dest.GetOBB();
					DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
					HitTest::CAPSULE_OBB(SrcCap, DestObb, RetVec);
					//接点へのベクトル
					float t;
					Vec3 SegPoint;
					HitTest::ClosetPtPointSegment(RetVec, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
					state.m_Src = this;
					state.m_Dest = &Dest;
					state.m_SrcHitNormal = SegPoint - RetVec;
					state.m_SrcHitNormal.normalize();
					state.m_DestHitNormal = RetVec - SegPoint;
					state.m_DestHitNormal.normalize();
					return true;
				}
				return false;
			}
			break;
		}
		return false;
	}

	bool Rigidbody::CollisionOBB(Rigidbody& Dest, float ElapsedTime, CollisionState& state) {
		switch (Dest.m_CollType) {
		case CollType::typeSPHERE:
		{
			if (HitTest::CollisionTestSphereObb(Dest.GetSPHERE(),
				Dest.m_Velocity - m_Velocity,
				GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				OBB SrcObb = GetOBB();
				SrcObb.m_Center += m_Velocity * state.m_HitTime;
				SPHERE DestSp = Dest.GetSPHERE();
				DestSp.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 Ret;
				HitTest::SPHERE_OBB(DestSp, SrcObb, Ret);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = Ret - DestSp.m_Center;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = DestSp.m_Center - Ret;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeCAPSULE:
		{
			if (HitTest::CollisionTestCapsuleObb(Dest.GetCAPSULE(),
				Dest.m_Velocity - m_Velocity,
				GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				Vec3 RetVec;
				OBB SrcObb = GetOBB();
				CAPSULE DestCap = Dest.GetCAPSULE();
				HitTest::CAPSULE_OBB(DestCap, SrcObb, RetVec);
				//接点へのベクトル
				float t;
				Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(RetVec, DestCap.m_PointBottom, DestCap.m_PointTop, t, SegPoint);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = RetVec - SegPoint;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = SegPoint - RetVec;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		case CollType::typeOBB:
		{
			if (HitTest::CollisionTestObbObb(GetOBB(),
				m_Velocity - Dest.m_Velocity,
				Dest.GetOBB(),
				0, ElapsedTime, state.m_HitTime)) {
				OBB SrcObb = GetOBB();
				SrcObb.m_Center += m_Velocity * state.m_HitTime;
				OBB DestObb = Dest.GetOBB();
				DestObb.m_Center += Dest.m_Velocity * state.m_HitTime;
				Vec3 RetVec;
				//SrcのOBBとDestの最近接点を得る
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, RetVec);
				state.m_Src = this;
				state.m_Dest = &Dest;
				state.m_SrcHitNormal = SrcObb.m_Center - RetVec;
				state.m_SrcHitNormal.normalize();
				state.m_DestHitNormal = RetVec - SrcObb.m_Center;
				state.m_DestHitNormal.normalize();
				return true;
			}
			return false;
		}
		break;
		}
		return false;
	}




	void Rigidbody::EscapeSPHERE(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
			case CollType::typeSPHERE:
			{
				bool hit = HitTest::SPHERE_SPHERE(GetSPHERE(), Dest->GetSPHERE());
				if (hit) {
					auto Norm = GetSPHERE().m_Center - Dest->GetSPHERE().m_Center;
					Norm.normalize();
					auto TotalRad = Dest->GetSPHERE().m_Radius + GetSPHERE().m_Radius;
					Norm *= TotalRad;
					m_Pos = Dest->m_Pos + Norm;
				}
			}
			break;
			case CollType::typeCAPSULE:
			{
				Vec3 Ret;
				bool hit = HitTest::SPHERE_CAPSULE(GetSPHERE(), Dest->GetCAPSULE(), Ret);
				if (hit) {
					auto Norm = GetSPHERE().m_Center - Ret;
					Norm.normalize();
					Norm *= GetSPHERE().m_Radius;
					m_Pos = Ret + Norm;
				}
			}
			break;
			case CollType::typeOBB:
			{
				Vec3 Ret;
				bool hit = HitTest::SPHERE_OBB(GetSPHERE(), Dest->GetOBB(), Ret);
				if (hit) {
					auto Norm = GetSPHERE().m_Center - Ret;
					Norm.normalize();
					Norm *= GetSPHERE().m_Radius;
					m_Pos = Ret + Norm;
				}
			}
			break;
		}
	}

	void Rigidbody::EscapeCAPSULE(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
			case CollType::typeSPHERE:
			{
				Vec3 Ret;
				bool hit = HitTest::SPHERE_CAPSULE(Dest->GetSPHERE(), GetCAPSULE(), Ret);
				if (hit) {
					auto Norm = Ret - Dest->GetSPHERE().m_Center;
					Norm.normalize();
					Norm *= GetCAPSULE().m_Radius;
					m_Pos = Ret + Norm;
				}
			}
			break;
			case CollType::typeCAPSULE:
			{
				CAPSULE SrcCap = GetCAPSULE();
				auto Center = SrcCap.GetCenter();
				CAPSULE DestCap = Dest->GetCAPSULE();
				Vec3 Ret1, Ret2;
				if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
					int count = 0;
					//接点へのベクトル
					float t;
					Vec3 SegPoint;
					HitTest::ClosetPtPointSegment(Ret1, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
					Vec3 span = SegPoint - Ret1;
					span.normalize();
					span *= 0.01f;
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
					m_Pos = Center;
				}
			}
			break;
			case CollType::typeOBB:
			{
				CAPSULE SrcCap = GetCAPSULE();
				auto Center = SrcCap.GetCenter();
				OBB DestObb = Dest->GetOBB();
				Vec3 Ret;
				if (HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
					int count = 0;
					//接点へのベクトル
					float t;
					bsm::Vec3 SegPoint;
					HitTest::ClosetPtPointSegment(Ret, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
					Vec3 span = SegPoint - Ret;
					span.normalize();
					span *= 0.01f;
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
					m_Pos = Center;
				}
			}
			break;
		}
	}

	void Rigidbody::EscapeOBB(Rigidbody* Dest) {
		switch (Dest->m_CollType) {
			case CollType::typeSPHERE:
			{
				OBB SrcObb = GetOBB();
				SPHERE DestSphere = Dest->GetSPHERE();
				Vec3 Ret;
				bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
				if (Ishit) {
					Vec3 NowSpan = Ret - DestSphere.m_Center;
					Vec3 NewSpan = NowSpan;
					NewSpan.normalize();
					NewSpan *= (DestSphere.m_Radius);
					auto MoveSpan = NewSpan - NowSpan;
					m_Pos += MoveSpan;
				}
			}
			break;
			case CollType::typeCAPSULE:
			{
				OBB SrcObb = GetOBB();
				auto Center = SrcObb.m_Center;
				CAPSULE DestCapsule = Dest->GetCAPSULE();
				Vec3 Ret;
				bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret);
				if (Ishit) {
					int count = 0;
					//接点へのベクトル
					float t;
					Vec3 SegPoint;
					HitTest::ClosetPtPointSegment(Ret, DestCapsule.m_PointBottom, DestCapsule.m_PointTop, t, SegPoint);
					Vec3 span = Ret - SegPoint;
					span.normalize();
					span *= 0.01f;
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
					m_Pos = Center;
				}
			}
			break;
			case CollType::typeOBB:
			{
				OBB SrcObb = GetOBB();
				OBB DestObb = Dest->GetOBB();
				bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
				if (Ishit) {
					bsm::Vec3 Ret;
					//SrcのOBBとDestの最近接点を得る
					HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, Ret);
					bsm::Vec3 span = SrcObb.m_Center - Ret;
					span.normalize();
					span *= 0.01f;
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
					m_Pos = Center;
				}
			}
			break;
		}
	}


	//--------------------------------------------------------------------------------------
	///	Rigidbodyマネージャ
	//--------------------------------------------------------------------------------------
	RigidbodyManager::RigidbodyManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	RigidbodyManager::~RigidbodyManager() {}

	//ターン毎の初期化
	void RigidbodyManager::InitRigidbody() {
		//1つ前の位置にセットとフォースの初期化
		for (auto& v : m_RigidbodyVec) {
			v.SetToBefore();
			v.m_Force = Vec3(0);
		}
	}



	//衝突判定
	bool RigidbodyManager::CollisionTest(Rigidbody& Src, Rigidbody& Dest, CollisionState& state) {
		if (Src.m_IsFixed) {
			return false;
		}
		if (CollisionStateChk(&Src, &Dest)) {
			return false;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		switch (Src.m_CollType) {
			case CollType::typeSPHERE:
				return Src.CollisionSPHERE(Dest, ElapsedTime, state);
			break;
			case CollType::typeCAPSULE:
				return Src.CollisionCAPSULE(Dest, ElapsedTime, state);
			break;
			case CollType::typeOBB:
				return Src.CollisionOBB(Dest, ElapsedTime, state);
			break;
		}
		return false;
	}

	bool RigidbodyManager::CollisionStateChk(Rigidbody* p1, Rigidbody* p2) {
		for (auto& v : m_CollisionStateVec) {
			if ((v.m_Src == p1 && v.m_Dest == p2) ||
				(v.m_Src == p2 && v.m_Dest == p1)) {
				return true;
			}
		}
		return false;
	}

	void RigidbodyManager::CollisionDest(Rigidbody& Src) {
		for (auto& v : m_RigidbodyVec) {
			if (Src.m_Owner.lock() != v.m_Owner.lock()) {
				CollisionState state;
				if (CollisionTest(Src, v, state)) {
					m_CollisionStateVec.push_back(state);
				}
			}
		}

	}

	void RigidbodyManager::OnUpdate() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//フォースから速度に変換
		for (auto& v : m_RigidbodyVec) {
			Vec3 accel = v.m_Force * v.m_Mass;
			v.m_Velocity += accel * ElapsedTime;
		}

		//衝突判定を行い、ヒットがあれば速度を変更する
		if (m_RigidbodyVec.size() >= 2) {
			//衝突判定
			for (auto& v : m_RigidbodyVec) {
				CollisionDest(v);
			}
		}
		if (m_CollisionStateVec.size() >= 2) {
			//ヒットタイムでソート（ヒットタイムが）近いものに対応
			auto func = [&](CollisionState& Left, CollisionState& Right)->bool {
				return (Left.m_HitTime < Right.m_HitTime);
			};
			std::sort(m_CollisionStateVec.begin(), m_CollisionStateVec.end(), func);
		}
		//衝突応答
		for (auto& v : m_CollisionStateVec) {
			if (!v.m_Src->m_IsFixed) {
				v.m_Src->Move(v.m_HitTime);
				v.m_Src->m_Velocity.slide(v.m_SrcHitNormal);
			}
			if (!v.m_Dest->m_IsFixed) {
				v.m_Dest->Move(v.m_HitTime);
				v.m_Dest->m_Velocity.slide(v.m_DestHitNormal);
			}
		}

		//設定された速度をもとに衝突無しのオブジェクトの位置の決定
		for (auto& v : m_RigidbodyVec) {
				v.Move(ElapsedTime);
		}

		//エスケープ処理
		for (auto& v : m_CollisionStateVec) {
			if (!v.m_Src->m_IsFixed) {
				Escape(v.m_Src, v.m_Dest);
			}
			if (!v.m_Dest->m_IsFixed) {
				Escape(v.m_Dest ,v.m_Src);
			}
		}


	}

	void RigidbodyManager::Escape(Rigidbody* Src, Rigidbody* Dest) {
		switch (Src->m_CollType) {
			case CollType::typeSPHERE:
				Src->EscapeSPHERE(Dest);
			break;
			case CollType::typeCAPSULE:
				Src->EscapeCAPSULE(Dest);
			break;
			case CollType::typeOBB:
				Src->EscapeOBB(Dest);
			break;
		}
	}


	void RigidbodyManager::OnUpdate2() {
		//衝突情報のクリア
		m_CollisionStateVec.clear();
	}


	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	GameStage::GameStage():
		m_LightDir(0.5f, -1.0f, 0.5f, 1.0f)
	{
		m_LightDir.normalize();
	}

	shared_ptr<GameObject> GameStage::GetPlayer() const {
		for (auto& v : GetGameObjectVec()) {
			if (v->FindTag(L"Player")) {
				auto shptr = dynamic_pointer_cast<GameObject>(v);
				if (shptr) {
					return shptr;
				}
			}
		}
		throw BaseException(
			L"オブジェクトが見つかりません",
			L"FindTag()",
			L"GameStage::GetPlayer()"
		);
		return nullptr;
	}

	shared_ptr<PNTDrawObject> GameStage::GetPNTDrawObject() const {
		for (auto& v : GetGameObjectVec()) {
			if (v->FindTag(L"PNTDrawObject")) {
				auto shptr = dynamic_pointer_cast<PNTDrawObject>(v);
				if (shptr) {
					return shptr;
				}
			}
		}
		throw BaseException(
			L"オブジェクトが見つかりません",
			L"FindTag()",
			L"GameStage::GetPNTDrawObject()"
		);
		return nullptr;
	}



	void GameStage::OnCreate() {
		//Rigidbodyマネージャの初期化
		m_RigidbodyManager 
			= ObjectFactory::Create<RigidbodyManager>(GetThis<GameStage>());

		//平面の作成
		Quat Qt;
		Qt.rotationX(XM_PIDIV2);
		AddGameObject<SquareObject>(
			L"SKY_TX",
			Vec3(50.0f, 50.0f, 1.0f),
			Qt,
			Vec3(0.0f, 0.0f, 0.0f)
			);


		//固定のボックス
		AddGameObject<BoxObject>(
			L"SKY_TX",
			false,
			Vec3(5.0f, 0.5f, 5.0f),
			Quat(),
			Vec3(5.0f, 0.25f, 0.0f)
			);

		//固定のボックス
		AddGameObject<BoxObject>(
			L"SKY_TX",
			false,
			Vec3(3.0f, 1.0f, 3.0f),
			Quat(),
			Vec3(5.0f, 0.5f, 0.0f)
			);

		//固定のボックス
		AddGameObject<BoxObject>(
			L"SKY_TX",
			false,
			Vec3(3.0f, 1.0f, 3.0f),
			Quat(),
			Vec3(-3.5f, 0.5f, 0.0f)
			);


		//固定のボックス
		AddGameObject<BoxObject>(
			L"SKY_TX",
			false,
			Vec3(5.0f, 0.5f, 5.0f),
			Quat(Vec3(0, 0, 1), -XM_PIDIV4),
			Vec3(-5.0f, 1.0f, 0.0f)
			);

		//固定のカプセル
		AddGameObject<CapsuleObject>(
			L"SKY_TX",
			false,
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(-5.0f, 0.0f, 10.0f)
			);

		//固定のカプセル
		AddGameObject<CapsuleObject>(
			L"SKY_TX",
			false,
			Vec3(2.0f, 2.0f, 2.0f),
			Quat(),
			Vec3(-5.0f, 0.0f, 15.0f)
			);


		//固定の球体
		AddGameObject<SphereObject>(
			L"SKY_TX",
			false,
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(5.0f, 0.0f, 10.0f)
			);

		//固定の球体
		AddGameObject<SphereObject>(
			L"SKY_TX",
			false,
			Vec3(2.0f, 2.0f, 2.0f),
			Quat(),
			Vec3(5.0f, 0.0f, 15.0f)
			);


		//プレイヤーの作成
		AddGameObject<Player>(
			L"TRACE_TX",
			true,
			Vec3(0.0f, 0.125f, 0.0f)
			);

		//カプセルプレイヤーの作成
		//AddGameObject<CapsulePlayer>(
		//	L"TRACE_TX",
		//	true,
		//	Vec3(0.0f, 0.25f, 0.0f)
		//	);

		//OBBプレイヤーの作成
		//AddGameObject<ObbPlayer>(
		//	L"TRACE_TX",
		//	true,
		//	Vec3(0.0f, 0.125f, 0.0f)
		//	);




		//PNT描画オブジェクトの作成
		AddGameObject<PNTDrawObject>();

		//回転するスプライトの作成
		AddGameObject<RotateSprite>(
			L"TRACE_TX",
			Vec2(160, 160),
			0.0f,
			Vec2(-480, 260),
			4, 4
			);


		//メッセージを表示するスプライトの作成
		AddGameObject<MessageSprite>(
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(480, 260),
			1, 1
			);

	}

	void GameStage::OnUpdateStage() {
		//ターン毎の初期化
		m_RigidbodyManager->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの更新
			v->OnUpdate();
		}
		//Rigidbodyマネージャの更新（衝突判定など）
		m_RigidbodyManager->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの最終更新
			v->OnUpdate2();
		}
		//自分自身の更新(カメラ)
		this->OnUpdate();
		//Rigidbodyマネージャの最終更新（衝突判定情報のクリア）
		m_RigidbodyManager->OnUpdate2();
	}


	void GameStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				m_Camera.m_CameraArmLen += 0.1f;
				if (m_Camera.m_CameraArmLen >= 50.0f) {
					m_Camera.m_CameraArmLen = 50.0f;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				m_Camera.m_CameraArmLen -= 0.1f;
				if (m_Camera.m_CameraArmLen <= 2.0f) {
					m_Camera.m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				m_Camera.m_CameraXZRad += CntlVec[0].fThumbRX * 0.02f;
				if (abs(m_Camera.m_CameraXZRad) >= XM_2PI) {
					m_Camera.m_CameraXZRad = 0;
				}
			}
			if (CntlVec[0].fThumbRY != 0) {
				m_Camera.m_CameraYRad -= CntlVec[0].fThumbRY * 0.02f;
				if (m_Camera.m_CameraYRad >= XM_PIDIV2 - 0.1f) {
					m_Camera.m_CameraYRad = XM_PIDIV2 - 0.1f;
				}
				else if (m_Camera.m_CameraYRad <= 0.2) {
					m_Camera.m_CameraYRad = 0.2;
				}
			}

			m_Camera.m_CamerAt = GetPlayer()->GetPosition();
			Vec3 CameraLocalEye =
				Vec3(
					sin(m_Camera.m_CameraXZRad) * m_Camera.m_CameraArmLen * sin(m_Camera.m_CameraYRad),
					cos(m_Camera.m_CameraYRad) * m_Camera.m_CameraArmLen,
					-cos(m_Camera.m_CameraXZRad) * m_Camera.m_CameraArmLen * sin(m_Camera.m_CameraYRad)
				);
			m_Camera.m_CamerEye = m_Camera.m_CamerAt + CameraLocalEye;
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
			}
		}
	}

	void GameStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの描画
			v->OnDraw();
		}
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void GameStage::OnDraw() {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//メッセージスプライト
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//スプライトの更新
		m_MessageSprite->OnUpdate();
		//自分自身の更新
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//スプライト描画
		m_MessageSprite->OnDraw();
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//何もしない
	}


}
//end basecross
