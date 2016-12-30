/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{
	}
	Box::~Box() {}

	//初期化
	void Box::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透過処理
		SetAlphaActive(true);
	}

	//更新
	void Box::OnUpdate() {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos.x = sin(m_TotalTime);
		GetComponent<Transform>()->SetPosition(Pos);
	}



}
//end basecross
