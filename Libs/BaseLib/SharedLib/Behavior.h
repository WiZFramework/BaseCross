/*!
@file Behavior.h
@brief 行動クラス
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス
	//--------------------------------------------------------------------------------------
	class Behavior : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Behavior(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Behavior();
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの取得
		@return	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージの取得
		@return	ゲームオブジェクトが所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化(空関数、必要なら多重定義する)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	行動ユーティリティクラス
	//--------------------------------------------------------------------------------------
	class UtilBehavior : public Behavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		UtilBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~UtilBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向くようにする
		@param[in]	LerpFact	補間係数（0.0から1.0の間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向くようにする(速度指定方式)
		@param[in]	LerpFact	補間係数（0.0から1.0の間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(const Vector3& Velocity,float LerpFact);

	};

	//--------------------------------------------------------------------------------------
	///	Jump行動クラス
	//--------------------------------------------------------------------------------------
	class JumpBehavior : public Behavior {
	public:
		JumpBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~JumpBehavior() {}
		void StartJump(const Vector3& FirstVelocity);
		bool Execute();
	};


	//--------------------------------------------------------------------------------------
	///	Seek行動クラス
	//--------------------------------------------------------------------------------------
	class SeekBehavior : public Behavior {
	public:
		SeekBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~SeekBehavior() {}
		void Enter(const Vector3& TargetPos);
		void Enter(const wstring& TargetKey);
		float Execute(const Vector3& TargetPos,bool RotHead, float LerpFact);
		float Execute(const wstring& TargetKey, bool RotHead, float LerpFact);
		void Exit();
	};

	//--------------------------------------------------------------------------------------
	///	Arrive行動クラス
	//--------------------------------------------------------------------------------------
	class ArriveBehavior : public Behavior {
	public:
		ArriveBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		virtual ~ArriveBehavior() {}
		void Enter(const Vector3& TargetPos);
		void Enter(const wstring& TargetKey);
		float Execute(const Vector3& TargetPos, bool RotHead, float LerpFact);
		float Execute(const wstring& TargetKey, bool RotHead, float LerpFact);
		void Exit();
	};


}
//end basecross
