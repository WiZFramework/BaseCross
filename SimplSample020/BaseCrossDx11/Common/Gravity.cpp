/*!
@file Gravity.cpp
@brief 重力コンポーネント実体
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct Gravity::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl {
		Vector3 m_DefaultGravity;		//自由落下加速度
		Vector3 m_Gravity;				//現在の重力加速度
		Vector3 m_GravityVelocity;		//重力加速度による現在の速度
	public:
		Impl() :
			m_DefaultGravity(0, -9.8f, 0),
			m_Gravity(m_DefaultGravity),
			m_GravityVelocity(0, 0, 0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Gravity : public Component ;
	//	用途: 重力
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Gravity::~Gravity() {}

	//アクセサ

	const Vector3& Gravity::GetDefaultGravity() const {
		return pImpl->m_DefaultGravity;
	}
	void Gravity::SetDefaultGravity(const Vector3& gravity) {
		pImpl->m_DefaultGravity = gravity;
	}
	void Gravity::SetDefaultGravity(float x, float y, float z) {
		pImpl->m_DefaultGravity = Vector3(x, y, z);

	}


	const Vector3& Gravity::GetGravity() const {
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const Vector3& gravity) {
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravity(float x, float y, float z) {
		pImpl->m_Gravity = Vector3(x, y, z);
	}
	void Gravity::SetGravityZero() {
		pImpl->m_Gravity.Zero();
	}

	void Gravity::SetGravityDefault() {
		pImpl->m_Gravity = pImpl->m_DefaultGravity;
	}



	const Vector3& Gravity::GetGravityVelocity() const {
		return pImpl->m_GravityVelocity;
	}

	void Gravity::SetGravityVelocity(const Vector3& GravityVelocity) {
		pImpl->m_GravityVelocity = GravityVelocity;
	}
	void Gravity::SetGravityVelocity(float x, float y, float z) {
		pImpl->m_GravityVelocity = Vector3(x, y, z);
	}

	bool Gravity::IsGravityVelocityZero() {
		return (pImpl->m_GravityVelocity == Vector3(0, 0, 0));
	}
	void Gravity::SetGravityVelocityZero() {
		pImpl->m_GravityVelocity.Zero();
	}

	void Gravity::StartJump(const Vector3& StartVec, float EscapeSpan) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		SetGravityVelocity(StartVec);
		Vector3 EscapeVec = StartVec;
		//ジャンプして親オブジェクトボリュームから脱出できないとき対応
		EscapeVec *= EscapeSpan;
		auto Pos = PtrTransform->GetPosition();
		Pos += EscapeVec;
		PtrTransform->ResetPosition(Pos);
	}
	void Gravity::StartJump(float x, float y, float z, float EscapeSpan) {
		StartJump(Vector3(x, y, z), EscapeSpan);
	}

	void Gravity::UpdateFromTime(float CalcTime) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		Vector3 Pos = PtrTransform->GetPosition();
		//自由落下加速度を計算
		pImpl->m_GravityVelocity += pImpl->m_Gravity * CalcTime;
		Pos += pImpl->m_GravityVelocity * CalcTime;
		//位置を設定
		PtrTransform->SetPosition(Pos);
	}

	//操作
	void Gravity::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}



}
//end basecross

