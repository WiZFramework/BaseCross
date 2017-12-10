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

#include "../../physics_effects.h"

namespace sce {
namespace PhysicsEffects {

#define SCE_PFX_DRENDER_ISLANDS_MAX_NUM			SCE_PFX_MAX_LARGETRIMESH_ISLANDS
#define SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM	SCE_PFX_NUMMESHFACETS

#define SCE_PFX_DRENDER_MESH_FLG_NONE           0x00
#define SCE_PFX_DRENDER_MESH_FLG_ISLAND         0x01
#define SCE_PFX_DRENDER_MESH_FLG_EDGE           0x02
#define SCE_PFX_DRENDER_MESH_FLG_FACET_AABB     0x04
#define SCE_PFX_DRENDER_MESH_FLG_NORMAL         0x08
#define SCE_PFX_DRENDER_MESH_FLG_THICKNESS      0x10
#define SCE_PFX_DRENDER_MESH_FLG_ALL            0x1f

#define SCE_PFX_DRENDER_INVISIBLE               0x00
#define SCE_PFX_DRENDER_VISIBLE                 0x01

#define SCE_PFX_DRENDER_RANDOM_COLOR_NUM         128

typedef void (*PfxDebugRenderPointFunc)(const PfxVector3 &position,const PfxVector3 &color);
typedef void (*PfxDebugRenderLineFunc)(const PfxVector3 &position1,const PfxVector3 &position2,const PfxVector3 &color);
typedef void (*PfxDebugRenderArcFunc)(const PfxVector3 &pos,const PfxVector3 &axis,const PfxVector3 &dir,const float radius,const float startRad,const float endRad,const PfxVector3 &color);
typedef void (*PfxDebugRenderAabbFunc)(const PfxVector3 &center,const PfxVector3 &halfExtent,const PfxVector3 &color);
typedef void (*PfxDebugRenderBoxFunc)(const PfxTransform3 &transform,const PfxVector3 &halfExtent,const PfxVector3 &color);

struct PfxLargeMeshFlagTable{
	PfxUInt32 rigidbodyId;
	PfxUInt8  *largeMeshFlagTable;
};

class PfxDebugRender
{
	PfxArray<PfxUInt8>              m_visibleFlags;
	PfxArray<PfxLargeMeshFlagTable> m_largeMeshFlagTables;

	PfxFloat m_scale;
	PfxFloat m_normalLength;

	PfxDebugRenderPointFunc m_renderPointFunc;
	PfxDebugRenderLineFunc  m_renderLineFunc;
	PfxDebugRenderArcFunc   m_renderArcFunc;
	PfxDebugRenderAabbFunc  m_renderAabbFunc;
	PfxDebugRenderBoxFunc   m_renderBoxFunc;

	SCE_PFX_PADDING(0,4)
	PfxVector3 m_colorWorld;
	PfxVector3 m_colorAabb;
	PfxVector3 m_colorIsland;
	PfxVector3 m_colorContactPointA;
	PfxVector3 m_colorContactPointB;
	PfxVector3 m_colorContactNormal;
	PfxVector3 m_colorMeshIsland;
	PfxVector3 m_colorMeshEdge;
	PfxVector3 m_colorMeshEdgeConvex;
	PfxVector3 m_colorFacetEdge;
	PfxVector3 m_colorFacetAabb;
	PfxVector3 m_colorFacetNormal;
	PfxVector3 m_colorFacetNormal2;
	PfxVector3 m_colorJointAxisX;
	PfxVector3 m_colorJointAxisY;
	PfxVector3 m_colorJointAxisZ;
	PfxVector3 m_colorJointAnchorA;
	PfxVector3 m_colorJointAnchorB;
	PfxVector3 m_colorJointArc1;
	PfxVector3 m_colorJointArc2;
	PfxVector3 m_colorBVH[8];
	PfxVector3 m_colorFlatTri;
	PfxVector3 m_colorRandom[SCE_PFX_DRENDER_RANDOM_COLOR_NUM];

public:
	PfxDebugRender();
	~PfxDebugRender();

	//function setting
	void setDebugRenderPointFunc(PfxDebugRenderPointFunc func);
	void setDebugRenderLineFunc(PfxDebugRenderLineFunc func);
	void setDebugRenderArcFunc(PfxDebugRenderArcFunc func);
	void setDebugRenderAabbFunc(PfxDebugRenderAabbFunc func);
	void setDebugRenderBoxFunc(PfxDebugRenderBoxFunc func);

	//render
	void renderWorld(const PfxVector3 &center,const PfxVector3 &halfExtent);
	void renderAabb(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies);
	void renderLocalAxis(const PfxRigidState *states,const PfxUInt32 numRigidbodies);
	void renderIsland(const PfxIsland *island,const PfxRigidState *states,const PfxCollidable *collidables);
	void renderContact(const PfxContactManifold *contacts,const PfxBroadphasePair *pairsBuff,const PfxRigidState *states,const PfxUInt32 numContacts);
	void renderLargeMesh(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies,const PfxUInt32 flag);
	void renderLargeMeshInFrustum(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies,const PfxUInt32 flag, const PfxVector4* planes, int numPlanes);
	void renderLargeMeshByFlag(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 flag);
	void renderJoint(const PfxJoint *joints,const PfxRigidState *states,const PfxUInt32 numJoints);

	void resetLargeMeshFlagTables(const PfxCollidable *collidables,const PfxUInt32 numRigidbodies);
	PfxInt32 getLargeMeshFlag(const PfxUInt32 rigidbodyId,const PfxUInt32 islandId,const PfxUInt32 facetId,PfxUInt8 &flag);
	PfxInt32 setLargeMeshFlag(const PfxUInt32 rigidbodyId,const PfxUInt32 islandId,const PfxUInt32 facetId,const PfxUInt8 flag);

	void resetVisible(const PfxUInt32 numRigidbodies);
	PfxInt32 getVisible(const PfxUInt32 rigidbodyId,PfxUInt8 &flag);
	PfxInt32 setVisible(const PfxUInt32 rigidbodyId,const PfxUInt8 flag);

	PfxFloat getScale();
	void setScale(PfxFloat scale);

	static void* operator new(size_t size) {
		return SCE_PFX_UTIL_ALLOC(16,size);
	}
	static void operator delete(void* pv) {
		SCE_PFX_UTIL_FREE(pv);
	}

protected:
	PfxInt32 getLargeMeshFlagTableId(const PfxUInt32 rigidbodyId);
	void renderLargeMeshIsland(const PfxLargeTriMesh *mesh,PfxUInt32 islandIdx,const PfxTransform3 &shapeTransform,const PfxVector3 &color);
	void renderLargeMeshEdge(const PfxQuantizedTriMesh &island,const PfxLargeTriMesh &mesh, const PfxUInt32 facetIdx,const PfxTransform3 &shapeTransform,const PfxUInt32 flag,const PfxVector3 &color);
    void renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxExpandedTriMesh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color);
	void renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxQuantizedTriMesh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color);
	void renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxExpandedTriMeshBvh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color);
	void renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxQuantizedTriMeshBvh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color);
	
	void getLargeMeshAABB(PfxVector3& aabbCenter, PfxVector3& aabbHalf, const PfxLargeTriMesh *mesh,PfxUInt32 islandIdx,const PfxTransform3 &shapeTransform);

private:
	bool isOutsideFrustum(const PfxVector4* planes, int numPlanes, const PfxVector3 &spherePos, PfxFloat radius);
};


} //namespace PhysicsEffects
} //namespace sce

