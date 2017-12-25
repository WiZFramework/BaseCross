/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//文字列の表示
		void DrawStrings();
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//進行方向を得る
		Vec3 GetMoveVector() const;
		//スケーリング（直径の大きさ）
		float m_Scale;
		//ホールドしているオブジェクト
		weak_ptr<ActivePsObject> m_HoldObject;
		//ラインオブジェクト（weak_ptr）
		weak_ptr<ActionLine> m_ActionLine;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//アクセサ
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//Aボタンハンドラ
		void OnPushA();
		//Bボタンハンドラ
		void OnPushB();
		//Xボタンハンドラ
		void OnPushX();
		//Yボタンハンドラ(押した瞬間)
		void OnPushY();
		//Yボタンハンドラ（押し続け）
		void OnPressY();
		//Yボタンハンドラ(リリース)
		void OnReleaseY();
		//
		void SetHoldObject(const shared_ptr<ActivePsObject>& ptr) {
			m_HoldObject = ptr;
		}
	};

}
//end basecross

