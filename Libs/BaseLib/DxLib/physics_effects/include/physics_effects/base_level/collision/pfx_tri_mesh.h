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

#include "../base/pfx_common.h"
#include "../base/pfx_vec_utils.h"
#include "pfx_aabb.h"

//J メッシュのリソース制限
//E Define some limitations of a triangle mesh
#define SCE_PFX_NUMMESHFACETS		64
#define SCE_PFX_NUMMESHEDGES		192
#define SCE_PFX_NUMMESHVERTICES		128

//J エッジの角
//E Edge types
#define SCE_PFX_EDGE_FLAT    0
#define SCE_PFX_EDGE_CONVEX  1
#define SCE_PFX_EDGE_CONCAVE 2

#define SCE_PFX_QUANTIZE_MAX 65535.0f

namespace sce {
namespace PhysicsEffects {

///////////////////////////////////////////////////////////////////////////////
// PfxQuantize

struct PfxQuantize
{
	PfxInt16 elem;
	
	PfxQuantize(PfxInt16 value)
	{
		elem = value;
	}
};

struct PfxQuantize2
{
	PfxInt16 elem[2];

	PfxQuantize2(PfxInt16 value1,PfxInt16 value2)
	{
		elem[0] = value1;
		elem[1] = value2;
	}
};

struct PfxQuantize3
{
	PfxInt16 elem[3];

	PfxQuantize3(PfxInt16 value1,PfxInt16 value2,PfxInt16 value3)
	{
		elem[0] = value1;
		elem[1] = value2;
		elem[2] = value3;
	}
};

///////////////////////////////////////////////////////////////////////////////
// PfxBvhStack

template <class T, int maxData = 32>
struct PfxBvhStack {
	int cur;
	SCE_PFX_PADDING(1,12)
	T data[maxData];

	PfxBvhStack() : cur(0) {}

	void push(const T &d)
	{
		SCE_PFX_ASSERT(cur<maxData-1);
		data[cur++] = d;
	}
	
	T pop()
	{
		return data[--cur];
	}

	bool isEmpty()
	{
		return cur == 0;
	}
};

///////////////////////////////////////////////////////////////////////////////
// PfxFloat3

class PfxFloat3
{
private:
	PfxFloat m_v[3];

public:
	PfxFloat3() {m_v[0] = m_v[1] = m_v[2] = 0.0f;}
	PfxFloat3(const PfxVector3 SCE_VECTORMATH_AOS_VECTOR_ARG vec) {pfxStoreVector3(vec,(PfxFloat*)m_v);}
	PfxFloat3(PfxFloat i0,PfxFloat i1,PfxFloat i2) {m_v[0]=i0; m_v[1] = i1; m_v[2] = i2;}
	PfxFloat3(PfxFloat iv) {m_v[0] = m_v[1] = m_v[2] = iv;}

	inline PfxFloat3 &operator =(const PfxFloat3 &vec)
	{
		m_v[0] = vec.m_v[0];
		m_v[1] = vec.m_v[1];
		m_v[2] = vec.m_v[2];
		return *this;
	}

	inline PfxFloat &operator [](const PfxInt32 i)
	{
		return m_v[i];
	}

	inline PfxFloat operator [](const PfxInt32 i) const
	{
		return PfxFloat(m_v[i]);
	}

	inline operator PfxVector3() const
	{
		return pfxReadVector3(m_v);
	}
};

///////////////////////////////////////////////////////////////////////////////
// Convex Mesh

struct SCE_PFX_ALIGNED(16) PfxConvexMesh
{
	PfxUInt8 m_numVerts;
	PfxUInt8 m_numIndices;
	SCE_PFX_PADDING(1,2)
	PfxUInt16  *m_indices;
	PfxVector3 *m_verts;
	SCE_PFX_PADDING(2,4)
	PfxVector3 m_half;

	PfxConvexMesh()
	{
		m_numVerts = m_numIndices = 0;
	}
	
	inline void updateAABB();
};

inline
void PfxConvexMesh::updateAABB()
{
	PfxVector3 halfMax(0);
	for(int i=0;i<m_numVerts;i++) {
		halfMax = maxPerElem(absPerElem(m_verts[i]),halfMax);
	}
	m_half = halfMax;
}

///////////////////////////////////////////////////////////////////////////////
// Edge

struct PfxEdge
{
	PfxUInt8 m_vertId[2];
	PfxUInt8 m_angleType;
	PfxUInt8 m_tilt;
};

inline
bool operator ==(const PfxEdge &e1,const PfxEdge &e2)
{
	return  (e1.m_vertId[0] == e2.m_vertId[0] && e1.m_vertId[1] == e2.m_vertId[1]) || 
			(e1.m_vertId[1] == e2.m_vertId[0] && e1.m_vertId[0] == e2.m_vertId[1]);
}

///////////////////////////////////////////////////////////////////////////////
// Facet

struct PfxQuantizedFacet
{
	PfxQuantize2 m_normal;
	PfxQuantize3 m_half;
	PfxQuantize3 m_center;
	PfxQuantize m_thickness;
	PfxUInt8 m_vertIds[3];
	PfxUInt8 m_edgeIds[3];
	PfxUInt32 m_userData;
};

struct PfxExpandedFacet
{
	PfxFloat3 m_normal;
	PfxFloat3 m_half;
	PfxFloat3 m_center;
	PfxFloat m_thickness;
	PfxUInt8 m_vertIds[3];
	PfxUInt8 m_edgeIds[3];
	SCE_PFX_PADDING(1,2)
	PfxUInt32 m_userData;
};

struct PfxQuantizedFacetBvh
{
	PfxQuantize2 m_normal;
	PfxQuantize m_thickness;
	PfxUInt8 m_vertIds[3];
	PfxUInt8 m_edgeIds[3];
	PfxUInt32 m_userData;
};

struct PfxExpandedFacetBvh
{
	PfxFloat3 m_normal;
	PfxFloat m_thickness;
	PfxUInt8 m_vertIds[3];
	PfxUInt8 m_edgeIds[3];
	SCE_PFX_PADDING(1,2)
	PfxUInt32 m_userData;
};

///////////////////////////////////////////////////////////////////////////////
// TriMesh

struct SCE_PFX_ALIGNED(16) PfxQuantizedTriMesh
{
	PfxUInt8 m_numVerts;
	PfxUInt8 m_numEdges;
	PfxUInt8 m_numFacets;
	PfxUInt8 m_reserved;
	PfxEdge *m_edges;
	PfxQuantizedFacet *m_facets;
	PfxQuantize3 *m_verts;
};

struct SCE_PFX_ALIGNED(16) PfxExpandedTriMesh
{
	PfxUInt8 m_numVerts;
	PfxUInt8 m_numEdges;
	PfxUInt8 m_numFacets;
	PfxUInt8 m_reserved;
	PfxEdge *m_edges;
	PfxExpandedFacet *m_facets;
	PfxFloat3 *m_verts;
};

struct SCE_PFX_ALIGNED(16) PfxQuantizedTriMeshBvh
{
	PfxUInt8 m_numVerts;
	PfxUInt8 m_numEdges;
	PfxUInt8 m_numFacets;
	PfxUInt8 m_reserved;
	PfxEdge  *m_edges;
	PfxQuantizedFacetBvh *m_facets;
	PfxQuantize3 *m_verts;
	PfxAabb16 *m_bvhNodes;
	PfxUInt32 m_bvhRootId;
	SCE_PFX_PADDING(1,8)
};

struct SCE_PFX_ALIGNED(16) PfxExpandedTriMeshBvh
{
	PfxUInt8 m_numVerts;
	PfxUInt8 m_numEdges;
	PfxUInt8 m_numFacets;
	PfxUInt8 m_reserved;
	PfxEdge  *m_edges;
	PfxExpandedFacetBvh *m_facets;
	PfxFloat3 *m_verts;
	PfxAabb16 *m_bvhNodes;
	PfxUInt32 m_bvhRootId;
	SCE_PFX_PADDING(1,8)
};

} // namespace PhysicsEffects
} // namespace sce

