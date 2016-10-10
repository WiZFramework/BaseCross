/*!
@file MeshHelper.h
@brief プリミティブ作成関数等
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	頂点関連のユーティリティ関数群(static呼び出しをす)
	//--------------------------------------------------------------------------------------
	class MeshUtill {
		static const float SQRT2;
		static const float SQRT3;
		static const float SQRT6;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Coneで使用されるヘルパー(分割数と分割インデックスから、ベクトルを得る)
		@param[in]	i	分割インデックス
		@param[in]	tessellation	分割数
		@return	ベクトル
		*/
		//--------------------------------------------------------------------------------------
		static inline XMVECTOR GetCircleVector(size_t i, size_t tessellation)
		{
			float angle = i * XM_2PI / tessellation;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			XMVECTORF32 v = { dx, 0, dz, 0 };
			return v;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Coneで使用されるヘルパー(分割数と分割インデックスから、Tangentベクトルを得る)
		@param[in]	i	分割インデックス
		@param[in]	tessellation	分割数
		@return	Tangentベクトル
		*/
		//--------------------------------------------------------------------------------------
		static inline XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
		{
			float angle = (i * XM_2PI / tessellation) + XM_PIDIV2;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, angle);

			XMVECTORF32 v = { dx, 0, dz, 0 };
			return v;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Cylinder / Coneで使用されるヘルパー(CylinderCapを作成する)
		@param[out]	vertices	頂点の配列
		@param[out]	indices インデックスの配列
		@param[in]	tessellation	分割数
		@param[in]	height	高さ
		@param[in]	radius	半径
		@param[in]	isTop	上部かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
			size_t tessellation, float height, float radius, bool isTop);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点とインデックスの向きを反転させる（LHとRH変換）
		@param[out]	indices インデックスの配列
		@param[out]	vertices	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReverseWinding(vector<uint16_t>& indices, vector<VertexPositionNormalTexture>& vertices)
		{
			if ((indices.size() % 3) != 0) {
				ThrowBaseException(
					L"インデックスの数が合いません",
					L"if((indices.size() % 3) == 0)",
					L"VertexUtil::ReverseWinding()"
				);
			}
			for (auto it = indices.begin(); it != indices.end(); it += 3)
			{
				std::swap(*it, *(it + 2));
			}

			for (auto it = vertices.begin(); it != vertices.end(); ++it)
			{
				it->textureCoordinate.x = (1.f - it->textureCoordinate.x);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの平面を作成する(static関数)
		@param[in]	size 1つの辺のサイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSquare(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの立方体を作成する(static関数)
		@param[in]	size 1つの辺のサイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCube(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの球体を作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateSphere(float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのカプセルを作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	PointA 中間部線分の開始点
		@param[in]	PointB 中間部線分の終了点
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCapsule(float diameter,
			const Vector3& PointA, const Vector3& PointB,
			size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのシリンダーを作成する(static関数)
		@param[in]	height 高さ
		@param[in]	diameter 直径
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCylinder(float height, float diameter, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのコーンを作成する(static関数)
		@param[in]	diameter 直径
		@param[in]	height 高さ
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateCone(float diameter, float height, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットのトーラスを作成する(static関数)
		@param[in]	diameter ドーナッツ中心までの直径
		@param[in]	thickness ドーナッツ太さ
		@param[in]	tessellation 分割数
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTorus(float diameter, float thickness, size_t tessellation,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正4面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateTetrahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正8面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateOctahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正12面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateDodecahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	VertexPosNormalTexフォーマットの正20面体を作成する(static関数)
		@param[in]	size サイズ
		@param[out]	vertices	頂点を作成するための配列
		@param[out]	indices	インデックスを作成するための配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateIcosahedron(float size,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices);
	};




}
//end basecross