/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ボックス
	//--------------------------------------------------------------------------------------
	class Box : public GameObject {
		Vec3 m_StartPos;
		float m_TotalTime;
	public:
		//構築と破棄
		Box(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~Box();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};


}
//end basecross
