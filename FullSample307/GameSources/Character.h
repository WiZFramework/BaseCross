/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	用途: タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vector3 m_Scale;
		Quaternion m_Qt;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~TilingPlate();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	///	フレームボックス
	//--------------------------------------------------------------------------------------
	class FrameBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		FrameBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		virtual ~FrameBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};






}
//end basecross
