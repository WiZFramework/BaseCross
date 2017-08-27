/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	コントローラのボタンのハンドラ
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void PushHandle(const shared_ptr<T>& Obj) {
			//コントローラの取得
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//Bボタン
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
					Obj->OnPushB();
				}

			}
		}
	};



	//--------------------------------------------------------------------------------------
	///	プレイヤーの行動クラス
	//--------------------------------------------------------------------------------------
	class PlayerBehavior : public Behavior {
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コントローラから方向ベクトルを得る
		@return	方向ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		PlayerBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_MaxSpeed(10.0f),	//最高速度
			m_Decel(0.95f),	//減速値
			m_Mass(0.5f)	//質量
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PlayerBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	最高速度を得る
		@return	最高速度
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減速値を得る
		@return	減速値
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	質量を得る
		@return	質量
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プレイヤーを移動させる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void MovePlayer();
		void StopPlayer();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	アタックボールを発射する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void FireAttackBall();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveBox上に衝突しているかどうかをチェックする
		@param[in]	OtherVec	衝突オブジェクトの配列
		@param[out]	retv	目的の衝突オブジェクトの参照
		@return	MoveBox上に衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool OnHitObjMoveBox(vector<shared_ptr<GameObject>>& OtherVec, shared_ptr<GameObject>& retv);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveBox上にいるかどうかをチェックする
		@return	MoveBox上にいればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool OnMoveBox();
	};




}

//end basecross

