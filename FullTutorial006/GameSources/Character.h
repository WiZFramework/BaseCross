/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class FineSeekBehavior;
	class TiredSeekBehavior;
	class SeekBehavior;
	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//行動クラスはfriend宣言を付ける
		friend class FineSeekBehavior;
		friend class TiredSeekBehavior;
		//現在の行動オブジェクト
		shared_ptr<SeekBehavior> m_SeekBehavior;
		//行動を切りかえるためのタイマー
		float m_FineTiredTime;
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
		//行動
		shared_ptr<SeekBehavior> GetSeekBehavior() const {
			return m_SeekBehavior;
		}
		//ステートマシン
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const {
			return m_StateMachine;
		}
		//操作
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	//	Seekオブジェクトの行動の親クラス
	//--------------------------------------------------------------------------------------
	class SeekBehavior {
	protected:
		SeekBehavior() {}
		virtual ~SeekBehavior() {}
	public:
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) = 0;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	//	Seekオブジェクトの元気な行動
	//--------------------------------------------------------------------------------------
	class FineSeekBehavior : public SeekBehavior {
		FineSeekBehavior() {}
	public:
		static shared_ptr<FineSeekBehavior> Instance();
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	Seekオブジェクトの疲れた行動
	//--------------------------------------------------------------------------------------
	class TiredSeekBehavior : public SeekBehavior {
		TiredSeekBehavior() {}
	public:
		static shared_ptr<TiredSeekBehavior> Instance();
		virtual void FarEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void FarExit(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearEnter(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExecute(const shared_ptr<SeekObject>& Obj) override;
		virtual void NearExit(const shared_ptr<SeekObject>& Obj) override;
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

	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	class CapsuleObject : public GameObject {
		Vector3 m_StartPos;
	public:
		//構築と破棄
		CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CapsuleObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};




}
//end basecross
