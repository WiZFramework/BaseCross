/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	飛んでいくボール
	//--------------------------------------------------------------------------------------
	class AttackBall : public GameObject {
	public:
		//構築と破棄
		AttackBall(const shared_ptr<Stage>& StagePtr);
		virtual ~AttackBall();
		void Weakup(const Vec3& Position, const Vec3& Velocity);
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
	};


	//Aボタンで動作するアクション
	enum class PlayerAction {
		Attack
	};



	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//文字列の表示
		void DrawStrings();
		//ステートマシーン
		unique_ptr<LayeredStateMachine<Player>>  m_StateMachine;
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//プレイヤーのAボタンによる行動
		PlayerAction m_PlayerAction;
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
		unique_ptr<LayeredStateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

		//Aボタン
		void OnPushA();
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
	///	アタックステート
	//--------------------------------------------------------------------------------------
	class PlayerAttackState : public ObjState<Player>
	{
		PlayerAttackState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerAttackState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};





}
//end basecross

