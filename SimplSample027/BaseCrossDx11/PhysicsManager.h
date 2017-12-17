/*!
@file PhysicsManager.h
@brief 物理計算用マネージャなど
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	struct SimpleDrawObject;
	class SimplePCStaticRenderer;

	//--------------------------------------------------------------------------------------
	///	物理計算マネージャ（ゲームオブジェクト）
	//--------------------------------------------------------------------------------------
	class PhysicsManager : public GameObject, public BasePhysics {
		shared_ptr<MeshResource> m_BoxMeshRes;
		shared_ptr<MeshResource> m_SphereMeshRes;
		//描画データの配列
		vector<shared_ptr<SimpleDrawObject>> m_PtrObjVec;
		//描画オブジェクト(weak_ptr)
		weak_ptr<SimplePCStaticRenderer> m_Renderer;
		void DrawShapeWireFrame(uint16_t index,const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world);
		void CreateDrawObject();
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
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のボックスの追加
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, uint16_t index = UINT16_MAX)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体の球体の追加
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param, uint16_t index = UINT16_MAX)override;
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

