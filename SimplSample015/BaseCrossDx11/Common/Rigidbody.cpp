/*!
@file Rigidbody.h
@brief 物理計算コンポーネント実体
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct SteeringComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SteeringComponent::Impl {
		float m_Weight;
		Impl() :
			m_Weight(1.0f)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class SteeringComponent : public Component ;
	//	用途: 操舵コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	SteeringComponent::SteeringComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	SteeringComponent::~SteeringComponent() {}

	float SteeringComponent::GetWeight() const { return pImpl->m_Weight; }
	void SteeringComponent::SetWeight(float f) { pImpl->m_Weight = f; }




	//--------------------------------------------------------------------------------------
	//	struct SeekSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SeekSteering::Impl {
		Vector3 m_TargetPosition;
		Impl() :
			m_TargetPosition(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class SeekSteering : public SteeringComponent;
	//	用途: Seek操舵
	//--------------------------------------------------------------------------------------
	SeekSteering::SeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		if (PtrT) {
			//所持オブジェクトがあればその位置に初期化
			pImpl->m_TargetPosition = PtrT->GetPosition();
		}
	}
	SeekSteering::~SeekSteering() {}

	//アクセサ

	const Vector3& SeekSteering::GetTargetPosition() const {
		return pImpl->m_TargetPosition;
	}
	void SeekSteering::SetTargetPosition(const Vector3& Vec) {
		pImpl->m_TargetPosition = Vec;
	}
	void SeekSteering::SetTargetPosition(float x, float y, float z) {
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}
	//操作
	void SeekSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Seek(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
				TransPtr->GetPosition(), RigidPtr->GetMaxSpeed()) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ArriveSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ArriveSteering::Impl {
		Vector3 m_TargetPosition;
		float m_Decl;
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_Decl(3.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ArriveSteering : public SteeringComponent;
	//	用途: Arrive操舵
	//--------------------------------------------------------------------------------------
	ArriveSteering::ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ArriveSteering::~ArriveSteering() {}

	//アクセサ
	const Vector3& ArriveSteering::GetTargetPosition() const {
		return pImpl->m_TargetPosition;
	}
	void ArriveSteering::SetTargetPosition(const Vector3& Vec) {
		pImpl->m_TargetPosition = Vec;
	}
	void ArriveSteering::SetTargetPosition(float x, float y, float z) {
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	float ArriveSteering::GetDecl() const { return pImpl->m_Decl; }
	void ArriveSteering::SetDecl(float f) { pImpl->m_Decl = f; }


	//操作
	void ArriveSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Arrive(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
				TransPtr->GetPosition(), RigidPtr->GetMaxSpeed(), pImpl->m_Decl) *  GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PursuitSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct PursuitSteering::Impl {
		Vector3 m_TargetPosition;	//目標の位置
		Quaternion m_TargetQuaternion;		//目標の向き
		Vector3 m_TargetVelocity;	//目標の速度
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_TargetQuaternion(),
			m_TargetVelocity(0, 0, 0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class PursuitSteering : public SteeringComponent;
	//	用途: Pursuit操舵
	//--------------------------------------------------------------------------------------
	PursuitSteering::PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	PursuitSteering::~PursuitSteering() {}

	//アクセサ
	const Vector3& PursuitSteering::GetTargetPosition() const {
		return pImpl->m_TargetPosition;
	}
	void PursuitSteering::SetTargetPosition(const Vector3& Vec) {
		pImpl->m_TargetPosition = Vec;
	}
	void PursuitSteering::SetTargetPosition(float x, float y, float z) {
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	const Quaternion& PursuitSteering::GetTargetQuaternion() const {
		return pImpl->m_TargetQuaternion;
	}
	void PursuitSteering::SetTargetQuaternion(const Quaternion& Qt) {
		pImpl->m_TargetQuaternion = Qt;
	}
	void PursuitSteering::SetTargetRotation(const Vector3& rotation) {
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(rotation);
	}
	void PursuitSteering::SetTargetRotation(float x, float y, float z) {
		SetTargetRotation(Vector3(x, y, z));
	}
	const Vector3& PursuitSteering::GetTargetVelocity() const {
		return pImpl->m_TargetVelocity;
	}
	void PursuitSteering::SetTargetVelocity(const Vector3& velocity) {
		pImpl->m_TargetVelocity = velocity;
	}
	void PursuitSteering::SetTargetVelocity(float x, float y, float z) {
		SetTargetVelocity(Vector3(x, y, z));
	}

	//操作
	void PursuitSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Pursuit(RigidPtr->GetVelocity(), TransPtr->GetPosition(), TransPtr->GetRotation(), RigidPtr->GetMaxSpeed(),
				pImpl->m_TargetVelocity, pImpl->m_TargetPosition, pImpl->m_TargetQuaternion.GetRotation()) *  GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WanderSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct WanderSteering::Impl {
		Vector3 m_WanderTarget;	//徘徊目標の回転係数（操舵によって書き換えられる）
		float m_WanderRadius;	//徘徊半径
		float m_WanderDistance;	//徘徊円までの距離
		float m_WanderJitter;	//ランダム変異の最大値
		Impl() :
			m_WanderTarget(0, 0, 0),
			m_WanderRadius(1.5f),
			m_WanderDistance(1.0f),
			m_WanderJitter(0.5f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class WanderSteering : public SteeringComponent;
	//	用途: Wander操舵
	//--------------------------------------------------------------------------------------
	WanderSteering::WanderSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WanderSteering::~WanderSteering() {}
	//アクセサ
	const Vector3& WanderSteering::GetWanderTarget() const {
		return pImpl->m_WanderTarget;
	}
	void WanderSteering::SetWanderTarget(const Vector3& target) {
		pImpl->m_WanderTarget = target;
	}
	float WanderSteering::GetWanderRadius() const {
		return pImpl->m_WanderRadius;
	}
	void WanderSteering::SetWanderRadius(float f) {
		pImpl->m_WanderRadius = f;
	}
	float WanderSteering::GetWanderDistance() const {
		return pImpl->m_WanderDistance;
	}
	void WanderSteering::SetWanderDistance(float f) {
		pImpl->m_WanderDistance = f;
	}
	float WanderSteering::GetWanderJitter() const {
		return pImpl->m_WanderJitter;
	}
	void WanderSteering::SetWanderJitter(float f) {
		pImpl->m_WanderJitter = f;
	}

	void WanderSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Wander(TransPtr->GetWorldMatrix(),
				pImpl->m_WanderRadius, pImpl->m_WanderDistance, pImpl->m_WanderJitter, pImpl->m_WanderTarget) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WallAvoidanceSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct WallAvoidanceSteering::Impl {
		vector<PLANE> m_PlaneVec;//回避すべき面の配列
		bool m_WallArrived;	//壁と衝突したか
		Impl() :
			m_WallArrived(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class WallAvoidanceSteering : public SteeringComponent;
	//	用途: WallAvoidance操舵(壁回避)
	//--------------------------------------------------------------------------------------
	WallAvoidanceSteering::WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WallAvoidanceSteering::~WallAvoidanceSteering() {}

	//アクセサ
	vector<PLANE>& WallAvoidanceSteering::GetPlaneVec() const {
		return pImpl->m_PlaneVec;
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<PLANE>& planevec) {
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec) {
			pImpl->m_PlaneVec.push_back(v);
		}
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<Plane>& planevec) {
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec) {
			PLANE p(v);
			pImpl->m_PlaneVec.push_back(p);
		}
	}


	//壁と衝突しているか
	bool WallAvoidanceSteering::IsWallArribed() const {
		return pImpl->m_WallArrived;
	}


	//操作
	void WallAvoidanceSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		pImpl->m_WallArrived = false;
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce(0, 0, 0);
			WorkForce = Steering::WallAvoidance(TransPtr->GetWorldMatrix(),
				RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_PlaneVec) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
			if (WorkForce.Length() > 0.0f) {
				//壁に衝突している
				pImpl->m_WallArrived = true;
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ObstacleAvoidanceSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ObstacleAvoidanceSteering::Impl {
		vector<SPHERE> m_ObstacleSphereVec;		//回避すべき障害物のSPHERE配列
		float m_RoadWidth;
		float m_RoadHeight;
		Impl() :
			m_RoadWidth(0.5f),
			m_RoadHeight(0.5f)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class ObstacleAvoidanceSteering : public SteeringComponent;
	//	用途: ObstacleAvoidance操舵(障害物回避)
	//--------------------------------------------------------------------------------------
	ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering() {}
	//アクセサ
	//障害物の配列
	const vector<SPHERE>& ObstacleAvoidanceSteering::GetObstacleSphereVec() const {
		return pImpl->m_ObstacleSphereVec;
	}
	void ObstacleAvoidanceSteering::SetObstacleSphereVec(const vector<SPHERE>& spherevec) {
		pImpl->m_ObstacleSphereVec.clear();
		for (auto sp : spherevec) {
			pImpl->m_ObstacleSphereVec.push_back(sp);
		}
	}

	//道の高さ
	float ObstacleAvoidanceSteering::GetRoadWidth() const {
		return pImpl->m_RoadWidth;
	}
	void ObstacleAvoidanceSteering::SetRoadWidth(float f) {
		pImpl->m_RoadWidth = f;
	}
	//道幅
	float ObstacleAvoidanceSteering::GetRoadHeight() const {
		return pImpl->m_RoadHeight;
	}
	void ObstacleAvoidanceSteering::SetRoadHeight(float f) {
		pImpl->m_RoadHeight = f;
	}


	void ObstacleAvoidanceSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce(0, 0, 0);
			WorkForce = Steering::ObstacleAvoidance(TransPtr->GetWorldMatrix(),
				RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_RoadWidth, pImpl->m_RoadHeight,
				pImpl->m_ObstacleSphereVec) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct FollowPathSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct FollowPathSteering::Impl {
		Path m_Path;	//経路をあらわすパス
		float m_Decl;	//減速値
		float m_WaypointSpan;	//経路の中心からの距離
		Impl() :
			m_Decl(3.0f),
			m_WaypointSpan(2.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class FollowPathSteering : public SteeringComponent;
	//	用途: FollowPath操舵(経路追従)
	//--------------------------------------------------------------------------------------
	FollowPathSteering::FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	FollowPathSteering::~FollowPathSteering() {}

	//アクセサ
	void FollowPathSteering::SetPathList(const list<Vector3>& pathlist) {
		pImpl->m_Path.SetList(pathlist);
	}

	float FollowPathSteering::GetDecl() const {
		return pImpl->m_Decl;
	}
	void FollowPathSteering::SetDecl(float f) {
		pImpl->m_Decl = f;
	}

	float FollowPathSteering::GetWaypointSpan() const {
		return pImpl->m_WaypointSpan;
	}
	void FollowPathSteering::SetWaypointSpan(float f) {
		pImpl->m_WaypointSpan = f;
	}

	bool FollowPathSteering::GetLooped() const {
		return pImpl->m_Path.GetLooped();
	}
	bool FollowPathSteering::IsLooped() const {
		return pImpl->m_Path.GetLooped();
	}
	void FollowPathSteering::SetLooped(bool b) {
		pImpl->m_Path.SetLooped(b);
	}

	bool FollowPathSteering::IsFinished() const {
		return pImpl->m_Path.IsFinished();
	}

	//操作
	void FollowPathSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (RigidPtr && TransPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce(0, 0, 0);
			WorkForce = Steering::FollowPath(pImpl->m_Path,
				pImpl->m_WaypointSpan,
				TransPtr->GetPosition(),
				RigidPtr->GetVelocity(),
				RigidPtr->GetMaxSpeed(),
				pImpl->m_Decl)
				* GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct AlignmentSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct AlignmentSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class AlignmentSteering : public SteeringComponent;
	//	用途: Alignment操舵(整列)
	//--------------------------------------------------------------------------------------
	AlignmentSteering::AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	AlignmentSteering::~AlignmentSteering() {}

	//アクセサ
	shared_ptr<GameObjectGroup> AlignmentSteering::GetGameObjectGroup() const {
		if (pImpl->m_Group.expired()) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"AlignmentSteering::GetGameObjectGroup()"
			);
		}
		return pImpl->m_Group.lock();
	}
	void AlignmentSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}
	//操作
	void AlignmentSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (TransPtr && RigidPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Alignment(GetGameObjectGroup(), GetGameObject()) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct CohesionSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CohesionSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CohesionSteering : public SteeringComponent;
	//	用途: Cohesion操舵(結合)
	//--------------------------------------------------------------------------------------
	CohesionSteering::CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	CohesionSteering::~CohesionSteering() {}

	//アクセサ
	shared_ptr<GameObjectGroup> CohesionSteering::GetGameObjectGroup() const {
		if (pImpl->m_Group.expired()) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"CohesionSteering::GetGameObjectGroup()"
			);
		}
		return pImpl->m_Group.lock();
	}
	void CohesionSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}

	//操作
	void CohesionSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (TransPtr && RigidPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Cohesion(GetGameObjectGroup(), GetGameObject(),
				RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed()) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SeparationSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SeparationSteering::Impl {
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class SeparationSteering : public SteeringComponent;
	//	用途: Separation操舵
	//--------------------------------------------------------------------------------------
	SeparationSteering::SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	SeparationSteering::~SeparationSteering() {}
	//アクセサ
	shared_ptr<GameObjectGroup> SeparationSteering::GetGameObjectGroup() const {
		if (pImpl->m_Group.expired()) {
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"SeparationSteering::GetGameObjectGroup()"
			);
		}
		return pImpl->m_Group.lock();
	}
	void SeparationSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_Group = Group;
	}

	void SeparationSteering::OnUpdate() {
		auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (TransPtr && RigidPtr) {
			Vector3 Force = RigidPtr->GetForce();
			Vector3 WorkForce;
			WorkForce = Steering::Separation(GetGameObjectGroup(), GetGameObject()) * GetWeight();
			Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
			RigidPtr->SetForce(Force);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct Rigidbody::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Rigidbody::Impl {
		Vector3 m_Velocity;				//速度
		float m_MaxSpeed;				//最高速度
		Vector3 m_MinVelocity;			//最低速度（XYZを指定できる）
		Vector3 m_AngularVelocity;		//回転速度

		float m_Mass;					//質量（キログラム）
		float m_Reflection;				//反発係数
		Vector3 m_Force;				//現在のフォース（ステアリング系コンポーネントで変更される）
		float m_FrictionForce;			//減速のフォース（速度の逆向きに働く）
		float m_MaxForce;				//最高フォース
		Impl() :
			m_Velocity(0, 0, 0),
			m_MaxSpeed(10.0f),
			m_MinVelocity(0.01f, 0.01f, 0.01f),
			m_AngularVelocity(0, 0, 0),
			m_Mass(1.0f),
			m_Reflection(1.0f),
			m_Force(0, 0, 0),
			m_FrictionForce(0.0f),
			m_MaxForce(30.0f)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	用途: 自動位置コントロール
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Rigidbody::Rigidbody(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Rigidbody::~Rigidbody() {}

	const Vector3& Rigidbody::GetVelocity() const { return pImpl->m_Velocity; }
	void Rigidbody::SetVelocity(const Vector3& Velocity) {
		pImpl->m_Velocity = Velocity;
		pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		if (abs(pImpl->m_Velocity.x) < abs(pImpl->m_MinVelocity.x)) {
			pImpl->m_Velocity.x = 0;
		}
		if (abs(pImpl->m_Velocity.y) < abs(pImpl->m_MinVelocity.y)) {
			pImpl->m_Velocity.y = 0;
		}
		if (abs(pImpl->m_Velocity.z) < abs(pImpl->m_MinVelocity.z)) {
			pImpl->m_Velocity.z = 0;
		}
	}
	void Rigidbody::SetVelocity(float x, float y, float z) {
		SetVelocity(Vector3(x, y, z));
	}

	float Rigidbody::GetMaxSpeed() const { return pImpl->m_MaxSpeed; }
	void Rigidbody::SetMaxSpeed(float f) { pImpl->m_MaxSpeed = f; }

	const Vector3& Rigidbody::GetMinVelocity() const {
		return pImpl->m_MinVelocity;
	}
	void Rigidbody::SetMinVelocity(const Vector3& Velocity) {
		pImpl->m_MinVelocity = Velocity;
	}
	void Rigidbody::SetMinVelocity(float x, float y, float z) {
		SetMinVelocity(Vector3(x, y, z));
	}


	const Vector3& Rigidbody::GetAngularVelocity() const { return pImpl->m_AngularVelocity; }
	void Rigidbody::SetAngularVelocity(const Vector3& AngularVelocity) { pImpl->m_AngularVelocity = AngularVelocity; }
	void Rigidbody::SetAngularVelocity(float x, float y, float z) { pImpl->m_AngularVelocity = Vector3(x, y, z); }


	float Rigidbody::GetMass() const { return pImpl->m_Mass; }
	void Rigidbody::SetMass(float f) {
		if (f <= 0) {
			throw BaseException(
				L"質量は0以下は設定できません。",
				L"if (f <= 0) ",
				L"Rigidbody::SetMass()"
			);
		}
		pImpl->m_Mass = f; 
	}

	float Rigidbody::GetReflection() const {
		return pImpl->m_Reflection;
	}
	void Rigidbody::SetReflection(float f) {
		pImpl->m_Reflection = f;
	}

	const Vector3& Rigidbody::GetForce() const { return pImpl->m_Force; }
	void Rigidbody::SetForce(const Vector3& Force) { pImpl->m_Force = Force; }
	void Rigidbody::SetForce(float x, float y, float z) { pImpl->m_Force = Vector3(x, y, z); }

	float Rigidbody::GetFrictionForce() const {
		return pImpl->m_FrictionForce;
	}
	void Rigidbody::SetFrictionForce(float f) {
		pImpl->m_FrictionForce = f;
	}


	float Rigidbody::GetMaxForce() const { return pImpl->m_MaxForce; }
	void Rigidbody::SetMaxForce(float f) { pImpl->m_MaxForce = f; }

	void Rigidbody::UpdateFromTime(float CalcTime) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = PtrTransform->GetPosition();
		Quaternion Qt = PtrTransform->GetQuaternion();
		//フォースが変更されていたら
		if (pImpl->m_Force.Length() > 0) {
			//フォースが0以上なら
			pImpl->m_Force.ClampLength(0, pImpl->m_MaxForce);
			//質量を計算して加速を求める
			Vector3 Accel = pImpl->m_Force / pImpl->m_Mass;
			pImpl->m_Velocity += Accel * CalcTime;
		}

		//摩擦加速を求める
		Vector3 FrictionForce = pImpl->m_Velocity * -1.0f;
		FrictionForce *= pImpl->m_FrictionForce;
		Vector3 FrictionAccel = FrictionForce / pImpl->m_Mass;
		pImpl->m_Velocity += FrictionAccel * CalcTime;

		pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		if (abs(pImpl->m_Velocity.x) < abs(pImpl->m_MinVelocity.x)) {
			pImpl->m_Velocity.x = 0;
		}
		if (abs(pImpl->m_Velocity.y) < abs(pImpl->m_MinVelocity.y)) {
			pImpl->m_Velocity.y = 0;
		}
		if (abs(pImpl->m_Velocity.z) < abs(pImpl->m_MinVelocity.z)) {
			pImpl->m_Velocity.z = 0;
		}
		Quaternion QtSpan;
		QtSpan.RotationRollPitchYawFromVector(pImpl->m_AngularVelocity * CalcTime);
		Qt *= QtSpan;
		Qt.Normalize();
		Pos += pImpl->m_Velocity * CalcTime;
		PtrTransform->SetQuaternion(Qt);
		PtrTransform->SetPosition(Pos);
	}





	void Rigidbody::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

}
//end basecross
