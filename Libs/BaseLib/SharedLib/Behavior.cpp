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
		Vector3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt.Slerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	void UtilBehavior::RotToHead(const Vector3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//回転の更新
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt.Slerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}

	}


	//--------------------------------------------------------------------------------------
	///	Jump行動クラス
	//--------------------------------------------------------------------------------------
	void JumpBehavior::StartJump(const Vector3& FirstVelocity) {
		//重力
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		//ジャンプスタート
		PtrGravity->StartJump(FirstVelocity);
	}

	bool JumpBehavior::Execute() {
		//重力
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}





	//--------------------------------------------------------------------------------------
	///	Seek行動クラス
	//--------------------------------------------------------------------------------------
	void SeekBehavior::Enter(const Vector3& TargetPos) {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(TargetPos);
	}
	void SeekBehavior::Enter(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(TargetPos);
	}



	float  SeekBehavior::Execute(const Vector3& TargetPos, bool RotHead, float LerpFact) {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}

	float SeekBehavior::Execute(const wstring& TargetKey, bool RotHead, float LerpFact) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}


	void SeekBehavior::Exit() {
		auto PtrSeek = GetGameObject()->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}


	//--------------------------------------------------------------------------------------
	///	Arrive行動クラス
	//--------------------------------------------------------------------------------------
	void ArriveBehavior::Enter(const Vector3& TargetPos) {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(TargetPos);
	}
	void ArriveBehavior::Enter(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(TargetPos);
	}



	float  ArriveBehavior::Execute(const Vector3& TargetPos, bool RotHead, float LerpFact) {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}

	float ArriveBehavior::Execute(const wstring& TargetKey, bool RotHead, float LerpFact) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetPosition();
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(TargetPos);
		if (RotHead) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(LerpFact);
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(Pos - TargetPos);
	}


	void ArriveBehavior::Exit() {
		auto PtrArrive = GetGameObject()->GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
	}





}
//end basecross
