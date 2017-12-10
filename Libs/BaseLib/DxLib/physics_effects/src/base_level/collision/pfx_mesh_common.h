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

#include "../../../include/physics_effects/base_level/base/pfx_common.h"
#include "../../../include/physics_effects/base_level/collision/pfx_tri_mesh.h"
#include "pfx_intersect_common.h"

#define SCE_PFX_THICKNESS_THRESHOLD 0.05f

namespace sce {
namespace PhysicsEffects {


static inline
PfxQuantize3 operator +( const PfxQuantize3 &q1, const PfxQuantize3 &q2 )
{
    return PfxQuantize3(q1.elem[0] + q2.elem[0],q1.elem[1] + q2.elem[1],q1.elem[2] + q2.elem[2]);
}

static inline
PfxQuantize3 operator -( const PfxQuantize3 &q1, const PfxQuantize3 &q2 )
{
    return PfxQuantize3(q1.elem[0] - q2.elem[0],q1.elem[1] - q2.elem[1],q1.elem[2] - q2.elem[2]);
}

static inline
PfxQuantize3 pfxHalve( const PfxQuantize3 &q )
{
	return PfxQuantize3(q.elem[0] >> 1,q.elem[1] >> 1,q.elem[2] >> 1);
}

struct PfxClosestPoints {
	PfxPoint3 pA[4],pB[4];
	PfxFloat distSqr[4];
	PfxFloat closestDistSqr;
	int numPoints;
	SCE_PFX_PADDING(1,8)
	
	PfxClosestPoints()
	{
		numPoints = 0;
		closestDistSqr = SCE_PFX_FLT_MAX;
	}
	
	void set(int i,const PfxPoint3 &pointA,const PfxPoint3 &pointB,PfxFloat dSqr)
	{
		pA[i] = pointA;
		pB[i] = pointB;
		distSqr[i] = dSqr;
	}
	
	void add(const PfxPoint3 &pointA,const PfxPoint3 &pointB,PfxFloat dSqr)
	{
		const PfxFloat epsilon = 0.00001f;
		if(closestDistSqr < dSqr) return;
		
		int replaceId = -1;
		PfxFloat distMax = -SCE_PFX_FLT_MAX;
		for(int i=0;i<numPoints;i++) {
			if(lengthSqr(pA[i]-pointA) < epsilon && lengthSqr(pB[i]-pointB) < epsilon && distSqr[i] < dSqr) {
				return;
			}
			if(distMax < distSqr[i]) {
				distMax = distSqr[i];
				replaceId = i;
			}
		}
		
		replaceId = (numPoints<4)?(numPoints++):replaceId;

		closestDistSqr = dSqr + epsilon;

		set(replaceId,pointA,pointB,dSqr);
	}
};

static SCE_PFX_FORCE_INLINE
PfxUInt32 pfxGatherFacets(
	const PfxQuantizedTriMesh *mesh,
	const PfxQuantize3 &aabbCenter,
	const PfxQuantize3 &aabbHalf,
	PfxUInt8 *selFacets)
{
	PfxUInt32 numSelFacets = 0;
	for(int f=0;f<mesh->m_numFacets;f++) {
		const PfxQuantizedFacet &facet = mesh->m_facets[f];
		if(abs((PfxInt32)facet.m_center.elem[0] - (PfxInt32)aabbCenter.elem[0]) > (PfxInt32)facet.m_half.elem[0] + (PfxInt32)aabbHalf.elem[0]) continue;
		if(abs((PfxInt32)facet.m_center.elem[1] - (PfxInt32)aabbCenter.elem[1]) > (PfxInt32)facet.m_half.elem[1] + (PfxInt32)aabbHalf.elem[1]) continue;
		if(abs((PfxInt32)facet.m_center.elem[2] - (PfxInt32)aabbCenter.elem[2]) > (PfxInt32)facet.m_half.elem[2] + (PfxInt32)aabbHalf.elem[2]) continue;
		selFacets[numSelFacets++] = f;
	}
	return numSelFacets;
}

static SCE_PFX_FORCE_INLINE
PfxUInt32 pfxGatherFacets(
	const PfxExpandedTriMesh *mesh,
	const PfxVector3 &aabbCenter,
	const PfxVector3 &aabbHalf,
	PfxUInt8 *selFacets)
{
	PfxUInt32 numSelFacets = 0;
	PfxVecInt3 vi3;
	for(int f=0;f<mesh->m_numFacets;f++) {
		const PfxExpandedFacet &facet = mesh->m_facets[f];
		PfxVector3 cnt = pfxReadVector3((PfxFloat*)&facet.m_center);
		PfxVector3 hlf = pfxReadVector3((PfxFloat*)&facet.m_half);
		PfxVector3 results = absPerElem(cnt - aabbCenter) - (hlf + aabbHalf);
		if(results[0] > 0.0f || results[1] > 0.0f || results[2] > 0.0f) continue;
		selFacets[numSelFacets++] = f;
	}
	return numSelFacets;
}

static SCE_PFX_FORCE_INLINE
PfxUInt32 pfxGatherFacets(
	const PfxQuantizedTriMeshBvh *mesh,
	const PfxAabb16 &aabb,
	PfxUInt8 *selFacets)
{
	PfxUInt32 numSelFacets = 0;
	PfxBvhStack<PfxAabb16> bvhStack;

	bvhStack.push(mesh->m_bvhNodes[mesh->m_bvhRootId]);

	while(!bvhStack.isEmpty()) {
		PfxAabb16 aabbA = bvhStack.pop();
		
		if(pfxGetXMax(aabbA) < pfxGetXMin(aabb) || pfxGetXMin(aabbA) > pfxGetXMax(aabb)) continue;
		if(pfxGetYMax(aabbA) < pfxGetYMin(aabb) || pfxGetYMin(aabbA) > pfxGetYMax(aabb)) continue;
		if(pfxGetZMax(aabbA) < pfxGetZMin(aabb) || pfxGetZMin(aabbA) > pfxGetZMax(aabb)) continue;
		
		PfxUInt32 flag = aabbA.get32(3) & 0xc0000000;
		PfxUInt32 packedId = aabbA.get32(3) & 0x3fffffff;
		PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;
		PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
		PfxUInt32 rightId = packedId&0x000003ff;

		if(flag != 0) {
			bvhStack.push(mesh->m_bvhNodes[rightId]);
			bvhStack.push(mesh->m_bvhNodes[leftId]);
			continue;
		}
		
		selFacets[numSelFacets++] = nodeId;
	}
	return numSelFacets;
}

static SCE_PFX_FORCE_INLINE
PfxUInt32 pfxGatherFacets(
	const PfxExpandedTriMeshBvh *mesh,
	const PfxAabb16 &aabb,
	PfxUInt8 *selFacets)
{
	PfxUInt32 numSelFacets = 0;
	PfxBvhStack<PfxAabb16> bvhStack;

	bvhStack.push(mesh->m_bvhNodes[mesh->m_bvhRootId]);

	while(!bvhStack.isEmpty()) {
		PfxAabb16 aabbA = bvhStack.pop();
		
		if(pfxGetXMax(aabbA) < pfxGetXMin(aabb) || pfxGetXMin(aabbA) > pfxGetXMax(aabb)) continue;
		if(pfxGetYMax(aabbA) < pfxGetYMin(aabb) || pfxGetYMin(aabbA) > pfxGetYMax(aabb)) continue;
		if(pfxGetZMax(aabbA) < pfxGetZMin(aabb) || pfxGetZMin(aabbA) > pfxGetZMax(aabb)) continue;
		
		PfxUInt32 flag = aabbA.get32(3) & 0xc0000000;
		PfxUInt32 packedId = aabbA.get32(3) & 0x3fffffff;
		PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;
		PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
		PfxUInt32 rightId = packedId&0x000003ff;

		if(flag != 0) {
			bvhStack.push(mesh->m_bvhNodes[rightId]);
			bvhStack.push(mesh->m_bvhNodes[leftId]);
			continue;
		}
		
		selFacets[numSelFacets++] = nodeId;
	}
	return numSelFacets;
}

static SCE_PFX_FORCE_INLINE
void pfxGetProjAxisPnts6(
	const PfxVector3 *verts,const PfxVector3 &axis,
	PfxFloat &distMin,PfxFloat &distMax)
{
PfxFloat p0 = dot(axis, verts[0]);
PfxFloat p1 = dot(axis, verts[1]);
PfxFloat p2 = dot(axis, verts[2]);
PfxFloat p3 = dot(axis, verts[3]);
PfxFloat p4 = dot(axis, verts[4]);
PfxFloat p5 = dot(axis, verts[5]);
distMin = SCE_PFX_MIN(p5,SCE_PFX_MIN(p4,SCE_PFX_MIN(p3,SCE_PFX_MIN(p2,SCE_PFX_MIN(p0,p1)))));
distMax = SCE_PFX_MAX(p5,SCE_PFX_MAX(p4,SCE_PFX_MAX(p3,SCE_PFX_MAX(p2,SCE_PFX_MAX(p0,p1)))));
}

static SCE_PFX_FORCE_INLINE
void pfxGetProjAxisPnts3(
	const PfxVector3 *verts,const PfxVector3 &axis,
	PfxFloat &distMin,PfxFloat &distMax)
{
PfxFloat p0 = dot(axis, verts[0]);
PfxFloat p1 = dot(axis, verts[1]);
PfxFloat p2 = dot(axis, verts[2]);
distMin = SCE_PFX_MIN(p2,SCE_PFX_MIN(p0,p1));
distMax = SCE_PFX_MAX(p2,SCE_PFX_MAX(p0,p1));
}

static SCE_PFX_FORCE_INLINE
void pfxGetProjAxisPnts2(
	const PfxVector3 *verts,const PfxVector3 &axis,
	PfxFloat &distMin,PfxFloat &distMax)
{
PfxFloat p0 = dot(axis, verts[0]);
PfxFloat p1 = dot(axis, verts[1]);
distMin = SCE_PFX_MIN(p0,p1);
distMax = SCE_PFX_MAX(p0,p1);
}

///////////////////////////////////////////////////////////////////////////////
// ２つのベクトルの向きをチェック

static SCE_PFX_FORCE_INLINE
bool pfxIsSameDirection(const PfxVector3 &vecA,const PfxVector3 &vecB)
{
return pfxAbsf(dot(vecA,vecB)) > 0.9999f;
}

///////////////////////////////////////////////////////////////////////////////
// 面ローカルの座標を算出

static SCE_PFX_FORCE_INLINE
void pfxGetLocalCoords(
	const PfxVector3 &pointOnTriangle,
	const PfxTriangle &triangle,
	PfxFloat &s,PfxFloat &t)
{
	PfxVector3 p0 = triangle.points[0];
	PfxVector3 p1 = triangle.points[1];
	PfxVector3 p2 = triangle.points[2];

	#if 0
	PfxVector3 v0 = p1 - p0;
	PfxVector3 v1 = p2 - p0;
	PfxVector3 dir = pointOnTriangle - p0;
	PfxVector3 v = cross( v0, v1 );
	PfxVector3 crS = cross( v, v0 );
	PfxVector3 crT = cross( v, v1 );
	s = dot( crT, dir ) / dot( crT, v0 );
	t = dot( crS, dir ) / dot( crS, v1 );
	#else
	PfxVector3 v = cross((p1 - p0),(p2 - p0));
	PfxFloat tmp1 = dot(v,v);
	PfxVector3 tmp2 = cross(p0-pointOnTriangle,v);
	s = dot(tmp2,(p2-pointOnTriangle)) / tmp1;
	t = dot(tmp2,(pointOnTriangle-p1)) / tmp1;
	#endif
}

// a,bからなる直線上に点pがあるかどうかを判定
static SCE_PFX_FORCE_INLINE
bool pfxPointOnLine(const PfxVector3 &p,const PfxVector3 &a,const PfxVector3 &b)
{
	PfxVector3 ab = normalize(b-a);
	PfxVector3 q = a + ab * dot(p-a,ab);
	return lengthSqr(p-q) < 0.00001f;
}

// 線分a,b上に点pがあるかどうかを判定
static SCE_PFX_FORCE_INLINE
bool pfxPointOnSegment(const PfxVector3 &p,const PfxVector3 &a,const PfxVector3 &b)
{
	PfxVector3 ab = b-a;
	PfxVector3 ap = p-a;
	PfxFloat denom = dot(ab,ab);
	PfxFloat num = dot(ap,ab);
	PfxFloat t = num/denom;
	if(t < 0.0f || t > 1.0f) return false;
	return (dot(ap,ap)-num*t) < 0.00001f;
}


} //namespace PhysicsEffects
} //namespace sce

