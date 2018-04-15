/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	用途: タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	//	マスキングスプライト
	//--------------------------------------------------------------------------------------
	class MaskSprite : public GameObject {
		wstring m_TextureKey;
		shared_ptr<MeshResource> m_SpriteMesh;
	public:
		//構築と破棄
		MaskSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureKey
		);
		virtual ~MaskSprite();
		//初期化
		virtual void OnCreate() override;
		//操作
		//描画
		virtual void OnDraw() override;
	};




}
//end basecross
