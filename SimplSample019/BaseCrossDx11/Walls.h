/*!
@file Walls.h
@brief 壁など
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	頂点変更できるプレート
	//--------------------------------------------------------------------------------------
	class DynamicPlate : public GameObject {
	protected:
		//バックアップ頂点
		vector<VertexPositionNormalTexture> m_BackupVertices;
		//バックアップのインデックス（法線の計算に使う）
		vector<uint16_t> m_BackupIndices;
		//更新用の頂点
		vector<VertexPositionNormalTexture> m_UpdateVertices;
		//テクスチャリソース名
		wstring m_TextureResName;
		UINT m_WidthCount;
		UINT m_HeightCount;
		bool m_IsTile;	//タイリング
		Vector3 m_StartScale;
		Quaternion m_StartQt;
		Vector3 m_StartPosition;
		//ライト
		Light m_MyLight;
	protected:
		//構築と破棄
		DynamicPlate(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthCount,
			UINT HeightCount,
			bool IsTile,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~DynamicPlate();
	public:
		//初期化
		virtual void OnCreate() override;
		virtual const Light& OnGetDrawLight()const override;

		//平面を返す
		PLANE GetPLANE() const;
		//判定用のRECTを返す
		COLRECT GetCOLRECT() const;
		//点との最近接点を返す
		void ClosestPtPoint(const Vector3& Point, Vector3& Ret);
		//点との距離を返す（戻り値がマイナスなら裏側）
		float GetDistPointPlane(const Vector3& Point) const;
		//ヒットテストをして最近接点と、Squareの法線を返す
		bool HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal);
	};

	//--------------------------------------------------------------------------------------
	//	通り抜ける壁
	//--------------------------------------------------------------------------------------
	class ThroughWall : public DynamicPlate {
	public:
		//構築と破棄
		ThroughWall(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthCount,
			UINT HeightCount,
			bool IsTile,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~ThroughWall();
		//操作
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	タイリングする普通の壁
	//--------------------------------------------------------------------------------------
	class TileWall : public DynamicPlate {
		UINT m_WidthTileCount;
		UINT m_HeightTileCount;
	public:
		//構築と破棄
		TileWall(const shared_ptr<Stage>& StagePtr,
			wstring TextureResName,
			UINT WidthTileCount,
			UINT HeightTileCount,
			const Vector3& Scale,
			const Quaternion& Qt,
			const Vector3& Position
		);
		virtual ~TileWall();
		//初期化
		virtual void OnCreate() override;
	};


}
//end basecross
