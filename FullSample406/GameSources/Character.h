/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	RollingBoxグループ(インスタンス描画のためだけのオブジェクト)
	//--------------------------------------------------------------------------------------
	class RollingBoxGroup : public GameObject {
	public:
		//構築と破棄
		RollingBoxGroup(const shared_ptr<Stage>& StagePtr);
		virtual ~RollingBoxGroup();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	class RollingBox : public GameObject;
	//	用途: 固定の回転ボックス
	//--------------------------------------------------------------------------------------
	class RollingBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		//回転係数
		float m_RotParam;
	public:
		//構築と破棄
		RollingBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float RotParam
		);
		virtual ~RollingBox();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
	};



}
//end basecross
