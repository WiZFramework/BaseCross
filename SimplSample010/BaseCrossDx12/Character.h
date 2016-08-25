/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	立方体
	//--------------------------------------------------------------------------------------
	class CubeObject : public ObjectInterface, public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		// コンスタントバッファ
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		StaticConstantBuffer m_StaticConstantBuffer;
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Flat;				///<フラット表示するかどうか
		void CreateBuffers();
		void UpdateConstantBuffer();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	Pos	位置
		@param[in]	Flat	フラット表示にするかどうか
		*/
		//--------------------------------------------------------------------------------------
		CubeObject(const Vector3& Pos, bool Flat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
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
