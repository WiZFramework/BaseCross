/*!
@file DrawObjects.h
@brief 描画オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	PNT頂点描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct DrawObject {
		shared_ptr<MeshResource> m_MeshRes;
		shared_ptr<TextureResource> m_TextureRes;
		Mat4x4 m_WorldMatrix;
		bool m_Trace;
		bool m_Wrap;
		DrawObject() :
			m_MeshRes(nullptr),
			m_TextureRes(nullptr),
			m_WorldMatrix(),
			m_Trace(false),
			m_Wrap(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class PNTDrawObject : public GameObject {
		vector<DrawObject> m_DrawObjectVec;
		vector<DrawObject> m_TraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画サブ処理
		@param[in]	ObjectVec	描画する配列
		@param[in]	sb	コンスタントバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnDrawSub(vector<DrawObject>& ObjectVec, SimpleConstants& sb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		PNTDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	TextureRes テクスチャ
		@param[in]	WorldMat ワールド行列
		@param[in]	Trace 透明処理するかどうか
		@param[in]	Wrap ラッピング処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace, bool Wrap = false);
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
		virtual void OnUpdate()override {}
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
