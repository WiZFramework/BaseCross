/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		//メッシュ
		shared_ptr<MeshResource> m_Mesh;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		void CreateBuffers();
		void CollisionTest();			///<衝突判定テスト
		bool OnObjectTest(const SPHERE& Src, const OBB& Dest);	///<spがobbに乗ってるかどうか
		void CollisionEscape();			///<エスケープ処理

		OBB m_OnObb;
		bool m_OnObject;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player();
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
		@brief	最終更新（衝突判定などをチェックする）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate()override;

	};



}
//end basecross

