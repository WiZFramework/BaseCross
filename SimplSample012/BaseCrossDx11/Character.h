/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	球
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		void CreateBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();
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
//end basecross
