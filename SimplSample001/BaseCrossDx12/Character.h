/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	三角形オブジェクト
	//--------------------------------------------------------------------------------------
	class TriangleObject : public ObjectInterface, public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		//メッシュ
		shared_ptr<MeshResource> m_TriangleMesh;
	public:
		//構築と破棄
		TriangleObject();
		virtual ~TriangleObject();
		//初期化
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;
	};



}
//end basecross
