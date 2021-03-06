/*!
@file Character.h
@brief LN^[ΘΗ
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PC
	//--------------------------------------------------------------------------------------
	class PcSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PcSphere();
		//ϊ»
		virtual void OnCreate() override;
		//XV
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	///	PT
	//--------------------------------------------------------------------------------------
	class PtSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PtSphere();
		//ϊ»
		virtual void OnCreate() override;
		//XV
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PCT
	//--------------------------------------------------------------------------------------
	class PctSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PctSphere();
		//ϊ»
		virtual void OnCreate() override;
		//XV
		virtual void OnUpdate() override;
	};




	//--------------------------------------------------------------------------------------
	///	Pnt
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//\zΖjό
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,bool TextureUse);
		virtual ~PntSphere();
		//ϊ»
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PntXyL[
	//--------------------------------------------------------------------------------------
	class PntSpSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//\zΖjό
		PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse);
		virtual ~PntSpSphere();
		//ϊ»
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	StaticL
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~StaticChara();
		//ϊ»
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	StaticL(}`bVΕ)
	//--------------------------------------------------------------------------------------
	class StaticMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~StaticMultiMeshChara();
		//ϊ»
		virtual void OnCreate() override;
	};



	//--------------------------------------------------------------------------------------
	///	BoneL
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BoneChara();
		//ϊ»
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	BoneL(}`bVΕ)
	//--------------------------------------------------------------------------------------
	class BoneMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
	public:
		//\zΖjό
		BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BoneMultiMeshChara();
		//ϊ»
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


}
//end basecross
