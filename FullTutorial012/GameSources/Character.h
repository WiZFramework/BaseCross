/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	物理計算するボックス
	//--------------------------------------------------------------------------------------
	class PhysicsBox : public GameObject {
		PsMotionType m_MotionType;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		size_t m_PsTransIndex;
	public:
		//構築と破棄
		PhysicsBox(const shared_ptr<Stage>& StagePtr,
			PsMotionType MotionType,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~PhysicsBox();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
	};


}
//end basecross
