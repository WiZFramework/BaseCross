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
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//ステートマシーン
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//ユーティリティ関数群
		//プレイヤーの位置を返す
		Vector3 GetPlayerPosition() const;
		//プレイヤーまでの距離を返す
		float GetPlayerLength() const;
	public:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const {
			return m_StateMachine;
		}
		//モーションを実装する関数群
		void  SeekStartMoton();
		bool  SeekUpdateMoton();
		void  SeekEndMoton();

		void  ArriveStartMoton();
		bool  ArriveUpdateMoton();
		void  ArriveEndMoton();

		//操作
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
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

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
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
		Vector3 m_StartPos;
	public:
		//構築と破棄
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SphereObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};





}
//end basecross
