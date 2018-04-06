/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SoundItem> m_BGM;
		//ビューの作成
		void CreateViewLight();
		//物理計算オブジェクトの作成
		void CreatePhysicsObjects();
		//スパークの作成
		void CreateSpark();
		//ボックスの作成
		void CreateBox();
		//プレイヤーの作成
		void CreatePlayer();
		//BGMの再生
		void PlayBGM();
		//マウス位置
		Point2D<int> m_MousePoint;
	public:
		//構築と破棄
		GameStage() :Stage(), m_MousePoint(0, 0) {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
		//削除時
		virtual void OnDestroy()override;
		//アクセサ
		Point2D<int> GetMousePoint()const {
			return m_MousePoint;
		}
		//マウスとカメラのレイの取得
		void GetMouseRay(Vec3& Near, Vec3& Far);
		//マウスによるオブジェクト選択のクリア
		void SelectClear();
		//マウスの左ボタン(押した瞬間)
		void OnLButtonEnter();
		//マウスの左ボタン(押し続け)
		void OnLButtonDown();
		//マウスの左ボタン(離した)
		void OnLButtonUp();
		//マウスの右ボタン(押した瞬間)
		void OnRButtonEnter();
	};

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateTitleSprite();
	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;

		//マウスの右ボタン(押した瞬間)
		void OnRButtonEnter();


	};

	//--------------------------------------------------------------------------------------
	//	ウエイトステージクラス（リソース読み込み用）
	//--------------------------------------------------------------------------------------
	class WaitStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateTitleSprite();
		//リソースロード用のスレッド（スタティック関数）
		static void LoadResourceFunc();
		//リソースを読み込んだことを知らせるフラグ（スタティック変数）
		static bool m_Loaded;
	public:
		//構築と破棄
		WaitStage() :Stage() {}
		virtual ~WaitStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};




}
//end basecross

