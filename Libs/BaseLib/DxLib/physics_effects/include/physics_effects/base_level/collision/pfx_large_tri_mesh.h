/*
Physics Effects Copyright(C) 2012 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/

#pragma once
#include "stdafx.h"

#include "pfx_tri_mesh.h"

namespace sce {
namespace PhysicsEffects {

#define SCE_PFX_MAX_LARGETRIMESH_ISLANDS 512

#define SCE_PFX_LARGE_MESH_TYPE_EXPANDED_ARRAY  0x00
#define SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_ARRAY 0x01
#define SCE_PFX_LARGE_MESH_TYPE_EXPANDED_BVH    0x02
#define SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_BVH   0x03

///////////////////////////////////////////////////////////////////////////////
// Large Mesh

struct SCE_PFX_ALIGNED(16) PfxLargeTriMesh
{
	//J ラージメッシュの種別を決める
	//E Defines the structure of the large mesh
	PfxUInt32 m_type;

	SCE_PFX_PADDING(1,12)

	//J オフセット座標
	//E Offset position
	PfxVector3 m_offset;
	
	//J ラージメッシュのサイズ
	//E Size of a large mesh
	PfxVector3 m_half;
	PfxFloat m_length; //E length of m_half
	
	//J 含まれるアイランドの総数
	//E Number of islands
	PfxUInt16 m_numIslands;
	
	SCE_PFX_PADDING(2,2)

	//J アイランドAABB配列
	//E Array of island AABB
	PfxAabb16 *m_aabbList;
	
	// Number of islands
	PfxUInt32 m_numBvhNodes;

	// BVH tree
	PfxAabb16 *m_bvhNodes;
	PfxUInt32 m_bvhRootId;

	//J バッファ
	//E Buffer
	/*
		type                                   |facet                |vertex         |island
		---------------------------------------|---------------------|---------------|------------------------
		SCE_PFX_LARGE_MESH_TYPE_EXPANDED_ARRAY |PfxExpandedFacet     |PfxVector3     |PfxExpandedTriMesh
		SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_ARRAY|PfxQuantizedFacet    |PfxQuantize3   |PfxQuantizedTriMesh
		SCE_PFX_LARGE_MESH_TYPE_EXPANDED_BVH   |PfxExpandedFacetBvh  |PfxVector3     |PfxExpandedTriMeshBvh
		SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_BVH  |PfxQuantizedFacetBvh |PfxQuantize3   |PfxQuantizedTriMeshBvh
	 */
	
	void *m_facetBuffer;   // PfxExpandedFacet / PfxQuantizedFacet
	void *m_edgeBuffer;    // PfxEdge
	void *m_vertexBuffer;  // PfxVector3 / PfxQuantize3
	void *m_bvhNodeBuffer; // PfxAabb16
	void *m_islands;	   // PfxQuantizedTriMesh / PfxExpandedTriMesh / PfxQuantizedTriMeshBvh / PfxExpandedTriMeshBvh

	PfxUInt32 m_facetBuffBytes;
	PfxUInt32 m_edgeBuffBytes;
	PfxUInt32 m_vertexBuffBytes;
	PfxUInt32 m_bvhNodeBuffBytes;

	SCE_PFX_PADDING(3,4)

	PfxLargeTriMesh()
	: m_type(0)
	, m_numIslands(0)
	, m_aabbList(0)
	, m_numBvhNodes(0)
	, m_bvhNodes(0)
	, m_bvhRootId(0)
	, m_facetBuffer(0)
	, m_edgeBuffer(0)
	, m_vertexBuffer(0)
	, m_bvhNodeBuffer(0)
	, m_islands(0)
	, m_facetBuffBytes(0)
	, m_edgeBuffBytes(0)
	, m_vertexBuffBytes(0)
	, m_bvhNodeBuffBytes(0)
	{
	}
	
	//J ワールド座標値をラージメッシュローカルに変換する
	//E Convert a position in the world coordinate into a position in the local coordinate
	inline PfxVecInt3 getLocalPosition(const PfxVector3 &worldPosition) const;
	inline void getLocalPosition(
		const PfxVector3 &worldMinPosition,const PfxVector3 &worldMaxPosition,
		PfxVecInt3 &localMinPosition,PfxVecInt3 &localMaxPosition) const;
	
	//J ラージメッシュローカル座標値をワールドに変換する
	//E Convert a position in the local coordinate into a position in the world coordinate
	inline PfxVector3 getWorldPosition(const PfxVecInt3 &localPosition) const;

	//J 量子化
	//E Quantize
	inline PfxQuantize3 quantizePosition(const PfxVector3 &p) const;
	inline PfxQuantize3 quantizeVector(const PfxVector3 &v) const;
	inline PfxQuantize2 quantizeNormal(const PfxVector3 &n) const;
	inline PfxQuantize  quantizeFloat(PfxFloat value) const;
	
	inline PfxVector3 decodePosition(const PfxQuantize3 &q) const;
	inline PfxVector3 decodeVector(const PfxQuantize3 &q) const;
	inline PfxVector3 decodeNormal(const PfxQuantize2 &q) const;
	inline PfxFloat   decodeFloat(PfxQuantize q) const;
	
	/**
	 * whether the large meshes are operated using BVH or not
	 * 
	 * @return true if using BVH
	 */
	inline PfxBool isUsingBvh() const {return (m_type&0x02)!=0;}

	/**
	 * whether the large meshes are compressd by quantized buffers
	 * 
	 * @return true if quantized
	 */
	inline PfxBool isQuantized() const {return (m_type&0x01)!=0;}
};

inline
PfxVecInt3 PfxLargeTriMesh::getLocalPosition(const PfxVector3 &worldPosition) const 
{
	const PfxVector3 sz(65535.0f);
	PfxVector3 tmp = divPerElem(worldPosition-m_offset+m_half,2.0f*m_half);
	tmp = mulPerElem(sz,minPerElem(maxPerElem(tmp,PfxVector3(0.0f)),PfxVector3(1.0f))); // clamp 0.0 - 1.0
	return PfxVecInt3(tmp);
}

inline
void PfxLargeTriMesh::getLocalPosition(
		const PfxVector3 &worldMinPosition,const PfxVector3 &worldMaxPosition,
		PfxVecInt3 &localMinPosition,PfxVecInt3 &localMaxPosition) const
{
	const PfxVector3 sz(65535.0f);
	PfxVector3 qmin = divPerElem(worldMinPosition-m_offset+m_half,2.0f*m_half);
	qmin = mulPerElem(sz,minPerElem(maxPerElem(qmin,PfxVector3(0.0f)),PfxVector3(1.0f))); // clamp 0.0 - 1.0

	PfxVector3 qmax = divPerElem(worldMaxPosition-m_offset+m_half,2.0f*m_half);
	qmax = mulPerElem(sz,minPerElem(maxPerElem(qmax,PfxVector3(0.0f)),PfxVector3(1.0f))); // clamp 0.0 - 1.0

localMinPosition = PfxVecInt3(floorf(qmin[0]),floorf(qmin[1]),floorf(qmin[2]));
localMaxPosition = PfxVecInt3(ceilf(qmax[0]),ceilf(qmax[1]),ceilf(qmax[2]));
}

inline
PfxVector3 PfxLargeTriMesh::getWorldPosition(const PfxVecInt3 &localPosition) const 
{
	PfxVector3 sz(65535.0f),lp(localPosition);
	PfxVector3 tmp = divPerElem(lp,sz);
	return mulPerElem(tmp,2.0f*m_half)+m_offset-m_half;
}

inline PfxQuantize3 PfxLargeTriMesh::quantizePosition(const PfxVector3 &p) const
{
	const PfxVector3 sz(SCE_PFX_QUANTIZE_MAX);
	PfxVector3 tmp = divPerElem(p-m_offset,2.0f*m_half);
	tmp = mulPerElem(sz,minPerElem(maxPerElem(tmp,PfxVector3(-0.5f)),PfxVector3(0.5f))); // clamp -0.5 , 0.5
	return PfxQuantize3((PfxInt16)tmp[0],(PfxInt16)tmp[1],(PfxInt16)tmp[2]);
}

inline PfxQuantize3 PfxLargeTriMesh::quantizeVector(const PfxVector3 &v) const
{
	const PfxVector3 sz(SCE_PFX_QUANTIZE_MAX);
	PfxVector3 tmp = divPerElem(v,2.0f*m_half);
	tmp = mulPerElem(sz,minPerElem(maxPerElem(tmp,PfxVector3(-0.5f)),PfxVector3(0.5f))); // clamp -0.5 , 0.5
	return PfxQuantize3((PfxInt16)tmp[0],(PfxInt16)tmp[1],(PfxInt16)tmp[2]);
}

inline PfxQuantize2 PfxLargeTriMesh::quantizeNormal(const PfxVector3 &n) const
{
	const PfxFloat PI2 = SCE_PFX_PI * 2.0f;
	PfxFloat r1=0.0f,r2=0.0f;
	r1 = acosf(n[2]); // ０～π
	r2 = atan2f(n[1],n[0]); // -π～π
	return PfxQuantize2(
		(PfxInt16)(((r1-0.5f*SCE_PFX_PI)/SCE_PFX_PI)*SCE_PFX_QUANTIZE_MAX),
		(PfxInt16)((r2/PI2)*SCE_PFX_QUANTIZE_MAX));
}

inline PfxQuantize PfxLargeTriMesh::quantizeFloat(PfxFloat value) const
{
	PfxFloat tmp = value / (2.0f*m_length);
	tmp = SCE_PFX_QUANTIZE_MAX * SCE_PFX_CLAMP(tmp,-0.5f,0.5f);
	return PfxQuantize((PfxInt16)tmp);
}

inline PfxVector3 PfxLargeTriMesh::decodePosition(const PfxQuantize3 &q) const
{
	const PfxVector3 sz(SCE_PFX_QUANTIZE_MAX),lp((PfxFloat)q.elem[0],(PfxFloat)q.elem[1],(PfxFloat)q.elem[2]);
	PfxVector3 tmp = divPerElem(lp,sz);
	return mulPerElem(tmp,2.0f*m_half)+m_offset;
}

inline PfxVector3 PfxLargeTriMesh::decodeVector(const PfxQuantize3 &q) const
{
	const PfxVector3 sz(SCE_PFX_QUANTIZE_MAX),lp((PfxFloat)q.elem[0],(PfxFloat)q.elem[1],(PfxFloat)q.elem[2]);
	PfxVector3 tmp = divPerElem(lp,sz);
	return mulPerElem(tmp,2.0f*m_half);
}

inline PfxVector3 PfxLargeTriMesh::decodeNormal(const PfxQuantize2 &q) const
{
	const PfxFloat PI2 = SCE_PFX_PI * 2.0f;
	PfxFloat rr1=0.0f,rr2=0.0f;
	rr1 = ((q.elem[0]/SCE_PFX_QUANTIZE_MAX)*SCE_PFX_PI) + 0.5f*SCE_PFX_PI;
	rr2 = ((q.elem[1]/SCE_PFX_QUANTIZE_MAX)*PI2);
	
	return PfxVector3(
		sinf(rr1)*cosf(rr2),
		sinf(rr1)*sinf(rr2),
		cosf(rr1));
}

inline PfxFloat PfxLargeTriMesh::decodeFloat(PfxQuantize q) const
{
	PfxFloat tmp = q.elem / SCE_PFX_QUANTIZE_MAX;
	return (tmp * 2.0f * m_length);
}

} // namespace PhysicsEffects
} // namespace sce

