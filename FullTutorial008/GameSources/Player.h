/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class Player :  public SS5ssae;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public SS5ssae {
		shared_ptr< StateMachine<Player> >  m_StateMachine;	//ステートマシーン
															//移動の方向を得る
		float GetMoveX();
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
		//Zレールの位置の配列
		vector<float> m_ZRail;
		size_t m_ZRailIndex;
		//一つ前のレール
		size_t m_ZRailBeforIndex;
		//丸影を作成するためのメッシュ
		shared_ptr<MeshResource> m_ShadowResource;
		//データとゲームとの変換行列
		Matrix4X4 m_ToAnimeMatrixLeft;
		Matrix4X4 m_ToAnimeMatrixRight;
	public:
		//構築と破棄
		Player(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir);
		virtual ~Player() {}
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr< StateMachine<Player> > GetStateMachine() const {
			return m_StateMachine;
		}
		//モーションを実装する関数群
		//移動して向きを移動方向にする
		//移動距離を返す
		float MoveRotationMotion();

		void AnimeChangeMotion(const wstring& key, bool looped);
		void LoopedAnimeUpdateMotion();

		//Aボタンでジャンプするどうかを得る
		bool IsJumpMotion();
		//ジャンプスタート処理
		void JumpStartMotion();
		//ジャンプしている間の処理
		//ジャンプ終了したらtrueを返す
		bool JumpMotion();

		//Bボタンでアタックするどうかを得る
		bool IsAttackMotion();
		//Bボタンでアタックする処理
		bool AttackMotion();

		//左スティックでZレールを変更するかどうか
		bool IsRailChangeMotion();
		//Zレール変更を開始する
		void RailChangeStartMotion();
		//Zレール変更を更新する
		bool RailChangeMotion();
		//Zレールを強制的に戻す
		void RailChangeBeforStartMotion();
		//Zレール変更を終了する
		void RailChangeEndMotion();

		//更新
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};

	//--------------------------------------------------------------------------------------
	//	class WaitState : public ObjState<Player>;
	//	用途: 待機状態
	//--------------------------------------------------------------------------------------
	class WaitState : public ObjState<Player>
	{
		WaitState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<WaitState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: 歩き移動
	//--------------------------------------------------------------------------------------
	class WalkState : public ObjState<Player>
	{
		WalkState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<WalkState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class RailChangeState : public ObjState<Player>;
	//	用途: レール変更状態
	//--------------------------------------------------------------------------------------
	class RailChangeState : public ObjState<Player>
	{
		RailChangeState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<RailChangeState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	用途: ジャンプ状態
	//--------------------------------------------------------------------------------------
	class JumpState : public ObjState<Player>
	{
		JumpState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<JumpState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class RunState : public ObjState<Player>;
	//	用途: 走るアニメーション
	//--------------------------------------------------------------------------------------
	class RunState : public ObjState<Player>
	{
		RunState() {}
	public:
		static shared_ptr<RunState> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class AttackState : public ObjState<Player>;
	//	用途: アタックアニメーション
	//--------------------------------------------------------------------------------------
	class AttackState : public ObjState<Player>
	{
		AttackState() {}
	public:
		static shared_ptr<AttackState> Instance();
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};


}
//end basecross

