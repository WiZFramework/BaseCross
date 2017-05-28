/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	用途: 剣
	//--------------------------------------------------------------------------------------
	class Sword : public GameObject {
		Vector3 m_Scale;
		wstring m_Tag;
	public:
		//構築と破棄
		Sword(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const wstring& Tag
		);
		virtual ~Sword();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	用途: タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vector3 m_Scale;
		Quaternion m_Qt;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	// 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vector3& Pos);
	};

	//--------------------------------------------------------------------------------------
	// 複数の炎クラス
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//構築と破棄
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//初期化
		virtual void OnCreate() override;
		void InsertFire(const Vector3& Pos);
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
		shared_ptr<StateMachine<Enemy>> m_StateMachine;
		//進行方向を向くようにする
		void RotToHead();
		//プレイヤー方向を向くようにする
		void RotToPlayer();
		//セル検索の行動クラス
		unique_ptr<CellSearchBehavior<Enemy>> m_CellSearchBehavior;
		//戦いの行動
		unique_ptr<FightBehavior<Enemy>> m_FightBehavior;
		//マックスHP
		float m_MaxHP;
		//現在のHP
		float m_HP;
	public:
		//構築と破棄
		Enemy(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<StageCellMap>& CellMap,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~Enemy();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セル検索の行動を得る
		@return	セル検索の行動
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<CellSearchBehavior<Enemy>>& GetCellSearchBehavior() {
			return m_CellSearchBehavior;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	戦い行動を得る
		@return	戦い行動
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<FightBehavior<Enemy>>& GetFightBehavior() {
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
		@brief	ステートマシンを得る
		@return	ステートマシン位置
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const {
			return m_StateMachine;
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
		//操作
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
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
	///	敵が剣を振るステート
	//--------------------------------------------------------------------------------------
	class EnemySwordState : public ObjState<Enemy>
	{
		EnemySwordState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemySwordState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤーの剣に当たったステート
	//--------------------------------------------------------------------------------------
	class EnemySwordHit : public ObjState<Enemy>
	{
		EnemySwordHit() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemySwordHit)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	倒されたステート
	//--------------------------------------------------------------------------------------
	class EnemyDieState : public ObjState<Enemy>
	{
		EnemyDieState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(EnemyDieState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};



	//--------------------------------------------------------------------------------------
	//	HPのスクエアの領域
	//--------------------------------------------------------------------------------------
	class HPSquareBase : public GameObject {
	public:
		//構築と破棄
		HPSquareBase(const shared_ptr<Stage>& StagePtr);
		virtual ~HPSquareBase();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	HPのスクエア
	//--------------------------------------------------------------------------------------
	class HPSquare : public GameObject {
		Color4 m_Color;
	public:
		//構築と破棄
		HPSquare(const shared_ptr<Stage>& StagePtr,const Color4& Col);
		virtual ~HPSquare();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	HPのマネージャ(テンプレートクラス)
	//--------------------------------------------------------------------------------------
	template<typename T>
	class HPManeger : public GameObject {
		weak_ptr<T> m_TargetObj;
		weak_ptr<HPSquareBase> m_HPSquareBase;
		weak_ptr<HPSquare> m_HPSquare;
		Color4 m_Color;

		void UpdateHPTrans() {
			auto ShTarget = m_TargetObj.lock();
			auto ShHPSquareBase = m_HPSquareBase.lock();
			auto ShHPSquare = m_HPSquare.lock();
			if (ShTarget && ShHPSquareBase && ShHPSquare) {
				auto HPBaseTrans = ShHPSquareBase->GetComponent<Transform>();
				auto HPTrans = ShHPSquare->GetComponent<Transform>();
				//スケーリング
				float MaxHP = ShTarget->GetMaxHP();
				float HP = ShTarget->GetHP();
				float Width = HP / MaxHP;

				HPBaseTrans->SetScale(0.5f, 0.125f, 0.5);
				HPTrans->SetScale(0.46f * Width, 0.1f, 0.46f);

				auto TargetTrans = ShTarget->GetComponent<Transform>();
				auto Pos = TargetTrans->GetPosition();
				Pos.y += 0.75f;
				HPBaseTrans->SetPosition(Pos);
				Pos = Vector3(0, 0, -0.001f);
				Pos.x -= 0.46f * 0.5f;
				Pos.x += (0.46f * Width * 0.5f);
				HPTrans->SetPosition(Pos);


 				auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
				Quaternion Qt;
				//向きをビルボードにする
				Qt.Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());
				HPBaseTrans->SetQuaternion(Qt);
			}
		}
	public:
		//構築と破棄
		HPManeger(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<T>& TargetObj,
			const Color4& Col):
			GameObject(StagePtr),
			m_TargetObj(TargetObj),
			m_Color(Col)
		{}

		virtual ~HPManeger() {}
		//初期化
		virtual void OnCreate() override {
			auto HPBase = GetStage()->AddGameObject<HPSquareBase>();
			auto HP = GetStage()->AddGameObject<HPSquare>(m_Color);
			auto PtrTrans = HP->GetComponent<Transform>();
			PtrTrans->SetParent(HPBase);

			m_HPSquareBase = HPBase;
			m_HPSquare = HP;
			UpdateHPTrans();
		}

		virtual void OnUpdate2() override {
			UpdateHPTrans();
		}


	};




}
//end basecross
