/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface,public ShapeInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public ObjectInterface,public ShapeInterface {
		//描画コンテキスト
		shared_ptr<VSPSDrawContext> m_DrawContext;
		//コンスタントバッファ構造体
		struct ConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			ConstantBuffer() {
				memset(this, 0, sizeof(ConstantBuffer));
			};
		};
		//コンスタントバッファデータ
		ConstantBuffer m_ConstantBufferData;
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
		void OnUpdate()override;
		void OnDraw()override;
	};



}
//end basecross
