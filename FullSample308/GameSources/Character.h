/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
	};

	//--------------------------------------------------------------------------------------
	//	用途: 剣
	//--------------------------------------------------------------------------------------
	class Sword : public GameObject {
		Vec3 m_Scale;
		wstring m_Tag;
		//剣の強さ
		float m_Strength;
	public:
		//構築と破棄
		Sword(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const wstring& Tag
		);
		virtual ~Sword();
		//初期化
		virtual void OnCreate() override;
		//強さを得る
		float GetStrength() const {
			return m_Strength;
		}
		//強さを設定する
		void SetStrength(float f) {
			m_Strength = f;
		}
		//操作
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	//	敵の箱
	//--------------------------------------------------------------------------------------
	class EnemyBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		EnemyBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~EnemyBox();
		//初期化
		virtual void OnCreate() override;
		//操作
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint);
	};

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
		vector<Vec3> m_TempPosvec;
		shared_ptr<MeshResource> m_TriangleMesh;
	public:
		//構築と破棄
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BoneChara();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint);
	};


}
//end basecross
