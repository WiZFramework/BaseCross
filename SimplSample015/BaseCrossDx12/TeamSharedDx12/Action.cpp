/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "../Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	用途: アクション系コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionComponent::ActionComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		m_Run(false),
		m_Arrived(false),
		m_TotalTime(0),
		m_NowTime(0)
	{}
	ActionComponent::~ActionComponent() {}

	//アクセサ
	void ActionComponent::SetRun(bool b) {
		m_Run = b;
	}
	void  ActionComponent::SetArrived(bool b) {
		m_Arrived = b;
	}


	bool ActionComponent::IsRun()const {
		return m_Run;
	}
	bool ActionComponent::GetRun()const {
		return m_Run;
	}

	bool ActionComponent::IsArrived()const {
		return m_Arrived;
	}
	bool ActionComponent::GetArrived()const {
		return m_Arrived;
	}

	float ActionComponent::GetTotalTime() const {
		return m_TotalTime;
	}
	void ActionComponent::SetTotalTime(float f) {
		m_TotalTime = f;
	}

	float ActionComponent::GetNowTime() const {
		return m_NowTime;
	}
	void ActionComponent::SetNowTime(float f) {
		m_NowTime = f;
	}

	//NowTimeを加算して、TotalTimeと比較する
	bool ActionComponent::AdditionalNowTime() {
		if (m_Run) {
			if (m_TotalTime > 0) {
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				m_NowTime += ElapsedTime;
				if (m_NowTime >= m_TotalTime) {
					m_NowTime = m_TotalTime;
					m_Run = false;
					//到着した
					m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}


	void ActionComponent::Stop() {
		m_Run = false;
	}
	void ActionComponent::ReStart() {
		if (!m_Arrived) {
			//到着してなければ動作させる
			m_Run = true;
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
	//	class MoveComponent : public ActionComponent ;
	//	用途: 移動アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveComponent::MoveComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		m_StartPosition(0, 0, 0),
		m_TargetPosition(0, 0, 0),
		m_RateX(Lerp::Linear),
		m_RateY(Lerp::Linear),
		m_RateZ(Lerp::Linear),
		m_Velocity(0, 0, 0)
	{}
	MoveComponent::~MoveComponent() {}
	//アクセサ

	Lerp::rate MoveComponent::GetRateX() const {
		return m_RateX;
	}
	Lerp::rate MoveComponent::GetRateY() const {
		return m_RateY;
	}
	Lerp::rate MoveComponent::GetRateZ() const {
		return m_RateZ;
	}
	void MoveComponent::SetRateX(const Lerp::rate r) {
		m_RateX = r;
	}
	void MoveComponent::SetRateY(const Lerp::rate r) {
		m_RateY = r;
	}
	void MoveComponent::SetRateZ(const Lerp::rate r) {
		m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate r) {
		m_RateX = r;
		m_RateY = r;
		m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate rX, const Lerp::rate rY, const Lerp::rate rZ) {
		m_RateX = rX;
		m_RateY = rY;
		m_RateZ = rZ;
	}

	Vector3 MoveComponent::GetVelocity()const {
		Vector3 Ret = m_Velocity;
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



	const Vector3& MoveComponent::GetStartPosition() const { return m_StartPosition; }
	void MoveComponent::SetStartPosition(const Vector3& StartPosition) { m_StartPosition = StartPosition; }
	void MoveComponent::SetStartPosition(float x, float y, float z) { m_StartPosition = Vector3(x, y, z); }

	const Vector3& MoveComponent::GetTargetPosition() const { return m_TargetPosition; }
	void MoveComponent::SetTargetPosition(const Vector3& TargetPosition) { m_TargetPosition = TargetPosition; }
	void MoveComponent::SetTargetPosition(float x, float y, float z) { m_TargetPosition = Vector3(x, y, z); }

	Vector3 MoveComponent::CalcVelocitySub(float NowTime) {
		Vector3 Pos;
		if (m_RateX == m_RateY && m_RateX == m_RateZ) {
			Pos = Lerp::CalculateLerp<Vector3>(
				m_StartPosition,
				m_TargetPosition,
				0,
				GetTotalTime(),
				NowTime,
				m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				m_StartPosition.x,
				m_TargetPosition.x,
				0,
				GetTotalTime(),
				NowTime,
				m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				m_StartPosition.y,
				m_TargetPosition.y,
				0,
				GetTotalTime(),
				NowTime,
				m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				m_StartPosition.z,
				m_TargetPosition.z,
				0,
				GetTotalTime(),
				NowTime,
				m_RateZ
				);
		}
		return Pos;
	}


	void MoveComponent::CalcVelocity() {
		if (GetTotalTime() <= 0) {
			m_Velocity = Vector3(0, 0, 0);
			return;
		}
		else {
			if (!GetRun() || GetArrived()) {
				m_Velocity = Vector3(0, 0, 0);
				return;
			}
			else {
				float NowTime = GetNowTime();
				if (NowTime <= 0) {
					m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				if (ElapsedTime <= 0) {
					m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float BeforeTime = GetNowTime() - App::GetApp()->GetElapsedTime();
				if (BeforeTime <= 0) {
					BeforeTime = 0;
				}
				if ((NowTime - BeforeTime) <= 0) {
					m_Velocity = Vector3(0, 0, 0);
					return;
				}
				Vector3 BeforePos = CalcVelocitySub(BeforeTime);
				Vector3 NowPos = CalcVelocitySub(NowTime);
				m_Velocity = (NowPos - BeforePos) / ElapsedTime;
				return;
			}
		}
	}


	//操作
	void MoveComponent::Run() {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		if (TransPtr) {
			m_StartPosition = TransPtr->GetPosition();
			SetNowTime(0);
			SetRun(true);
			SetArrived(false);
			if (GetTotalTime() <= 0) {
				//トータルタイムが0に設定されている
				//トータルタイムが0以下なら、すでに到着していることになる
				TransPtr->SetPosition(m_TargetPosition);
				SetTotalTime(0);
				SetNowTime(0);
				SetRun(false);
				SetArrived(true);
			}
		}
		CalcVelocity();
	}

	void MoveComponent::OnUpdate() {
		if (GetRun()) {
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()) {
				Vector3 Pos;
				if (m_RateX == m_RateY && m_RateX == m_RateZ) {
					Pos = Lerp::CalculateLerp<Vector3>(
						m_StartPosition,
						m_TargetPosition,
						0,
						GetTotalTime(),
						GetNowTime(),
						m_RateX
						);
				}
				else {
					Pos.x = Lerp::CalculateLerp<float>(
						m_StartPosition.x,
						m_TargetPosition.x,
						0,
						GetTotalTime(),
						GetNowTime(),
						m_RateX
						);
					Pos.y = Lerp::CalculateLerp<float>(
						m_StartPosition.y,
						m_TargetPosition.y,
						0,
						GetTotalTime(),
						GetNowTime(),
						m_RateY
						);
					Pos.z = Lerp::CalculateLerp<float>(
						m_StartPosition.z,
						m_TargetPosition.z,
						0,
						GetTotalTime(),
						GetNowTime(),
						m_RateZ
						);
				}
				PtrTrans->SetPosition(Pos);
			}
			else {
				if (GetTotalTime() <= 0) {
					//トータルタイムが0以下なら、すでに到着していることになる
					PtrTrans->SetPosition(m_TargetPosition);
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
			return m_TargetPosition;
		}
		Vector3 Pos;
		if (m_RateX == m_RateY && m_RateX == m_RateZ) {
			Pos = Lerp::CalculateLerp<Vector3>(
				m_StartPosition,
				m_TargetPosition,
				0,
				GetTotalTime(),
				GetNowTime(),
				m_RateX
				);
		}
		else {
			Pos.x = Lerp::CalculateLerp<float>(
				m_StartPosition.x,
				m_TargetPosition.x,
				0,
				GetTotalTime(),
				GetNowTime(),
				m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				m_StartPosition.y,
				m_TargetPosition.y,
				0,
				GetTotalTime(),
				GetNowTime(),
				m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				m_StartPosition.z,
				m_TargetPosition.z,
				0,
				GetTotalTime(),
				GetNowTime(),
				m_RateZ
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
	//	class MoveBy : public  MoveComponent ;
	//	用途: 目的の相対位置に移動
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBy::MoveBy(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr), 
		m_LocalVector(0, 0, 0)
	{}
	MoveBy::~MoveBy() {}

	//アクセサ
	const Vector3& MoveBy::GetLocalVector() const { return m_LocalVector; }
	void MoveBy::SetLocalVector(const Vector3& LocalVector) { m_LocalVector = LocalVector; }
	void MoveBy::SetLocalVector(float x, float y, float z) { m_LocalVector = Vector3(x, y, z); }

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
		Vector3 Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		Vector3 Target = Pos + m_LocalVector;
		SetTargetPosition(Target);
		MoveComponent::Run();
	}




}
//end basecross
