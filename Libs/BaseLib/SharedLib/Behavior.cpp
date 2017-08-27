/*!
@file Behavior.cpp
@brief 行動クラス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Behavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Behavior::Impl {
		weak_ptr<GameObject> m_GameObject;
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& GameObjectPtr) :
		pImpl(new Impl(GameObjectPtr))
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (!shptr)",
				L"Behavior::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}
	shared_ptr<Stage> Behavior::GetStage() const {
		return GetGameObject()->GetStage();
	}


	//--------------------------------------------------------------------------------------
	///	行動ユーティリティクラス
	//--------------------------------------------------------------------------------------
	//進行方向を向くようにする
	void UtilBehavior::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	void UtilBehavior::RotToHead(const bsm::Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//回転の更新
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}

	}


	//--------------------------------------------------------------------------------------
	///	何もしない行動クラス
	//--------------------------------------------------------------------------------------
	float WaitBehavior::Execute(const bsm::Vec3& TargetPos) {
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		//減速
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		return bsm::length(Pos - TargetPos);
	}

	float WaitBehavior::Execute(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetWorldPosition();
		return Execute(TargetPos);
	}


	//--------------------------------------------------------------------------------------
	//	struct GravityBehavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl {
		//現在の重力加速度
		bsm::Vec3 m_Gravity;
	public:
		Impl() :
			m_Gravity(0, -9.8f, 0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	Gravity行動クラス
	//--------------------------------------------------------------------------------------
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Behavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	Gravity::~Gravity() {}

	const bsm::Vec3& Gravity::GetGravity() const {
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const bsm::Vec3& gravity) {
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravity(float x, float y, float z) {
		pImpl->m_Gravity = bsm::Vec3(x, y, z);
	}

	void Gravity::StartJump(const bsm::Vec3& StartVec, float EscapeSpan) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetGravityVelocity();
		Velo += StartVec;
		PtrRigid->SetGravityVelocity(Velo);
		bsm::Vec3 EscapeVec = StartVec;
		//ジャンプして親オブジェクトボリュームから脱出できないとき対応
		EscapeVec *= EscapeSpan;
		auto Pos = PtrTransform->GetWorldPosition();
		Pos += EscapeVec;
		PtrTransform->ResetWorldPosition(Pos);
	}
	void Gravity::StartJump(float x, float y, float z, float EscapeSpan) {
		StartJump(bsm::Vec3(x, y, z), EscapeSpan);
	}

	void Gravity::UpdateFromTime(float CalcTime) {
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetGravityVelocity();
		Velo += pImpl->m_Gravity * CalcTime;
		PtrRigid->SetGravityVelocity(Velo);
	}

	void Gravity::Execute() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}


}
//end basecross
