/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	用途: 卵のアニメーション
	//--------------------------------------------------------------------------------------
	class EggAnime : public SS5ssae {
		float m_Span;
	public:
		//構築と消滅
		EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir);
		virtual ~EggAnime();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
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
