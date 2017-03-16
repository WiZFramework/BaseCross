/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PC球
	//--------------------------------------------------------------------------------------
	class PcSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		PcSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PcSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	///	PT球
	//--------------------------------------------------------------------------------------
	class PtSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		PtSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PtSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PCT球
	//--------------------------------------------------------------------------------------
	class PctSphere : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		PctSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~PctSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};




	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vector3 m_StartPos;
		bool m_TextureUse;
	public:
		//構築と破棄
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,bool TextureUse);
		virtual ~PntSphere();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Pntスペキュラー球
	//--------------------------------------------------------------------------------------
	class PntSpSphere : public GameObject {
		Vector3 m_StartPos;
		bool m_TextureUse;
	public:
		//構築と破棄
		PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TextureUse);
		virtual ~PntSpSphere();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vector3 m_StartPos;
		bool m_TamgentUse;
	public:
		//構築と破棄
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse);
		virtual ~StaticChara();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vector3 m_StartPos;
		bool m_TamgentUse;
	public:
		//構築と破棄
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, bool TamgentUse);
		virtual ~BoneChara();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};



}
//end basecross
