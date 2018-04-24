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
	///	ライン
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartPos;
		Vec3 m_EndPos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartPos	開始点
		@param[in]	EndtPos　　終了点
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndtPos);
		virtual ~ActionLine() {}
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		const Vec3& GetStartPos()const {
			return m_StartPos;
		}
		const Vec3& GetEndPos()const {
			return m_EndPos;
		}
		void SetStartPos(const Vec3& StartPos) {
			m_StartPos = StartPos;
		}
		void SetEndPos(const Vec3& EndtPos) {
			m_EndPos = EndtPos;
		}

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
	//	敵
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		shared_ptr<MeshResource> m_BallMesh;
		vector<Vec3> m_WorldTriangles;
	public:
		//構築と破棄
		Enemy(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Enemy();
		//初期化
		virtual void OnCreate() override;
		//操作
		vector<Vec3>& WorldTriangles();
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint);
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
	};

	//--------------------------------------------------------------------------------------
	///	Bone三角形オブジェクト
	//--------------------------------------------------------------------------------------
	class BoneTriangles : public GameObject {
		weak_ptr<BoneChara> m_BoneChara;
		vector<VertexPositionColor> m_BackupVertices;
		shared_ptr<MeshResource> m_TriangleMesh;
		vector<Vec3> m_TempPosvec;
	public:
		//構築と破棄
		BoneTriangles(const shared_ptr<Stage>& StagePtr,const shared_ptr<BoneChara>& boneChara);
		virtual ~BoneTriangles() {}
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, bsm::Vec3& HitPoint);
	};


	//--------------------------------------------------------------------------------------
	///	ヒット時の三角形オブジェクト
	//--------------------------------------------------------------------------------------
	class HitTriangles : public GameObject {
		shared_ptr<MeshResource> m_TriangleMesh;
		struct DrawTriangle {
			TRIANGLE m_Triangle;
			float m_LastTime;
			DrawTriangle() {}
			DrawTriangle(const TRIANGLE& tri, float lasttime) :
				m_Triangle(tri), m_LastTime(lasttime) {}
		};
		vector<DrawTriangle> m_DrawTriangleVec;
	public:
		//構築と破棄
		HitTriangles(const shared_ptr<Stage>& StagePtr);
		virtual ~HitTriangles() {}
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void AddHitTriangle(const TRIANGLE& tri);
	};




}
//end basecross
