/*!
@file Player.h
@brief プレイヤー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		///メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		///テクスチャリソース名
		wstring m_TextureResName;
		///描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		///シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;

		//Transform情報
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Pos;
		//物理計算用のオブジェクト
		shared_ptr<PsSphere> m_PhysicsSphere;
		//進行方向を得る
		Vec3 GetMoveVector() const;
		//回転を設定する
		void RotToHead(const bsm::Vec3& Velocity, float LerpFact);
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr, const wstring& TextureResName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief 位置を得る
		@return	位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() override {
			return m_Pos;
		}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//シャドウマップ描画
		virtual void OnDrawShadowmap() override;
		//描画
		virtual void OnDraw()override;
		//Aボタンハンドラ
		void OnPushA();
		//Xボタンハンドラ
		void OnPushX();
	};

}
//end basecross
