/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//文字列の表示
		void DrawStrings();
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//ステートマシーン
		unique_ptr<StateMachine<Player>>  m_StateMachine;
		//

	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Player> >& GetStateMachine(){
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		//Aボタンハンドラ
		void OnPushA();
		//Bボタンハンドラ
		void OnPushB();
		//Xボタンハンドラ
		void OnPushX();
	};


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ジャンプステート
	//--------------------------------------------------------------------------------------
	class PlayerJumpState : public ObjState<Player>
	{
		PlayerJumpState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerJumpState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//end basecross

