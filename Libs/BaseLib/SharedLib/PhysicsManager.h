/*!
@file PhysicsManager.h
@brief 物理計算用マネージャなど
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算マネージャ（ゲームオブジェクト）
	//--------------------------------------------------------------------------------------
	class PhysicsManager : public GameObject, public BasePhysics {
		shared_ptr<MeshResource> m_BoxMeshRes;
		shared_ptr<MeshResource> m_SphereMeshRes;
		void DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（デフォルトは何もしない）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のボックスの追加
		@param[in]	param	作成のパラメータ
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, const wstring& indexKey = L"")override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体の球体の追加
		@param[in]	param	作成のパラメータ
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param,const wstring& indexKey = L"")override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画（ワイアフレーム）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
// end basecross

