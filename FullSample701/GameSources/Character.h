/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ライン
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		weak_ptr<GameObject> m_StartObj;
		weak_ptr<GameObject> m_EndObj;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartObj	スタートのオブジェクト
		@param[in]	EndObj	エンドのオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, const shared_ptr<GameObject>& StartObj,
			const shared_ptr<GameObject>& EndObj);
		virtual ~ActionLine() {}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//線のスタートオブジェクトの取得
		shared_ptr<GameObject> GetStartObj() const {
			return m_StartObj.lock();
		}
		//線のスタートオブジェクトの設定
		void SetStartObj(const shared_ptr<GameObject>& Obj){
			m_StartObj = Obj;
		}
		//線のエンドオブジェクトの取得
		shared_ptr<GameObject> GetEndObj() const {
			return m_EndObj.lock();
		}
		//線のエンドオブジェクトの設定
		void SetEndObj(const shared_ptr<GameObject>& Obj) {
			m_EndObj = Obj;
		}
		//線のスタートとエンドオブジェクトの設定
		void ResetObject(const shared_ptr<GameObject>& StartObj, const shared_ptr<GameObject>& EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
	};


	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Scale	スケール
		@param[in]	Qt	回転
		@param[in]	Position	位置
		*/
		//--------------------------------------------------------------------------------------
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなオブジェクトの親
	//--------------------------------------------------------------------------------------
	class ActivePsObject : public GameObject {
		//ステートマシーン
		unique_ptr<StateMachine<ActivePsObject>>  m_StateMachine;
	protected:
		ActivePsObject(const shared_ptr<Stage>& StagePtr):
			GameObject(StagePtr)
		{
		}
		virtual ~ActivePsObject() {}
		//初期化
		virtual void OnCreate() override;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr< StateMachine<ActivePsObject> >& GetStateMachine() {
			return m_StateMachine;
		}
		//ホールドの設定
		void SetHold(bool b);
		//ホールド時の行動（仮想関数）
		virtual void SetHoldBehavior(bool b) {}
	};

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class ActivePsDefaultState : public ObjState<ActivePsObject>
	{
		ActivePsDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(ActivePsDefaultState)
		virtual void Enter(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Execute(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Exit(const shared_ptr<ActivePsObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	選択ステート
	//--------------------------------------------------------------------------------------
	class ActivePsHoldState : public ObjState<ActivePsObject>
	{
		ActivePsHoldState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(ActivePsHoldState)
		virtual void Enter(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Execute(const shared_ptr<ActivePsObject>& Obj)override;
		virtual void Exit(const shared_ptr<ActivePsObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public ActivePsObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Scale	スケール
		@param[in]	Qt	回転
		@param[in]	Position	位置
		*/
		//--------------------------------------------------------------------------------------
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//初期化
		virtual void OnCreate() override;
		//ホールド時の行動（仮想関数）
		virtual void SetHoldBehavior(bool b) override;
	};




	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public ActivePsObject {
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Scale	スケール
		@param[in]	Qt	回転
		@param[in]	Position	位置
		*/
		//--------------------------------------------------------------------------------------
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//初期化
		virtual void OnCreate() override;
		//ホールド時の行動（仮想関数）
		virtual void SetHoldBehavior(bool b) override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなカプセル
	//--------------------------------------------------------------------------------------
	class ActivePsCapsule : public ActivePsObject {
		//長さ
		float m_Len;
		//直径
		float m_Diameter;
		Quat m_Qt;
		Vec3 m_Position;
		//メッシュ（カプセルは各オブジェクトが保持）
		shared_ptr<MeshResource> m_CapsuleMesh;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Len	長さ
		@param[in]	Diameter	直径
		@param[in]	Qt	回転
		@param[in]	Position	位置
		*/
		//--------------------------------------------------------------------------------------
		ActivePsCapsule(const shared_ptr<Stage>& StagePtr,
			float Len,
			float Diameter,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCapsule();
		//初期化
		virtual void OnCreate() override;
		//ホールド時の行動（仮想関数）
		virtual void SetHoldBehavior(bool b) override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算しない発射する球体
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		weak_ptr<ActivePsObject> m_HoldObject;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Emitter	発射位置
		@param[in]	Velocity	速度
		*/
		//--------------------------------------------------------------------------------------
		FireSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FireSphere();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};

}
//end basecross
