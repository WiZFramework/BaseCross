/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SquareObject> m_SquareObject;				///<平面オブジェクト
		shared_ptr<Player> m_Player;				///<プレイヤー

		shared_ptr<PNTDrawObject>	m_PNTDrawObject;				///<描画オブジェクト
		shared_ptr<RotateSprite> m_RotateSprite;		///<回転するスプライト
		shared_ptr<MessageSprite> m_MessageSprite;		///<メッセージを表示するスプライト


		Vec4 m_LightDir;		///<ライト向き
		Camera m_Camera;		///<カメラ
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GameStage();
		virtual ~GameStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの取得
		@return	カメラの参照（内容変更できない）
		*/
		//--------------------------------------------------------------------------------------
		const Camera& GetCamera()const {
			return m_Camera;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの取得
		@return	カメラの参照（内容変更できる）
		*/
		//--------------------------------------------------------------------------------------
		Camera& GetCamera(){
			return m_Camera;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief プレイヤーの取得
		@return	プレイヤー
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Player> GetPlayer() const {
			return m_Player;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画オブジェクトの取得
		@return	描画オブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PNTDrawObject> GetPNTDrawObject() const {
			return m_PNTDrawObject;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト向きの取得
		@param[out]	LightDir	ライト向き受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vec4& LightDir)const {
			LightDir = m_LightDir;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ描画（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	class EmptyStage : public Stage {
		shared_ptr<MessageSprite> m_MessageSprite;		///<メッセージを表示するスプライト
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		EmptyStage() {}
		virtual ~EmptyStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ描画（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};




}
//end basecross
