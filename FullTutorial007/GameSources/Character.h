/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//ステートマシーン
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_StateChangeSize;
	public:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine() {
			return m_StateMachine;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//操作
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class StaticModel : public GameObject;
	//	用途: 固定のモデル
	//--------------------------------------------------------------------------------------
	class StaticModel : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		StaticModel(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~StaticModel();
		//初期化
		virtual void OnCreate() override;
		//操作
	};



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~MoveBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SphereObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	class CapsuleObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~CapsuleObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	用途: でこぼこ床のデータ
	//--------------------------------------------------------------------------------------
	class UnevenGroundData : public GameObject {
		vector<TRIANGLE> m_Triangles;
	public:
		//構築と破棄
		UnevenGroundData(const shared_ptr<Stage>& StagePtr);
		virtual ~UnevenGroundData();
		//初期化
		virtual void OnCreate() override;
		//三角形の配列を返す
		const vector<TRIANGLE>& GetTriangles() const {
			return m_Triangles;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	用途: でこぼこ床
	//--------------------------------------------------------------------------------------
	class UnevenGround : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		UnevenGround(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position);
		virtual ~UnevenGround();
		//初期化
		virtual void OnCreate() override;
		//操作
	};





}
//end basecross
