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
		void SetStartObj(const shared_ptr<GameObject>& Obj) {
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
		//構築と破棄
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
	///	物理計算する煉瓦
	//--------------------------------------------------------------------------------------
	class PsBrick : public GameObject {
		//ステートマシーン
		unique_ptr<StateMachine<PsBrick>>  m_StateMachine;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_Mass;
		bool m_IsFixed;
		//自分についているジョイント
		weak_ptr<PsJoint> m_Joint;
		//ホールドカウント
		size_t m_HoldCount;
	public:
		//構築と破棄
		PsBrick(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float Mass,
			bool IsFixed
		);
		virtual ~PsBrick();
		//初期化
		virtual void OnCreate() override;
		//ステートマシーンを得る
		unique_ptr< StateMachine<PsBrick> >& GetStateMachine() {
			return m_StateMachine;
		}
		//ホールドの追加
		void AddHoldCount();
		//ホールドの解放
		void ResetHoldCount();
		//ホールド時の行動
		void SetHoldBehavior(bool b);
		//Fixかどうか
		bool IsFixed() const {
			return m_IsFixed;
		}
		//ジョイントの設定
		void SetJoint(const shared_ptr<PsJoint> joint) {
			m_Joint = joint;
		}
	};

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PsBrickDefaultState : public ObjState<PsBrick>
	{
		PsBrickDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PsBrickDefaultState)
		virtual void Enter(const shared_ptr<PsBrick>& Obj)override;
		virtual void Execute(const shared_ptr<PsBrick>& Obj)override;
		virtual void Exit(const shared_ptr<PsBrick>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	選択ステート
	//--------------------------------------------------------------------------------------
	class PsBrickHoldState : public ObjState<PsBrick>
	{
		PsBrickHoldState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PsBrickHoldState)
		virtual void Enter(const shared_ptr<PsBrick>& Obj)override;
		virtual void Execute(const shared_ptr<PsBrick>& Obj)override;
		virtual void Exit(const shared_ptr<PsBrick>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	/// ジョイントマネージャ
	//--------------------------------------------------------------------------------------
	class PsJointManager : public GameObject {
		shared_ptr<PsBallJoint> m_PsBallJoint;
		shared_ptr<PsSwingTwistJoint> m_PsSwingTwistJoint;
		shared_ptr<PsHingeJoint> m_PsHingeJoint;
		shared_ptr<PsSliderJoint> m_PsSliderJoint;
		shared_ptr<PsFixJoint> m_PsFixJoint;
		shared_ptr<PsUniversalJoint> m_PsUniversalJoint;
		void CreateBallJoint();
		void CreateSwingTwistJoint();
		void CreateHingeJoint();
		void CreateSliderJoint();
		void CreateFixJoint();
		void CreateUniversalJoint();
	public:
		//構築と破棄
		PsJointManager(const shared_ptr<Stage>& StagePtr);
		virtual ~PsJointManager();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	物理計算しない発射する球体
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		weak_ptr<PsBrick> m_HoldObject;
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
