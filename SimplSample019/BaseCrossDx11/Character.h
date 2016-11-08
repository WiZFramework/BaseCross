/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	カメラマン
	//--------------------------------------------------------------------------------------
	class Cameraman : public GameObject {
	public:
		//構築と破棄
		Cameraman(const shared_ptr<Stage>& StagePtr);
		virtual ~Cameraman();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
	};



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};



}
//end basecross
