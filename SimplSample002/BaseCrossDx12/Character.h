/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	三角形スプライト
	//--------------------------------------------------------------------------------------
	class TriangleSprite : public ObjectInterface, public ShapeInterface {
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
		shared_ptr<MeshResource> m_TriangleMesh;
		Vector3 m_Pos;					///<現在の位置
		Vector3 m_MoveSpan;				///<位置変更値
	public:
		//構築と破棄
		TriangleSprite();
		virtual ~TriangleSprite();
		//初期化
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;
	};



}
//end basecross
