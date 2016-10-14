/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	用途: 固定のモデル
	//--------------------------------------------------------------------------------------
	class StaticModel : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		StaticModel(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~StaticModel();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


}
//end basecross
