/*!
@file Enemy.h
@brief 敵など
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class EnemyInterface {
	public:
		EnemyInterface() {}
		virtual ~EnemyInterface() {}
	};

	//--------------------------------------------------------------------------------------
	//	class HeadObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class HeadEnemy : public GameObject ,public EnemyInterface {
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//行動マシーン
		shared_ptr< BehaviorMachine<HeadEnemy> >  m_BehaviorMachine;
	public:
		//ユーティリティ関数群
		float GetStateChangeSize()const {
			return m_StateChangeSize;
		}
		//プレイヤーの位置を返す
		Vector3 GetPlayerPosition() const;
		//プレイヤーまでの距離を返す
		float GetPlayerLength() const;
		//回転を進行方向に向かせる
		void RotToFront();
		//構築と破棄
		HeadEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~HeadEnemy();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr<BehaviorMachine<HeadEnemy>> GetBehaviorMachine() const {
			return m_BehaviorMachine;
		}
		//操作
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	静止行動
	//--------------------------------------------------------------------------------------
	class HeadStop : public Behavior<HeadEnemy> {
		float m_StopTime;
	public:
		HeadStop() :m_StopTime(0){}
		virtual ~HeadStop() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void WakeUp(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<HeadEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	ジャンプして追跡行動
	//--------------------------------------------------------------------------------------
	class HeadJumpSeek : public Behavior<HeadEnemy> {
	public:
		HeadJumpSeek() {}
		virtual ~HeadJumpSeek() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Execute2(const shared_ptr<HeadEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<HeadEnemy>& Obj) override;
	};


	//--------------------------------------------------------------------------------------
	//	もぞもぞした敵
	//--------------------------------------------------------------------------------------
	class MozoEnemy : public GameObject, public EnemyInterface {
		Vector3 m_StartPos;
		float m_BaseY;
		//行動マシーン
		shared_ptr< BehaviorMachine<MozoEnemy> >  m_BehaviorMachine;
		//ライト
		Light m_MyLight;
		void CreateMeshRes();
	public:
		//ユーティリティ関数群
		//プレイヤーの位置を返す
		Vector3 GetPlayerPosition() const;
		//プレイヤーまでの距離を返す
		float GetPlayerLength() const;
		//スタート時の位置を返す
		Vector3 GetStartPosition() const {
			return m_StartPos;
		}
		//構築と破棄
		MozoEnemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~MozoEnemy();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr<BehaviorMachine<MozoEnemy>> GetBehaviorMachine() const {
			return m_BehaviorMachine;
		}
		//操作
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnLastUpdate() override;
		//独自のライト
		virtual const Light& OnGetDrawLight()const override;
	};

	//--------------------------------------------------------------------------------------
	//	スタート行動
	//--------------------------------------------------------------------------------------
	class MozoStart : public Behavior<MozoEnemy> {
	public:
		MozoStart(){}
		virtual ~MozoStart() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute2(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<MozoEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	デフォルト行動
	//--------------------------------------------------------------------------------------
	class MozoDefault : public Behavior<MozoEnemy> {
		bool m_IsInit;
	public:
		MozoDefault():m_IsInit(false){}
		virtual ~MozoDefault() {}
		virtual void OnCreateWithParam() {}
		virtual void Enter(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Sleep(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void WakeUp(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Execute(const shared_ptr<MozoEnemy>& Obj) override;
		virtual void Exit(const shared_ptr<MozoEnemy>& Obj) override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedHead : public GameObject;
	//	用途: 固定の頭
	//--------------------------------------------------------------------------------------
	class FixedHead : public GameObject {
		Vector3 m_Position;
	public:
		//構築と破棄
		FixedHead(const shared_ptr<Stage>& StagePtr,
			const Vector3& Position
		);
		virtual ~FixedHead();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class FixedCylinder : public GameObject;
	//	用途: 固定のシリンダー
	//--------------------------------------------------------------------------------------
	class FixedCylinder : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		bool m_BalloonActive;
	public:
		//構築と破棄
		FixedCylinder(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedCylinder();
		//初期化
		virtual void OnCreate() override;
		//操作
		void BalloonObOff();
	};




}

//end basecross