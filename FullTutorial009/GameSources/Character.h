/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	固定のボックス
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
	//	敵
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
	protected:
		weak_ptr<StageCellMap> m_CelMap;
		Vector3 m_Scale;
		Vector3 m_StartRotation;
		Vector3 m_StartPosition;
		vector<CellIndex> m_CellPath;
		//現在の自分のセルインデックス
		int m_CellIndex;
		//めざす（次の）のセルインデックス
		int m_NextCellIndex;
		//ターゲットのセルインデックス
		int m_TargetCellIndex;
		shared_ptr<StateMachine<Enemy>> m_StateMachine;
		//進行方向を向くようにする
		void RotToHead();
	public:
		//構築と破棄
		Enemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<StageCellMap>& CellMap,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~Enemy();
		//プレイヤーの検索
		bool SearchPlayer();

		//デフォルト行動
		virtual bool DefaultBehavior();
		//Seek行動
		bool SeekBehavior();
		//アクセサ
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const {
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
	};

	//--------------------------------------------------------------------------------------
	///	デフォルトステート
	//--------------------------------------------------------------------------------------
	class EnemyDefault : public ObjState<Enemy>
	{
		EnemyDefault() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyDefault)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤーを追いかけるステート
	//--------------------------------------------------------------------------------------
	class EnemySeek : public ObjState<Enemy>
	{
		EnemySeek() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemySeek)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	セルマップを再設定する敵
	//--------------------------------------------------------------------------------------
	class TestCellChangeEnemy : public Enemy {
	public:
		//構築と破棄
		TestCellChangeEnemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<StageCellMap>& CellMap,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~TestCellChangeEnemy();
		//デフォルト行動
		virtual bool DefaultBehavior() override;
	};




}
//end basecross
