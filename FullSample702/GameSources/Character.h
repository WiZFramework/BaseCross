/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

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
	///	追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//ステートマシーン
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		float m_StateChangeSize;
		Vec3 m_Force;
		float m_MaxForce;
		float m_MaxSpeed;
		float m_Decl;
	protected:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
	public:
		//ステートマシンの取得
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine() {
			return m_StateMachine;
		}
		//ステートを変更するサイズを得る
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//目標までの距離を得る
		float GetTargetToLen() const;
		//現在の速度を得る（仮想関数）
		virtual Vec3 GetVelocity() const {
			return Vec3(0);
		}
		//現在のフォースを得る
		const Vec3& GetForce() const {
			return m_Force;
		}
		//シーク行動
		void SeekBehavior();
		//到着行動
		void ArriveBehavior();
		//分離行動
		void SeparationBehavior();
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};




	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな凸面オブジェクト
	//--------------------------------------------------------------------------------------
	class ActivePsConvex : public SeekObject {
		Vec3 m_Position;
		//メッシュ（描画用）
		static shared_ptr<MeshResource> m_ConvexMesh;
		//物理計算用
		static shared_ptr<PsConvexMeshResource> m_PsConvexMesh;
	public:
		//構築と破棄
		ActivePsConvex(const shared_ptr<Stage>& StagePtr,
			const Vec3& Position
		);
		virtual ~ActivePsConvex();
		//初期化
		virtual void OnCreate() override;
		//現在の速度を得る（仮想関数）
		virtual Vec3 GetVelocity() const;
		//更新
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public SeekObject {
		float m_Scale;
		Vec3 m_Position;
	public:
		//構築と破棄
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//初期化
		virtual void OnCreate() override;
		//現在の速度を得る（仮想関数）
		virtual Vec3 GetVelocity() const;
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな合成オブジェクト
	//--------------------------------------------------------------------------------------
	class ActivePsCombinedObject : public GameObject {
		Quat m_Qt;
		Vec3 m_Position;
		//メッシュ（カプセル）
		shared_ptr<MeshResource> m_CapsuleMesh;
		shared_ptr<MultiMeshResource> m_MultiMeshResource;
		//描画コンポーネントの作成
		void CreateDrawComp(const PsCombinedParam& param);
	public:
		//構築と破棄
		ActivePsCombinedObject(const shared_ptr<Stage>& StagePtr,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsCombinedObject();
		//初期化
		virtual void OnCreate() override;
	};



	//--------------------------------------------------------------------------------------
	///	物理計算する発射する球体
	//--------------------------------------------------------------------------------------
	class FirePsSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		void CreateDefParam(PsSphereParam& param);
	public:
		//構築と破棄
		FirePsSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FirePsSphere();
		//初期化
		virtual void OnCreate() override;
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};




}
//end basecross
