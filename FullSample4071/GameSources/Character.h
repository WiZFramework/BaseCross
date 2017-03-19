/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	カスタムシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap {
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);

		virtual ~CustomShadowmap();
		//操作
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	//	ジオメトリシェーダを使った独自の描画コンポーネント
	//--------------------------------------------------------------------------------------
	class CustomPNCStaticDraw : public StaticBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CustomPNCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CustomPNCStaticDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	//	独自の描画コンポーネントで描画する8面体
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject {
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//バックアップ用の頂点配列
		vector<VertexPositionNormalColor> m_BackupVertices;
		//頂点の変更
		void UpdateCustomMesh();
		//経過時間
		float m_TotalTime;
	public:
		//構築と破棄
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawOctahedron();
		//アクセサ

		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


}
//end basecross
