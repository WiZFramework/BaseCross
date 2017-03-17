/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PntSphere();
		//初期化
		virtual void OnCreate() override;
		virtual void OnDraw() override;
	};



}
//end basecross
