/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public ObjectInterface {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		//テクスチャ
		shared_ptr<TextureResource>m_WallTex;
		Vector3 m_Scale;
		Vector3 m_Pos;
		Quaternion m_Qt;
	public:
		//構築と破棄
		NormalTextureBox(const Vector3& Pos);
		virtual ~NormalTextureBox();
		//初期化
		virtual void OnCreate()override;
		void OnUpdate();
		void OnDraw();
	};


}
//end basecross
