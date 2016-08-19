/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	四角形スプライト
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		struct SpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			SpriteConstantBuffer() {
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};
		SpriteConstantBuffer m_SpriteConstantBuffer;
		///メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		Vector2 m_Scale;				///<スケーリング
		float m_Rot;				///<回転角度
		Vector2 m_Pos;				///<位置
		Vector2 m_PosSpan;				///<位置変更間隔
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
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
