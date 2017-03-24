/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~StaticChara();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BoneChara();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};




}
//end basecross
