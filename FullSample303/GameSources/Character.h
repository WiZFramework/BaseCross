/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	MySeekSteering行動クラス
	//--------------------------------------------------------------------------------------
	class MySeekSteering : public SteeringBehavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		MySeekSteering(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MySeekSteering();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動を実行する
		@param[in]	TargetPos	追いかける位置
		@return	追いかける位置との距離
		*/
		//--------------------------------------------------------------------------------------
		float Execute(const Vector3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動を実行する
		@param[in]	TargetKey	追いかけるオブジェクトのキー（SharedObjec）
		@return	追いかけるオブジェクトとの距離
		*/
		//--------------------------------------------------------------------------------------
		float Execute(const wstring& TargetKey);
	};



	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//ステートマシーン
		Vector3 m_StartPos;
		float m_StateChangeSize;
		//進行方向を向くようにする
		void RotToHead();
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


}
//end basecross
