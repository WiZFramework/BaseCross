/*!
@file Rigidbody.h
@brief 物理計算コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Rigidbody::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Rigidbody::Impl {
		Vector3 m_Velocity;				//速度
		float m_MaxSpeed;				//最高速度(フォースで使用)
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
			m_Reflection(0.5f),
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
		if (Velocity.IsNaN() || Velocity.IsInfinite()) {
			pImpl->m_Velocity = Vector3(0,0,0);
		}
		else {
			pImpl->m_Velocity = Velocity;
		}
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
		Vector3 Pos = PtrTransform->GetWorldPosition();
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
		PtrTransform->SetWorldPosition(Pos);
	}

	void Rigidbody::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

}
//end basecross
