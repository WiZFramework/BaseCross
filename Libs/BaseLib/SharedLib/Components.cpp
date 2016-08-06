/*!
@file Components.cpp
@brief コンポーネント実体
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Component::Impl {
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//updateするかどうか
		bool m_DrawActive{ true };		//Drawするかどうか
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	用途: コンポーネント基底クラス
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component() {}
	//アクセサ
	shared_ptr<GameObject> Component::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (!shptr)",
				L"Component::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}

	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr) {
		pImpl->m_GameObject = GameObjectPtr;
	}
	shared_ptr<Stage> Component::GetStage() const {
		return GetGameObject()->GetStage();
	}


	bool Component::IsUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	bool Component::GetUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	void Component::SetUpdateActive(bool b) {
		pImpl->m_UpdateActive = b;
	}

	bool Component::IsDrawActive() const {
		return pImpl->m_DrawActive;
	}
	bool Component::GetDrawActive() const {
		return pImpl->m_DrawActive;
	}
	void Component::SetDrawActive(bool b) {
		pImpl->m_DrawActive = b;
	}

	//--------------------------------------------------------------------------------------
	//	struct Transform::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Transform::Impl {
		bool m_Init{ false };	//初期化済みかどうか（1回目のUpdateで、Beforeに値を入れる）
		//1つ前の変数
		Vector3 m_BeforeScale;
		Vector3 m_BeforePivot;
		Quaternion m_BeforeQuaternion;
		Vector3 m_BeforePosition;
		//現在の変数
		Vector3 m_Scale;
		Vector3 m_Pivot;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	用途: 変化
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform() {}

	//アクセサ
	//BeforeGetter
	Vector3 Transform::GetBeforeScale() const {
		return pImpl->m_BeforeScale;
	}

	Vector3 Transform::GetBeforePivot() const{
		return pImpl->m_BeforePivot;
	}

	Quaternion Transform::GetBeforeQuaternion() const {
		return pImpl->m_BeforeQuaternion;
	}

	Vector3 Transform::GetBeforeRotation() const {
		return pImpl->m_BeforeQuaternion.GetRotation();
	}

	Vector3 Transform::GetBeforePosition() const {
		return pImpl->m_BeforePosition;
	}

	Matrix4X4 Transform::GetBeforeWorldMatrix() const{
		Matrix4X4 mat;
		mat.AffineTransformation(pImpl->m_BeforeScale,
			pImpl->m_BeforePivot, 
			pImpl->m_BeforeQuaternion, 
			pImpl->m_BeforePosition
		);
		return mat;
	}


	//Getter&Setter

	Vector3 Transform::GetScale() const {
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const Vector3& Scale) {
		pImpl->m_Scale = Scale;
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(Vector3(x, y, z));
	}

	Vector3 Transform::GetPivot() const {
		return pImpl->m_Pivot;
	}
	void Transform::SetPivot(const Vector3& Pivot) {
		pImpl->m_Pivot = Pivot;
	}
	void Transform::SetPivot(float x, float y, float z) {
		SetPivot(Vector3(x, y, z));
	}

	Quaternion Transform::GetQuaternion() const {
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const Quaternion& quaternion) {
		pImpl->m_Quaternion = quaternion;
		pImpl->m_Quaternion.Normalize();
	}
	Vector3 Transform::GetRotation() const {
		return pImpl->m_Quaternion.GetRotation();
	}

	void Transform::SetRotation(const Vector3& Rot) {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetQuaternion(Qt);
	}
	void Transform::SetRotation(float x, float y, float z) {
		SetRotation(Vector3(x, y, z));
	}

	Vector3 Transform::GetPosition() const {
		return pImpl->m_Position;
	}

	void Transform::SetPosition(const Vector3& Position) {
		pImpl->m_Position = Position;
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(Vector3(x, y, z));
	}
	Matrix4X4 Transform::GetWorldMatrix() const{
		Matrix4X4 mat;
		mat.AffineTransformation(
			pImpl->m_Scale,
			pImpl->m_Pivot,
			pImpl->m_Quaternion,
			pImpl->m_Position
		);
		return mat;
	}

	void Transform::SetToBefore() {
		pImpl->m_BeforeScale = pImpl->m_Scale;
		pImpl->m_BeforePivot = pImpl->m_Pivot;
		pImpl->m_BeforeQuaternion = pImpl->m_Quaternion;
		pImpl->m_BeforePosition = pImpl->m_Position;
	}



	//操作
	void Transform::OnUpdate() {
		if (!pImpl->m_Init) {
			SetToBefore();
			pImpl->m_Init = true;
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ActionComponent::Impl {
		bool m_Run;			//移動中かどうか
		bool m_Arrived;	//到着したかどうか
		float m_TotalTime;	//移動にかける時間
		float m_NowTime;	//現在の時間
		Impl() :
			m_Run(false),
			m_Arrived(false),
			m_TotalTime(0),
			m_NowTime(0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	用途: アクション系コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionComponent::ActionComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	ActionComponent::~ActionComponent() {}

	//アクセサ
	void ActionComponent::SetRun(bool b) {
		pImpl->m_Run = b;
	}
	void  ActionComponent::SetArrived(bool b) {
		pImpl->m_Arrived = b;
	}


	bool ActionComponent::IsRun()const {
		return pImpl->m_Run;
	}
	bool ActionComponent::GetRun()const {
		return pImpl->m_Run;
	}

	bool ActionComponent::IsArrived()const {
		return pImpl->m_Arrived;
	}
	bool ActionComponent::GetArrived()const {
		return pImpl->m_Arrived;
	}

	float ActionComponent::GetTotalTime() const {
		return pImpl->m_TotalTime;
	}
	void ActionComponent::SetTotalTime(float f) {
		pImpl->m_TotalTime = f;
	}

	float ActionComponent::GetNowTime() const {
		return pImpl->m_NowTime;
	}
	void ActionComponent::SetNowTime(float f) {
		pImpl->m_NowTime = f;
	}

	//NowTimeを加算して、TotalTimeと比較する
	bool ActionComponent::AdditionalNowTime() {
		if (pImpl->m_Run) {
			if (pImpl->m_TotalTime > 0) {
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime) {
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//到着した
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}


	void ActionComponent::Stop() {
		pImpl->m_Run = false;
	}
	void ActionComponent::ReStart() {
		if (!pImpl->m_Arrived) {
			//到着してなければ動作させる
			pImpl->m_Run = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	用途: アクションのインターバル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionInterval::ActionInterval(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr)
	{}
	ActionInterval::~ActionInterval() {}

	void ActionInterval::SetParams(float TotalTime) {
		SetTotalTime(TotalTime);
	}

	void ActionInterval::Run() {
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//トータルタイムが0以下なら、すでに到着していることになる
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}
	void ActionInterval::OnUpdate() {
		if (GetRun()) {
			if (!AdditionalNowTime()) {
				if (GetTotalTime() <= 0) {
					//トータルタイムが0以下なら、すでに到着していることになる
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ScaleComponent::Impl {
		Vector3 m_StartScale;	//開始倍率
		Vector3 m_TargetScale;	//目的倍率
		Lerp::rate m_Rate;	//補間方法
		Impl() :
			m_StartScale(1.0f, 1.0f, 1.0f),
			m_TargetScale(1.0f, 1.0f, 1.0f),
			m_Rate(Lerp::Linear)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public Component ;
	//	用途: 拡大縮小アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ScaleComponent::ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ScaleComponent::~ScaleComponent() {}


	//アクセサ
	Lerp::rate ScaleComponent::GetRate() const { return pImpl->m_Rate; }
	void ScaleComponent::SetRate(const Lerp::rate r) { pImpl->m_Rate = r; }

	const Vector3& ScaleComponent::GetStartScale() const { return pImpl->m_StartScale; }
	void ScaleComponent::SetStartScale(const Vector3& StartPosition) { pImpl->m_StartScale = StartPosition; }
	void ScaleComponent::SetStartScale(float x, float y, float z) { pImpl->m_StartScale = Vector3(x, y, z); }

	const Vector3& ScaleComponent::GetTargetScale() const { return pImpl->m_TargetScale; }
	void ScaleComponent::SetTargetScale(const Vector3& TargetScale) { pImpl->m_TargetScale = TargetScale; }
	void ScaleComponent::SetTargetScale(float x, float y, float z) { pImpl->m_TargetScale = Vector3(x, y, z); }

	//操作
	void ScaleComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		pImpl->m_StartScale = TransPtr->GetScale();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//トータルタイムが0以下なら、すでに到着していることになる
			TransPtr->SetScale(pImpl->m_TargetScale);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}

	void ScaleComponent::OnUpdate() {
		if (GetRun()) {
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				Vector3 Scale = Lerp::CalculateLerp<Vector3>(
					pImpl->m_StartScale,
					pImpl->m_TargetScale,
					0,
					GetTotalTime(),
					GetNowTime(),
					pImpl->m_Rate
					);
				TransPtr->SetScale(Scale);
			}
			else {
				if (GetTotalTime() <= 0) {
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//現在スケールを計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 ScaleComponent::GetNowScale() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetScale;
		}
		Vector3 Scale = Lerp::CalculateLerp(
			pImpl->m_StartScale,
			pImpl->m_TargetScale,
			0,
			GetTotalTime(),
			GetNowTime(),
			pImpl->m_Rate
		);
		return Scale;
	}



	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	用途: 目的の拡大率に拡大縮小
	//--------------------------------------------------------------------------------------
	ScaleTo::ScaleTo(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr) {}
	ScaleTo::~ScaleTo() {}
	void ScaleTo::SetParams(float TotalTime, const Vector3& TargetScale, Lerp::rate Rate) {
		SetTargetScale(TargetScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ScaleBy::Impl {
		Vector3 m_LocalScale;	//相対スケール
	public:
		Impl() :
			m_LocalScale(1.0f, 1.0f, 1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	用途: 目的の相対スケールにに拡大縮小
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ScaleBy::ScaleBy(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr), pImpl(new Impl()) {}
	ScaleBy::~ScaleBy() {}


	//アクセサ
	const Vector3& ScaleBy::GetLocalScale() const { return pImpl->m_LocalScale; }
	void ScaleBy::SetLocalScale(const Vector3& LocalScale) { pImpl->m_LocalScale = LocalScale; }
	void ScaleBy::SetLocalScale(float x, float y, float z) { pImpl->m_LocalScale = Vector3(x, y, z); }

	void ScaleBy::SetParams(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate) {
		SetLocalScale(LocalScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}


	//操作
	void ScaleBy::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Vector3 Target = TransPtr->GetScale();
		Target.x *= pImpl->m_LocalScale.x;
		Target.y *= pImpl->m_LocalScale.y;
		Target.z *= pImpl->m_LocalScale.z;
		SetTargetScale(Target);
		ScaleComponent::Run();
	}



	//--------------------------------------------------------------------------------------
	//	struct RotateComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct RotateComponent::Impl {
		Quaternion m_StartQuaternion;	//開始回転
		Quaternion m_TargetQuaternion;	//終了回転
	public:
		Impl() :
			m_StartQuaternion(),
			m_TargetQuaternion()
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	用途: 回転アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateComponent::RotateComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{
	}
	RotateComponent::~RotateComponent() {}


	//アクセサ
	const Quaternion& RotateComponent::GetStartQuaternion() const { return pImpl->m_StartQuaternion; }
	void RotateComponent::SetStartQuaternion(const Quaternion& StartQuaternion) {
		pImpl->m_StartQuaternion = StartQuaternion;
		pImpl->m_StartQuaternion.Normalize();
	}

	Vector3 RotateComponent::GetStartRotate() const {
		return pImpl->m_StartQuaternion.GetRotation();
	}
	void RotateComponent::SetStartRotate(const Vector3& StartRotate) {
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(StartRotate);
		pImpl->m_StartQuaternion.Normalize();
	}
	void RotateComponent::SetStartRotate(float x, float y, float z) {
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_StartQuaternion.Normalize();
	}

	const Quaternion& RotateComponent::GetTargetQuaternion() const {
		return pImpl->m_TargetQuaternion;
	}
	void RotateComponent::SetTargetQuaternion(const Quaternion& TargetQuaternion) {
		pImpl->m_TargetQuaternion = TargetQuaternion;
		pImpl->m_TargetQuaternion.Normalize();
	}
	Vector3 RotateComponent::GetTargetRotate() const {
		return pImpl->m_TargetQuaternion.GetRotation();
	}
	void RotateComponent::SetTargetRotate(const Vector3& TargetRotate) {
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(TargetRotate);
		pImpl->m_TargetQuaternion.Normalize();
	}
	void RotateComponent::SetTargetRotate(float x, float y, float z) {
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_TargetQuaternion.Normalize();
	}

	//操作
	void RotateComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//クオータニオンから現在の回転を得る
		pImpl->m_StartQuaternion = TransPtr->GetQuaternion();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//トータルタイムが0以下なら、すでに到着していることになる
			TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
	}
	void RotateComponent::OnUpdate() {
		if (GetRun()) {
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				Quaternion Qt = QuaternionEX::Slerp(
					pImpl->m_StartQuaternion,
					pImpl->m_TargetQuaternion,
					GetNowTime() / GetTotalTime()
				);
				Qt.Normalize();
				TransPtr->SetQuaternion(Qt);
			}
			else {
				if (GetTotalTime() <= 0) {
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//現在回転を計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 RotateComponent::GetNowRotate() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetQuaternion.GetRotation();
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
		);
		Qt.Normalize();
		return Qt.GetRotation();
	}
	Quaternion RotateComponent::GetNowQuaternion() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetQuaternion;
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
		);
		Qt.Normalize();
		return Qt;
	}



	//--------------------------------------------------------------------------------------
	//	class RotateTo : public  MoveComponent ;
	//	用途: 目的の角度に回転
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateTo::RotateTo(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr)
	{}
	RotateTo::~RotateTo() {}

	void RotateTo::SetParams(float TotalTime, const Vector3& TargetRotate) {
		SetTargetRotate(TargetRotate);
		SetTotalTime(TotalTime);
	}

	void RotateTo::SetParams(float TotalTime, const Quaternion& TargetQuaternion) {
		SetTargetQuaternion(TargetQuaternion);
		SetTotalTime(TotalTime);
	}


	//--------------------------------------------------------------------------------------
	//	struct RotateBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct RotateBy::Impl {
		Quaternion m_LocalQuaternion;	//相対回転
		Impl() :
			m_LocalQuaternion()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	用途: 目的の相対角度に回転
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateBy::RotateBy(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr), pImpl(new Impl()) {}
	RotateBy::~RotateBy() {}

	//アクセサ
	const Quaternion& RotateBy::GetLocalQuaternion() const { return pImpl->m_LocalQuaternion; }
	void RotateBy::SetLocalQuaternion(const Quaternion& LocalQuaternion) {
		pImpl->m_LocalQuaternion = LocalQuaternion;
		pImpl->m_LocalQuaternion.Normalize();
	}
	Vector3 RotateBy::GetLocalRotate() const {
		return pImpl->m_LocalQuaternion.GetRotation();
	}
	void RotateBy::SetLocalRotate(const Vector3& LocalRotate) {
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(LocalRotate);
		pImpl->m_LocalQuaternion.Normalize();
	}
	void RotateBy::SetLocalRotate(float x, float y, float z) {
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_LocalQuaternion.Normalize();
	}

	void RotateBy::SetParams(float TotalTime, const Vector3& LocalRotate) {
		SetTotalTime(TotalTime);
		SetLocalRotate(LocalRotate);
	}

	void RotateBy::SetParams(float TotalTime, const Quaternion& LocalQuaternion) {
		SetTotalTime(TotalTime);
		SetLocalQuaternion(LocalQuaternion);
	}


	//操作
	void RotateBy::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		Quaternion QtNow = TransPtr->GetQuaternion();
		Quaternion Target = QtNow * pImpl->m_LocalQuaternion;
		SetTargetQuaternion(Target);
		RotateComponent::Run();
	}

	//--------------------------------------------------------------------------------------
	//	struct MoveComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MoveComponent::Impl {
		Vector3 m_StartPosition;	//開始地点
		Vector3 m_TargetPosition;	//目的地点
		Lerp::rate m_RateX;	//補間方法X
		Lerp::rate m_RateY;	//補間方法Y
		Lerp::rate m_RateZ;	//補間方法Z
		Vector3 m_Velocity;	//現在の速度
		Impl() :
			m_StartPosition(0, 0, 0),
			m_TargetPosition(0, 0, 0),
			m_RateX(Lerp::Linear),
			m_RateY(Lerp::Linear),
			m_RateZ(Lerp::Linear),
			m_Velocity(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	用途: 移動アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveComponent::MoveComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	MoveComponent::~MoveComponent() {}
	//アクセサ

	Lerp::rate MoveComponent::GetRateX() const {
		return pImpl->m_RateX;
	}
	Lerp::rate MoveComponent::GetRateY() const {
		return pImpl->m_RateY;
	}
	Lerp::rate MoveComponent::GetRateZ() const {
		return pImpl->m_RateZ;
	}
	void MoveComponent::SetRateX(const Lerp::rate r) {
		pImpl->m_RateX = r;
	}
	void MoveComponent::SetRateY(const Lerp::rate r) {
		pImpl->m_RateY = r;
	}
	void MoveComponent::SetRateZ(const Lerp::rate r) {
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate r) {
		pImpl->m_RateX = r;
		pImpl->m_RateY = r;
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate rX, const Lerp::rate rY, const Lerp::rate rZ) {
		pImpl->m_RateX = rX;
		pImpl->m_RateY = rY;
		pImpl->m_RateZ = rZ;
	}

	Vector3 MoveComponent::GetVelocity()const {
		Vector3 Ret = pImpl->m_Velocity;
		if (GetTotalTime() <= 0) {
			Ret.Zero();
		}
		else {
			if (!GetRun() || GetArrived()) {
				Ret.Zero();
			}
		}
		return Ret;
	}



	const Vector3& MoveComponent::GetStartPosition() const { return pImpl->m_StartPosition; }
	void MoveComponent::SetStartPosition(const Vector3& StartPosition) { pImpl->m_StartPosition = StartPosition; }
	void MoveComponent::SetStartPosition(float x, float y, float z) { pImpl->m_StartPosition = Vector3(x, y, z); }

	const Vector3& MoveComponent::GetTargetPosition() const { return pImpl->m_TargetPosition; }
	void MoveComponent::SetTargetPosition(const Vector3& TargetPosition) { pImpl->m_TargetPosition = TargetPosition; }
	void MoveComponent::SetTargetPosition(float x, float y, float z) { pImpl->m_TargetPosition = Vector3(x, y, z); }

	Vector3 MoveComponent::CalcVelocitySub(float NowTime) {
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateZ
				);
		}
		return Pos;
	}


	void MoveComponent::CalcVelocity() {
		if (GetTotalTime() <= 0) {
			pImpl->m_Velocity = Vector3(0, 0, 0);
			return;
		}
		else {
			if (!GetRun() || GetArrived()) {
				pImpl->m_Velocity = Vector3(0, 0, 0);
				return;
			}
			else {
				float NowTime = GetNowTime();
				if (NowTime <= 0) {
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				if (ElapsedTime <= 0) {
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float BeforeTime = GetNowTime() - App::GetApp()->GetElapsedTime();
				if (BeforeTime <= 0) {
					BeforeTime = 0;
				}
				if ((NowTime - BeforeTime) <= 0) {
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				Vector3 BeforePos = CalcVelocitySub(BeforeTime);
				Vector3 NowPos = CalcVelocitySub(NowTime);
				pImpl->m_Velocity = (NowPos - BeforePos) / ElapsedTime;
				return;
			}
		}
	}


	//操作
	void MoveComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		pImpl->m_StartPosition = TransPtr->GetPosition();
		SetNowTime(0);
		SetRun(true);
		SetArrived(false);
		if (GetTotalTime() <= 0) {
			//トータルタイムが0以下なら、すでに到着していることになる
			TransPtr->SetPosition(pImpl->m_TargetPosition);
			SetTotalTime(0);
			SetNowTime(0);
			SetRun(false);
			SetArrived(true);
		}
		CalcVelocity();
	}

	void MoveComponent::OnUpdate() {
		if (GetRun()) {
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				Vector3 Pos;
				if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
					Pos = Lerp::CalculateLerp<Vector3>(
						pImpl->m_StartPosition,
						pImpl->m_TargetPosition,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
				}
				else {
					Pos.x = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.x,
						pImpl->m_TargetPosition.x,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
					Pos.y = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.y,
						pImpl->m_TargetPosition.y,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateY
						);
					Pos.z = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.z,
						pImpl->m_TargetPosition.z,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateZ
						);
				}
				PtrTrans->SetPosition(Pos);
			}
			else {
				if (GetTotalTime() <= 0) {
					//トータルタイムが0以下なら、すでに到着していることになる
					PtrTrans->SetPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	//現在位置を計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 MoveComponent::GetNowPosition() const {
		if (GetTotalTime() <= 0) {
			return pImpl->m_TargetPosition;
		}
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ) {
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateZ
				);
		}
		return Pos;
	}



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	用途: 目的の位置に移動
	//--------------------------------------------------------------------------------------
	MoveTo::MoveTo(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr) {}
	MoveTo::~MoveTo() {}

	void MoveTo::SetParams(float TotalTime, const Vector3& TargetPosition, Lerp::rate Rate) {
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveTo::SetParams(float TotalTime, const Vector3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}



	//--------------------------------------------------------------------------------------
	//	struct MoveBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MoveBy::Impl {
		Vector3 m_LocalVector;	//相対方向
		Impl() :
			m_LocalVector(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	用途: 目的の相対位置に移動
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBy::MoveBy(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr), pImpl(new Impl()) {}
	MoveBy::~MoveBy() {}

	//アクセサ
	const Vector3& MoveBy::GetLocalVector() const { return pImpl->m_LocalVector; }
	void MoveBy::SetLocalVector(const Vector3& LocalVector) { pImpl->m_LocalVector = LocalVector; }
	void MoveBy::SetLocalVector(float x, float y, float z) { pImpl->m_LocalVector = Vector3(x, y, z); }

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate) {
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}

	//操作
	void MoveBy::Run() {
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = PtrTrans->GetPosition();
		Vector3 Target = Pos + pImpl->m_LocalVector;
		SetTargetPosition(Target);
		MoveComponent::Run();
	}



	//--------------------------------------------------------------------------------------
	//	struct Action::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Action::Impl {
		bool m_Looped;

		bool m_ScaleArrived;	//m_Loopがfalseのときのみ有効
		bool m_RotateArrived;	//m_Loopがfalseのときのみ有効
		bool m_MoveArrived;	//m_Loopがfalseのときのみ有効


		vector<shared_ptr<ActionComponent> > m_ScaleVec;
		vector<shared_ptr<ActionComponent> > m_RotateVec;
		vector<shared_ptr<ActionComponent> > m_MoveVec;

		size_t m_ScaleActiveIndex;
		size_t m_RotateActiveIndex;
		size_t m_MoveActiveIndex;

		Impl() :
			m_Looped(false),
			m_ScaleArrived(false),
			m_RotateArrived(false),
			m_MoveArrived(false),
			m_ScaleActiveIndex(0),
			m_RotateActiveIndex(0),
			m_MoveActiveIndex(0)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	用途: 自動状態変更コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Action::Action(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Action::~Action() {}

	//アクセサ
	bool Action::IsLooped()const { return pImpl->m_Looped; }
	bool Action::GetLooped()const { return pImpl->m_Looped; }
	void Action::SetLooped(bool b) { pImpl->m_Looped = b; }

	bool Action::IsArrived()const {
		bool retScale = true;
		if (pImpl->m_ScaleVec.size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_ScaleArrived) {
				retScale = false;
			}
		}
		bool retRot = true;
		if (pImpl->m_RotateVec.size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_RotateArrived) {
				retRot = false;
			}
		}
		bool retMove = true;
		if (pImpl->m_MoveVec.size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_MoveArrived) {
				retMove = false;
			}
		}
		return
			retScale &&
			retRot &&
			retMove;
	}
	bool Action::GetArrived()const { return IsArrived(); }


	size_t Action::GetScaleActiveIndex() const {
		if (pImpl->m_ScaleVec.size() <= 0) {
			throw BaseException(
				L"スケールアクションが登録されていません",
				L"if (pImpl->m_ScaleVec.size() <= 0)",
				L"Action::GetScaleActiveIndex()"
			);
		}
		return pImpl->m_ScaleActiveIndex;
	}
	size_t Action::GetRotateActiveIndex() const {
		if (pImpl->m_RotateVec.size() <= 0) {
			throw BaseException(
				L"回転アクションが登録されていません",
				L"if (pImpl->m_RotateVec.size() <= 0)",
				L"Action::GetRotateActiveIndex()"
			);
		}
		return pImpl->m_RotateActiveIndex;
	}
	size_t Action::GetMoveActiveIndex() const {
		if (pImpl->m_MoveVec.size() <= 0) {
			throw BaseException(
				L"移動アクションが登録されていません",
				L"if (pImpl->m_MoveVec.size() <= 0)",
				L"Action::GetMoveActiveIndex()"
			);
		}
		return pImpl->m_MoveActiveIndex;
	}

	//現在の速度
	//Moveコンポーネントがない場合はVector3(0,0,0)を返す

	Vector3 Action::GetVelocity()const {
		if (pImpl->m_MoveVec.size() <= 0) {
			return Vector3(0, 0, 0);
		}
		else {
			auto Ptr = dynamic_pointer_cast<MoveComponent>(pImpl->m_MoveVec[pImpl->m_MoveActiveIndex]);
			if (Ptr) {
				return Ptr->GetVelocity();
			}
			else {
				return Vector3(0, 0, 0);
			}
		}
	}



	//操作
	shared_ptr<ScaleTo> Action::AddScaleTo(float TotalTime, const Vector3& TargetScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}
	shared_ptr<ScaleBy> Action::AddScaleBy(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionInterval> Action::AddScaleInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetScaleComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_ScaleVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_ScaleVec.size())",
				L"Action::GetScaleComponent()"
			);
		}
		return pImpl->m_ScaleVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetScaleVec() const {
		return pImpl->m_ScaleVec;
	}


	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Vector3& TargetRotate) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Quaternion& TargetQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Vector3& LocalRotate) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Quaternion& LocalQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddRotateInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetRotateComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_RotateVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_RotateVec.size())",
				L"Action::GetRotateComponent()"
			);
		}
		return pImpl->m_RotateVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetRotateVec() const {
		return pImpl->m_RotateVec;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& TargetPosition, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddMoveInterval(float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetMoveComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_MoveVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_MoveVec.size())",
				L"Action::GetMoveComponent()"
			);
		}
		return pImpl->m_MoveVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetMoveVec() const {
		return pImpl->m_MoveVec;
	}
	//すべてのアクションをクリアする
	void Action::AllActionClear() {
		pImpl->m_ScaleVec.clear();
		pImpl->m_RotateVec.clear();
		pImpl->m_MoveVec.clear();

		pImpl->m_Looped = false;
		pImpl->m_ScaleArrived = false;
		pImpl->m_RotateArrived = false;
		pImpl->m_MoveArrived = false;
		pImpl->m_ScaleActiveIndex = 0;
		pImpl->m_RotateActiveIndex = 0;
		pImpl->m_MoveActiveIndex = 0;
	}



	void Action::RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0) {
			TgtIndex = 0;
			TgtVector[TgtIndex]->Run();
		}
	}
	void Action::Run() {
		RunSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		RunSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		RunSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->Stop();
		}
	}
	void Action::Stop() {
		StopSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		StopSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		StopSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->ReStart();
		}
	}

	void Action::ReStart() {
		ReStartSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		ReStartSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		ReStartSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	bool Action::UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		bool ret = false;
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			if (TgtVector[TgtIndex]->IsUpdateActive()) {
				TgtVector[TgtIndex]->OnUpdate();
				if (TgtVector[TgtIndex]->IsArrived()) {
					//到着した
					//インデックスを加算
					TgtIndex++;
					if (TgtIndex >= TgtVector.size()) {
						//範囲外なら
						if (pImpl->m_Looped) {
							TgtIndex = 0;
							TgtVector[TgtIndex]->Run();
						}
						else {
							TgtIndex = TgtVector.size() - 1;
							//Runは行わない
							//到着した
							ret = true;
						}
					}
					else {
						TgtVector[TgtIndex]->Run();
					}
				}
			}
		}
		return ret;
	}
	//更新
	void Action::OnUpdate() {
		pImpl->m_ScaleArrived = UpdateSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		pImpl->m_RotateArrived = UpdateSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		pImpl->m_MoveArrived = UpdateSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}


}
//end basecross
