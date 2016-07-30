/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public GameObject {
	public:
		//構築と破棄
		NormalTextureBox(const shared_ptr<Stage>& StagePtr);
		virtual ~NormalTextureBox();
		//初期化
		virtual void OnCreate()override {}
		void OnCreateWithParam(const Vector3& Pos);
		virtual void OnUpdate();
	};



}
//end basecross
