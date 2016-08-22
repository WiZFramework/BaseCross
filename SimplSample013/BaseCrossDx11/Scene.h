/*!
@file Scene.h
@brief シーンなど
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		shared_ptr<SquareObject> m_SquareObject;				///<平面オブジェクト
		shared_ptr<SphereObject> m_SphereObject;				///<球オブジェクト
		Vector3 m_CamerEye;			///<カメラ位置
		Vector3 m_CamerAt;			///<カメラ視点
		Vector3 m_CamerUp;			///<カメラ傾き
		Vector4 m_LightDir;			///<ライト向き
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビュー行列と射影行列の取得（各参照パラメータに代入）
		@param[out]	View	ビュー行列を受け取る参照
		@param[out]	Proj	射影行列を受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Matrix4X4& View, Matrix4X4& Proj)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト向きの取得
		@param[out]	LightDir	ライト向き受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vector4& LightDir)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
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
// end basecross
