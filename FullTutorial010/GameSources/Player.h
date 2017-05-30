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
		//戦いの行動
		unique_ptr<FightBehavior<Player>> m_FightBehavior;
		//マックスHP
		float m_MaxHP;
		//現在のHP
		float m_HP;
		//スタート位置
		Vector3 m_StartPosition;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Player> >& GetStateMachine(){
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	戦いの行動を得る
		@return	戦いの行動
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< FightBehavior<Player> >& GetFightBehavior() {
			return m_FightBehavior;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を得る
		@return	スタート位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetStartPosition()const {
			return m_StartPosition;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	HPを得る
		@return	HP
		*/
		//--------------------------------------------------------------------------------------
		float GetHP()const {
			return m_HP;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	HPを設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHP(float f) {
			m_HP = f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MaxHPを得る
		@return	MaxHP
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxHP()const {
			return m_MaxHP;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MaxHPを設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxHP(float f) {
			m_MaxHP = f;
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
		virtual void Execute2(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	剣を振るステート
	//--------------------------------------------------------------------------------------
	class PlayerSwordState : public ObjState<Player>
	{
		PlayerSwordState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerSwordState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	剣を突くステート
	//--------------------------------------------------------------------------------------
	class PlayerSwordPokeState : public ObjState<Player>
	{
		PlayerSwordPokeState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerSwordPokeState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	敵の剣に当たったステート
	//--------------------------------------------------------------------------------------
	class PlayerSwordHit : public ObjState<Player>
	{
		PlayerSwordHit() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerSwordHit)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	倒されたステート
	//--------------------------------------------------------------------------------------
	class PlayerDieState : public ObjState<Player>
	{
		PlayerDieState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerDieState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//end basecross

