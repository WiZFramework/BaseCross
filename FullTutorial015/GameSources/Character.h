/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	アニメスプライト
	//--------------------------------------------------------------------------------------
	class AnimeSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		//トータル時間
		float m_TotalTime;
		//選択されているかどうか
		bool m_Selected;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//破棄
		virtual ~AnimeSprite();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate()override;
		//アクセサ
		bool IsSelect() const{
			return m_Selected;
		}
		void SetSelect(bool b) {
			m_Selected = b;
			m_TotalTime = 0.0f;
		}
	};


	//--------------------------------------------------------------------------------------
	//	用途: タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
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
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};



	//--------------------------------------------------------------------------------------
	///	敵１
	//--------------------------------------------------------------------------------------
	class Enemy1 : public GameObject {
		Vec3 m_StartPos;
		//ステートマシーン
		unique_ptr<StateMachine<Enemy1>>  m_StateMachine;
		//NearとFarを切り替える値
		const float m_NearFarChange;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Enemy1(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy1() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy1>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	NearとFarの境値を得る
		@return	NearとFarの境値
		*/
		//--------------------------------------------------------------------------------------
		float GetNearFarChange()const {
			return m_NearFarChange;
		}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy1のFarステート
	//--------------------------------------------------------------------------------------
	class Enemy1FarState : public ObjState<Enemy1>
	{
		Enemy1FarState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy1FarState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy1のNearステート
	//--------------------------------------------------------------------------------------
	class Enemy1NearState : public ObjState<Enemy1>
	{
		Enemy1NearState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy1NearState)
		virtual void Enter(const shared_ptr<Enemy1>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy1>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy1>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	敵2
	//--------------------------------------------------------------------------------------
	class Enemy2 : public GameObject {
		Vec3 m_StartPos;
		//ステートマシーン
		unique_ptr<StateMachine<Enemy2>>  m_StateMachine;
		//longとmediumを切り替える値
		const float m_LongMediumChange;
		//mediumとshortを切り替える値
		const float m_MediumShortChange;

	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Enemy2(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy2() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy2>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	LongとMediumの境値を得る
		@return	LongとMediumの境値
		*/
		//--------------------------------------------------------------------------------------
		float GetLongMediumChange() const {
			return m_LongMediumChange;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MediumとShortの境値を得る
		@return	MediumとShortの境値
		*/
		//--------------------------------------------------------------------------------------
		float GetMediumShortChange() const {
			return m_MediumShortChange;

		}

		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2のlongステート
	//--------------------------------------------------------------------------------------
	class Enemy2longState : public ObjState<Enemy2>
	{
		Enemy2longState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy2longState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	Enemy2のMediumステート
	//--------------------------------------------------------------------------------------
	class Enemy2MediumState : public ObjState<Enemy2>
	{
		Enemy2MediumState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy2MediumState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy2のShortステート
	//--------------------------------------------------------------------------------------
	class Enemy2ShortState : public ObjState<Enemy2>
	{
		Enemy2ShortState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy2ShortState)
		virtual void Enter(const shared_ptr<Enemy2>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy2>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy2>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	敵３
	//--------------------------------------------------------------------------------------
	class Enemy3 : public GameObject {
		Vec3 m_StartPos;
		//ステートマシーン
		unique_ptr<StateMachine<Enemy3>>  m_StateMachine;
		//defaultとnearを切り替える値
		const float m_DefaultNearChange;

	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Enemy3(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Enemy3() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<Enemy3>>& GetStateMachine() {
			return m_StateMachine;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DefaultとNearの境値を得る
		@return	DefaultとNearの境値
		*/
		//--------------------------------------------------------------------------------------
		float GetDefaultNearChange() const {
			return m_DefaultNearChange;

		}

		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3のDefaultステート
	//--------------------------------------------------------------------------------------
	class Enemy3DefaultState : public ObjState<Enemy3>
	{
		Enemy3DefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy3DefaultState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	Enemy3のNearステート
	//--------------------------------------------------------------------------------------
	class Enemy3NearState : public ObjState<Enemy3>
	{
		Enemy3NearState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(Enemy3NearState)
		virtual void Enter(const shared_ptr<Enemy3>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy3>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy3>& Obj)override;
	};




}
//end basecross
