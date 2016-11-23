/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	class Player;
	//--------------------------------------------------------------------------------------
	///	ジャンプコマンド
	//--------------------------------------------------------------------------------------
	class JumpCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------
	///	行動切り替えコマンド
	//--------------------------------------------------------------------------------------
	class BehaviorChangeCommand : public ObjCommand<Player> {
		virtual void Excute(const shared_ptr<Player>& Obj)override;
	};


	class PlayerBehavior;
	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//行動関数クラス
		shared_ptr<PlayerBehavior> m_PlayerBehavior;
		//階層化ステートマシーン
		shared_ptr< LayeredStateMachine<Player> >  m_StateMachine;	//ステートマシーン
		//コマンド関連
		void InputCheck();
		//ジャンプコマンド
		JumpCommand m_Jump;
		//行動切り替え
		BehaviorChangeCommand m_BehaviorChange;
		InputHandler<Player> m_Handler;
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
		virtual ~Player();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	階層化ステートマシンを得る
		@return	階層化ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< LayeredStateMachine<Player> > GetStateMachine() const {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動関数クラスを得る
		@return	行動関数クラス
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PlayerBehavior> GetPlayerBehavior() const {
			return m_PlayerBehavior;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動関数クラスを切り替える
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ChangePlayerBehavior();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤー行動親
	//--------------------------------------------------------------------------------------
	class PlayerBehavior {
	protected:
		PlayerBehavior() {}
	public:
		virtual ~PlayerBehavior() {}
		virtual Vector3 GetAngle(const shared_ptr<Player>& Obj);
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj) = 0;
		virtual void Move(const shared_ptr<Player>& Obj) = 0;
		virtual void StartJump(const shared_ptr<Player>& Obj) = 0;
		virtual void JumpExcute(const shared_ptr<Player>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	シンプルプレイヤー行動
	//--------------------------------------------------------------------------------------
	class SimplePlayerBehavior : public PlayerBehavior {
	protected:
		SimplePlayerBehavior() {}
	public:
		//行動のインスタンス取得
		DECLARE_SINGLETON_INSTANCE(SimplePlayerBehavior)
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj)override;
		virtual void Move(const shared_ptr<Player>& Obj)override;
		virtual void StartJump(const shared_ptr<Player>& Obj)override;
		virtual void JumpExcute(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ハイジャンププレイヤー行動
	//--------------------------------------------------------------------------------------
	class HighJumpPlayerBehavior : public SimplePlayerBehavior {
	protected:
		HighJumpPlayerBehavior() {}
	public:
		//行動のインスタンス取得
		DECLARE_SINGLETON_INSTANCE(HighJumpPlayerBehavior)
		virtual shared_ptr<PlayerBehavior> CreateNextBehavior(const shared_ptr<Player>& Obj)override;
		virtual void StartJump(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PlayerDefault : public ObjState<Player>
	{
		PlayerDefault() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerDefault)
		virtual void Enter(const shared_ptr<Player>& Obj)override {}
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};

	//--------------------------------------------------------------------------------------
	///	ジャンプステート
	//--------------------------------------------------------------------------------------
	class PlayerJump : public ObjState<Player>
	{
		PlayerJump() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerJump)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override {}
	};





}
//end basecross

