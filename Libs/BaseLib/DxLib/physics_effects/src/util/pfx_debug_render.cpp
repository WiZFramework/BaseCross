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
#include "stdafx.h"

#include "../../include/physics_effects/util/pfx_debug_render.h"


namespace sce {
namespace PhysicsEffects {

PfxDebugRender::PfxDebugRender() : m_scale(1.0f)
{
	m_normalLength    = 0.5f;

	m_renderPointFunc = NULL;
	m_renderLineFunc  = NULL;
	m_renderArcFunc   = NULL;
	m_renderAabbFunc  = NULL;
	m_renderBoxFunc   = NULL;

	m_colorWorld             = PfxVector3(0.0f,0.0f,0.6f);
	m_colorAabb              = PfxVector3(0.0f,0.0f,1.0f);
	m_colorIsland            = PfxVector3(0.0f,1.0f,0.0f);
	m_colorContactPointA     = PfxVector3(1.0f,0.0f,0.0f);
	m_colorContactPointB     = PfxVector3(1.0f,0.0f,0.5f);
	m_colorContactNormal     = PfxVector3(0.0f,0.0f,0.8f);
	m_colorMeshIsland        = PfxVector3(1.0f,0.0f,0.0f);
	m_colorMeshEdge          = PfxVector3(0.2f,0.2f,0.2f);
	m_colorMeshEdgeConvex    = PfxVector3(1.0f,1.0f,0.0f);
	m_colorFacetEdge         = PfxVector3(1.0f,0.0f,0.0f);
	m_colorFacetAabb         = PfxVector3(0.4f,0.4f,0.4f);
	m_colorFacetNormal       = PfxVector3(0.3f,0.9f,0.3f);
	m_colorFacetNormal2      = PfxVector3(0.0f,0.0f,0.8f);
	m_colorJointAxisX        = PfxVector3(1.0f,0.0f,0.0f);
	m_colorJointAxisY        = PfxVector3(0.0f,1.0f,0.0f);
	m_colorJointAxisZ        = PfxVector3(0.0f,0.0f,1.0f);
	m_colorJointAnchorA      = PfxVector3(1.0f,0.0f,0.0f);
	m_colorJointAnchorB      = PfxVector3(0.0f,1.0f,0.0f);
	m_colorJointArc1         = PfxVector3(1.0f,0.0f,0.0f);
	m_colorJointArc2         = PfxVector3(0.0f,1.0f,0.0f);

	m_colorBVH[0]            = PfxVector3(1.0f,0.1f,0.1f);
	m_colorBVH[1]            = PfxVector3(1.0f,0.3f,0.1f);
	m_colorBVH[2]            = PfxVector3(1.0f,0.6f,0.1f);
	m_colorBVH[3]            = PfxVector3(1.0f,0.8f,0.1f);
	m_colorBVH[4]            = PfxVector3(1.0f,0.1f,0.3f);
	m_colorBVH[5]            = PfxVector3(1.0f,0.1f,0.6f);
	m_colorBVH[6]            = PfxVector3(1.0f,0.1f,0.8f);
	m_colorBVH[7]            = PfxVector3(1.0f,0.8f,0.8f);
	
	m_colorFlatTri           = PfxVector3(0.1f,1.0f,0.1f);
	
	for(unsigned int i=0;i<SCE_PFX_DRENDER_RANDOM_COLOR_NUM;i++){
		m_colorRandom[i][0] = (rand()%100)*0.01f;
		m_colorRandom[i][1] = (rand()%100)*0.01f;
		m_colorRandom[i][2] = (rand()%100)*0.01f;
	}
}

PfxDebugRender::~PfxDebugRender()
{
	for(unsigned int k=0;k<m_largeMeshFlagTables.size();k++) {
		if (m_largeMeshFlagTables[k].largeMeshFlagTable) {
				SCE_PFX_UTIL_FREE(m_largeMeshFlagTables[k].largeMeshFlagTable);
		}
	}
	m_largeMeshFlagTables.clear();
	m_visibleFlags.clear();
}

void PfxDebugRender::setDebugRenderPointFunc(PfxDebugRenderPointFunc func)
{
	m_renderPointFunc = func;
}

void PfxDebugRender::setDebugRenderLineFunc(PfxDebugRenderLineFunc func)
{
	m_renderLineFunc = func;
}

void PfxDebugRender::setDebugRenderArcFunc(PfxDebugRenderArcFunc func)
{
	m_renderArcFunc = func;
}

void PfxDebugRender::setDebugRenderAabbFunc(PfxDebugRenderAabbFunc func)
{
	m_renderAabbFunc = func;
}

void PfxDebugRender::setDebugRenderBoxFunc(PfxDebugRenderBoxFunc func)
{
	m_renderBoxFunc = func;
}

void PfxDebugRender::renderWorld(const PfxVector3 &center,const PfxVector3 &halfExtent)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderAabbFunc,"PfxDebugRender: render Aabb function is not set.\n");
	m_renderAabbFunc(center,halfExtent,m_colorWorld);
}

void PfxDebugRender::renderLocalAxis(const PfxRigidState *states,const PfxUInt32 numRigidbodies)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");

	PfxUInt32 vsize = m_visibleFlags.size();
	for(PfxUInt32 i=0;i<numRigidbodies;i++) {
		if(i < vsize && m_visibleFlags[i] == SCE_PFX_DRENDER_VISIBLE){
			const PfxRigidState &state = states[i];

			PfxVector3 pos(state.getPosition());
			PfxMatrix3 ori(state.getOrientation());
			ori *= m_scale;

			m_renderLineFunc(pos,pos+ori.getCol0(),m_colorJointAxisX);
			m_renderLineFunc(pos,pos+ori.getCol1(),m_colorJointAxisY);
			m_renderLineFunc(pos,pos+ori.getCol2(),m_colorJointAxisZ);
		}
	}
}

void PfxDebugRender::renderAabb(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderAabbFunc,"PfxDebugRender: render Aabb function is not set.\n");

	PfxUInt32 vsize = m_visibleFlags.size();
	for(PfxUInt32 i=0;i<numRigidbodies;i++) {
		if(i < vsize && m_visibleFlags[i] == SCE_PFX_DRENDER_VISIBLE){
			const PfxRigidState &state = states[i];
			const PfxCollidable &coll = collidables[i];

			PfxVector3 pos(state.getPosition());
			PfxMatrix3 ori(state.getOrientation());
			PfxVector3 center = pos + ori * coll.getCenter();
			PfxVector3 half   = absPerElem(ori) * coll.getHalf();
			m_renderAabbFunc(center,half,m_colorAabb);
		}
	}
}

void PfxDebugRender::renderIsland(const PfxIsland *island,const PfxRigidState *states,const PfxCollidable *collidables)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderAabbFunc,"PfxDebugRender: render Aabb function is not set.\n");

	if(island) {
		for(PfxUInt32 i=0;i<pfxGetNumIslands(island);i++) {
			PfxIslandUnit *islandUnit = pfxGetFirstUnitInIsland(island,i);
			PfxVector3 aabbMin(SCE_PFX_FLT_MAX);
			PfxVector3 aabbMax(-SCE_PFX_FLT_MAX);
			for(;islandUnit!=NULL;islandUnit=pfxGetNextUnitInIsland(islandUnit)) {
				const PfxRigidState &state = states[pfxGetUnitId(islandUnit)];
				const PfxCollidable &coll = collidables[pfxGetUnitId(islandUnit)];
				PfxVector3 center = state.getPosition() + PfxMatrix3(state.getOrientation()) * coll.getCenter();
				PfxVector3 half = absPerElem(PfxMatrix3(state.getOrientation())) * coll.getHalf();
				aabbMin = minPerElem(aabbMin,center-half);
				aabbMax = maxPerElem(aabbMax,center+half);
			}
			PfxVector3 center = (aabbMax+aabbMin)*0.5f;
			PfxVector3 half =  (aabbMax-aabbMin)*0.5f;

			m_renderAabbFunc(center,half,m_colorIsland);
		}
	}
}

void PfxDebugRender::renderContact(const PfxContactManifold *contacts,const PfxBroadphasePair *pairsBuff,const PfxRigidState *states,const PfxUInt32 numContacts)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderPointFunc,"PfxDebugRender: render Point function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");

	PfxUInt32 vsize = m_visibleFlags.size();
	for(PfxUInt32 i=0;i<numContacts;i++) {
		const PfxContactManifold &contact = contacts[pfxGetConstraintId(pairsBuff[i])];
		const PfxUInt32 rbIdA = contact.getRigidBodyIdA();	
		const PfxUInt32 rbIdB = contact.getRigidBodyIdB();
		const PfxRigidState &stateA = states[rbIdA];	
		const PfxRigidState &stateB = states[rbIdB];	
		
		for(int j=0;j<contact.getNumContacts();j++) {
			const PfxContactPoint &cp = contact.getContactPoint(j);
			PfxVector3 pointNormal = pfxReadVector3(cp.m_constraintRow[0].m_normal);

			if(rbIdA < vsize && m_visibleFlags[rbIdA] == SCE_PFX_DRENDER_VISIBLE){
				PfxVector3 pointA = stateA.getPosition()+rotate(stateA.getOrientation(),pfxReadVector3(cp.m_localPointA));
				m_renderPointFunc(pointA,m_colorContactPointA);
				m_renderLineFunc(pointA,pointA+m_scale * pointNormal,m_colorContactNormal);
			}
			if(rbIdB < vsize && m_visibleFlags[rbIdB] == SCE_PFX_DRENDER_VISIBLE){
				PfxVector3 pointB = stateB.getPosition()+rotate(stateB.getOrientation(),pfxReadVector3(cp.m_localPointB));
				m_renderPointFunc(pointB,m_colorContactPointB);
				m_renderLineFunc(pointB,pointB-m_scale * pointNormal,m_colorContactNormal);
			}
		}
	}
}

static PfxAabb16 findIslandInBvh(const PfxLargeTriMesh *mesh,PfxUInt32 islandIdx)
{
	PfxBvhStack<PfxAabb16> bvhStack;
	
	bvhStack.push(mesh->m_bvhNodes[mesh->m_bvhRootId]);

	while(!bvhStack.isEmpty()) {
		PfxAabb16 aabb = bvhStack.pop();
		
		PfxUInt32 flag = aabb.get32(3) & 0xc0000000;
		PfxUInt32 packedId = aabb.get32(3) & 0x3fffffff;
		PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;
		PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
		PfxUInt32 rightId = packedId&0x000003ff;
		
		if(nodeId == islandIdx) return aabb;
		
		if(flag != 0) {
			bvhStack.push(mesh->m_bvhNodes[rightId]);
			bvhStack.push(mesh->m_bvhNodes[leftId]);
			continue;
		}
	}
	return mesh->m_bvhNodes[mesh->m_bvhRootId];
}

void PfxDebugRender::renderLargeMeshIsland(const PfxLargeTriMesh *mesh,PfxUInt32 islandIdx,const PfxTransform3 &shapeTransform,const PfxVector3 &color)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderBoxFunc,"PfxDebugRender: render Box function is not set.\n");

	PfxAabb16 aabb;
	aabb.set32(0,0);
	aabb.set32(1,0);
	aabb.set32(2,0);
	aabb.set32(3,0);

	if(mesh->m_type & 0x02) {
		aabb = findIslandInBvh(mesh,islandIdx);
	} else {
		aabb = mesh->m_aabbList[islandIdx];
	}

	PfxVector3 aabbMin = mesh->getWorldPosition(PfxVecInt3(pfxGetXMin(aabb),pfxGetYMin(aabb),pfxGetZMin(aabb)));
	PfxVector3 aabbMax = mesh->getWorldPosition(PfxVecInt3(pfxGetXMax(aabb),pfxGetYMax(aabb),pfxGetZMax(aabb)));
	PfxVector3 aabbCenter = (aabbMin + aabbMax) * 0.5f;
	PfxVector3 aabbHalf   = (aabbMax - aabbMin) * 0.5f;
	m_renderBoxFunc(shapeTransform * PfxTransform3::translation(aabbCenter),aabbHalf,color);
}

void PfxDebugRender::renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxExpandedTriMesh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color)
{
	(void) mesh;
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderBoxFunc,"PfxDebugRender: render Box function is not set.\n");

	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;

	const PfxExpandedFacet &facet = island->m_facets[facetId];
	const PfxEdge *edge[3] = {
		&island->m_edges[facet.m_edgeIds[0]],
		&island->m_edges[facet.m_edgeIds[1]],
		&island->m_edges[facet.m_edgeIds[2]],
	};

	if(SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
		PfxVector3 cnt = pfxReadVector3((PfxFloat*)&facet.m_center);
		PfxVector3 hlf = pfxReadVector3((PfxFloat*)&facet.m_half);
		m_renderBoxFunc(shapeTransform * PfxTransform3::translation(cnt),hlf,m_colorFacetAabb);
	}

	if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag) {
		PfxVector3 normal = shapeTransform.getUpper3x3() * pfxReadVector3((PfxFloat*)&facet.m_normal);
		PfxVector3 pnts[6];
		
		pnts[0] = island->m_verts[facet.m_vertIds[0]];
		pnts[1] = island->m_verts[facet.m_vertIds[1]];
		pnts[2] = island->m_verts[facet.m_vertIds[2]];
		
		for(int m=0;m<3;m++) {
			pnts[m] = PfxVector3(shapeTransform * PfxPoint3(pnts[m]));
		}
		m_renderLineFunc(pnts[0],pnts[1],color);
		m_renderLineFunc(pnts[1],pnts[2],color);
		m_renderLineFunc(pnts[2],pnts[0],color);
		
		if(SCE_PFX_DRENDER_MESH_FLG_THICKNESS & flag) {
			pnts[3] = pnts[0]-facet.m_thickness*normal;
			pnts[4] = pnts[1]-facet.m_thickness*normal;
			pnts[5] = pnts[2]-facet.m_thickness*normal;

			if(edge[0]->m_tilt > 0 || edge[1]->m_tilt > 0 || edge[2]->m_tilt > 0) {
				PfxVector3 sideNml[3];
				sideNml[0] = normalize(cross((pnts[1] - pnts[0]),normal));
				sideNml[1] = normalize(cross((pnts[2] - pnts[1]),normal));
				sideNml[2] = normalize(cross((pnts[0] - pnts[2]),normal));
				float angle0 = 0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f);
				float angle1 = 0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f);
				float angle2 = 0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f);
				sideNml[0] = cosf(angle0)*sideNml[0] - sinf(angle0)*normal;
				sideNml[1] = cosf(angle1)*sideNml[1] - sinf(angle1)*normal;
				sideNml[2] = cosf(angle2)*sideNml[2] - sinf(angle2)*normal;
				PfxMatrix3 mtx(0.0f);
				PfxVector3 vec(0.0f);
				mtx.setRow(0,sideNml[0]);
				mtx.setRow(1,sideNml[1]);
				mtx.setRow(2,sideNml[2]);
				vec[0] = dot(pnts[0],sideNml[0]);
				vec[1] = dot(pnts[1],sideNml[1]);
				vec[2] = dot(pnts[2],sideNml[2]);
				PfxVector3 intersection = inverse(mtx) * vec;
				float dist = -dot(intersection-pnts[0],normal);
				if(facet.m_thickness < dist) {
					float t = facet.m_thickness / dist;
					pnts[3] = pnts[0]+t*(intersection-pnts[0]);
					pnts[4] = pnts[1]+t*(intersection-pnts[1]);
					pnts[5] = pnts[2]+t*(intersection-pnts[2]);
				} else {
					pnts[3] = pnts[4] = pnts[5] = intersection;
				}
			}

			m_renderLineFunc(pnts[3],pnts[4],color);
			m_renderLineFunc(pnts[4],pnts[5],color);
			m_renderLineFunc(pnts[5],pnts[3],color);
			m_renderLineFunc(pnts[0],pnts[3],color);
			m_renderLineFunc(pnts[1],pnts[4],color);
			m_renderLineFunc(pnts[2],pnts[5],color);
		}
	}
	
	if(SCE_PFX_DRENDER_MESH_FLG_NORMAL & flag)
	{
		PfxVector3 normal = facet.m_normal;
		PfxVector3 pnts[3];
		
		pnts[0] = island->m_verts[facet.m_vertIds[0]];
		pnts[1] = island->m_verts[facet.m_vertIds[1]];
		pnts[2] = island->m_verts[facet.m_vertIds[2]];
		
		PfxVector3 p1((pnts[0] + pnts[1] + pnts[2])/3.0f);
		PfxVector3 p2(p1 + normal * m_normalLength);

		p1 = PfxVector3(shapeTransform * PfxPoint3(p1));
		p2 = PfxVector3(shapeTransform * PfxPoint3(p2));
		
		PfxVector3 white(0.0f, 0.0f, 1.0f);
		m_renderLineFunc(p1,p2,m_colorFacetNormal);
	}
}

void PfxDebugRender::renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxQuantizedTriMesh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color)

{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderBoxFunc,"PfxDebugRender: render Box function is not set.\n");

	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;

	const PfxQuantizedFacet &facet = island->m_facets[facetId];
	const PfxEdge *edge[3] = {
		&island->m_edges[facet.m_edgeIds[0]],
		&island->m_edges[facet.m_edgeIds[1]],
		&island->m_edges[facet.m_edgeIds[2]],
	};

	if(SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
		m_renderBoxFunc(shapeTransform * PfxTransform3::translation( mesh->decodePosition(facet.m_center)), mesh->decodePosition(facet.m_half),m_colorFacetAabb);
	}

	if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag) {
		PfxVector3 normal = shapeTransform.getUpper3x3() * mesh->decodeNormal(facet.m_normal);
		PfxVector3 pnts[6];
		
		pnts[0] = mesh->decodePosition(island->m_verts[facet.m_vertIds[0]]);
		pnts[1] = mesh->decodePosition(island->m_verts[facet.m_vertIds[1]]);
		pnts[2] = mesh->decodePosition(island->m_verts[facet.m_vertIds[2]]);

		for(int m=0;m<3;m++) {
			pnts[m] = PfxVector3(shapeTransform * PfxPoint3(pnts[m]));
		}
		m_renderLineFunc(pnts[0],pnts[1],color);
		m_renderLineFunc(pnts[1],pnts[2],color);
		m_renderLineFunc(pnts[2],pnts[0],color);

		if(SCE_PFX_DRENDER_MESH_FLG_THICKNESS & flag) {
			pnts[3] = pnts[0]-mesh->decodeFloat(facet.m_thickness)*normal;
			pnts[4] = pnts[1]-mesh->decodeFloat(facet.m_thickness)*normal;
			pnts[5] = pnts[2]-mesh->decodeFloat(facet.m_thickness)*normal;

			if(edge[0]->m_tilt > 0 || edge[1]->m_tilt > 0 || edge[2]->m_tilt > 0) {
				PfxVector3 sideNml[3];
				sideNml[0] = normalize(cross((pnts[1] - pnts[0]),normal));
				sideNml[1] = normalize(cross((pnts[2] - pnts[1]),normal));
				sideNml[2] = normalize(cross((pnts[0] - pnts[2]),normal));
				float angle0 = 0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f);
				float angle1 = 0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f);
				float angle2 = 0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f);
				sideNml[0] = cosf(angle0)*sideNml[0] - sinf(angle0)*normal;
				sideNml[1] = cosf(angle1)*sideNml[1] - sinf(angle1)*normal;
				sideNml[2] = cosf(angle2)*sideNml[2] - sinf(angle2)*normal;
				PfxMatrix3 mtx(0.0f);
				PfxVector3 vec(0.0f);
				mtx.setRow(0,sideNml[0]);
				mtx.setRow(1,sideNml[1]);
				mtx.setRow(2,sideNml[2]);
				vec[0] = dot(pnts[0],sideNml[0]);
				vec[1] = dot(pnts[1],sideNml[1]);
				vec[2] = dot(pnts[2],sideNml[2]);
				PfxVector3 intersection = inverse(mtx) * vec;
				float dist = -dot(intersection-pnts[0],normal);
				if(mesh->decodeFloat(facet.m_thickness) < dist) {
					float t = mesh->decodeFloat(facet.m_thickness) / dist;
					pnts[3] = pnts[0]+t*(intersection-pnts[0]);
					pnts[4] = pnts[1]+t*(intersection-pnts[1]);
					pnts[5] = pnts[2]+t*(intersection-pnts[2]);
				} else {
					pnts[3] = pnts[4] = pnts[5] = intersection;
				}
			}

			m_renderLineFunc(pnts[3],pnts[4],color);
			m_renderLineFunc(pnts[4],pnts[5],color);
			m_renderLineFunc(pnts[5],pnts[3],color);
			m_renderLineFunc(pnts[0],pnts[3],color);
			m_renderLineFunc(pnts[1],pnts[4],color);
			m_renderLineFunc(pnts[2],pnts[5],color);
		}
	}
	
	if(SCE_PFX_DRENDER_MESH_FLG_NORMAL & flag)
	{
		PfxVector3 normal = mesh->decodeNormal(facet.m_normal);
		PfxVector3 pnts[3];
		
		pnts[0] = mesh->decodePosition(island->m_verts[facet.m_vertIds[0]]);
		pnts[1] = mesh->decodePosition(island->m_verts[facet.m_vertIds[1]]);
		pnts[2] = mesh->decodePosition(island->m_verts[facet.m_vertIds[2]]);
		
		PfxVector3 p1((pnts[0] + pnts[1] + pnts[2])/3.0f);
		PfxVector3 p2(p1 + normal * m_normalLength);

		p1 = PfxVector3(shapeTransform * PfxPoint3(p1));
		p2 = PfxVector3(shapeTransform * PfxPoint3(p2));
		
		PfxVector3 white(1.0f, 1.0f, 1.0f);
		m_renderLineFunc(p1,p2,m_colorFacetNormal);
	}
}

void PfxDebugRender::renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxExpandedTriMeshBvh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color)
{
	(void) mesh;

	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderBoxFunc,"PfxDebugRender: render Box function is not set.\n");

	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;

	const PfxExpandedFacetBvh &facet = island->m_facets[facetId];
	const PfxEdge *edge[3] = {
		&island->m_edges[facet.m_edgeIds[0]],
		&island->m_edges[facet.m_edgeIds[1]],
		&island->m_edges[facet.m_edgeIds[2]],
	};

	if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag || SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
		PfxVector3 normal = shapeTransform.getUpper3x3() * pfxReadVector3((PfxFloat*)&facet.m_normal);

		PfxVector3 pnts[6];
		pnts[0] = PfxVector3(shapeTransform * PfxPoint3(island->m_verts[facet.m_vertIds[0]]));
		pnts[1] = PfxVector3(shapeTransform * PfxPoint3(island->m_verts[facet.m_vertIds[1]]));
		pnts[2] = PfxVector3(shapeTransform * PfxPoint3(island->m_verts[facet.m_vertIds[2]]));
		pnts[3] = pnts[0]-facet.m_thickness*normal;
		pnts[4] = pnts[1]-facet.m_thickness*normal;
		pnts[5] = pnts[2]-facet.m_thickness*normal;

		if(edge[0]->m_tilt > 0 || edge[1]->m_tilt > 0 || edge[2]->m_tilt > 0) {
			PfxVector3 sideNml[3];
			sideNml[0] = normalize(cross((pnts[1] - pnts[0]),normal));
			sideNml[1] = normalize(cross((pnts[2] - pnts[1]),normal));
			sideNml[2] = normalize(cross((pnts[0] - pnts[2]),normal));
			float angle0 = 0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f);
			float angle1 = 0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f);
			float angle2 = 0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f);
			sideNml[0] = cosf(angle0)*sideNml[0] - sinf(angle0)*normal;
			sideNml[1] = cosf(angle1)*sideNml[1] - sinf(angle1)*normal;
			sideNml[2] = cosf(angle2)*sideNml[2] - sinf(angle2)*normal;
			PfxMatrix3 mtx(0.0f);
			PfxVector3 vec(0.0f);
			mtx.setRow(0,sideNml[0]);
			mtx.setRow(1,sideNml[1]);
			mtx.setRow(2,sideNml[2]);
			vec[0] = dot(pnts[0],sideNml[0]);
			vec[1] = dot(pnts[1],sideNml[1]);
			vec[2] = dot(pnts[2],sideNml[2]);
			PfxVector3 intersection = inverse(mtx) * vec;
			float dist = -dot(intersection-pnts[0],normal);
			if(facet.m_thickness < dist) {
				float t = facet.m_thickness / dist;
				pnts[3] = pnts[0]+t*(intersection-pnts[0]);
				pnts[4] = pnts[1]+t*(intersection-pnts[1]);
				pnts[5] = pnts[2]+t*(intersection-pnts[2]);
			} else {
				pnts[3] = pnts[4] = pnts[5] = intersection;
			}
		}

		PfxVector3 aabbMin = minPerElem(pnts[5],minPerElem(pnts[4],minPerElem(pnts[3],minPerElem(pnts[2],minPerElem(pnts[1],pnts[0])))));
		PfxVector3 aabbMax = maxPerElem(pnts[5],maxPerElem(pnts[4],maxPerElem(pnts[3],maxPerElem(pnts[2],maxPerElem(pnts[1],pnts[0])))));

		if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag) {
			m_renderLineFunc(pnts[0],pnts[1],color);
			m_renderLineFunc(pnts[1],pnts[2],color);
			m_renderLineFunc(pnts[2],pnts[0],color);
			if(SCE_PFX_DRENDER_MESH_FLG_THICKNESS & flag) {
				m_renderLineFunc(pnts[3],pnts[4],color);
				m_renderLineFunc(pnts[4],pnts[5],color);
				m_renderLineFunc(pnts[5],pnts[3],color);
				m_renderLineFunc(pnts[0],pnts[3],color);
				m_renderLineFunc(pnts[1],pnts[4],color);
				m_renderLineFunc(pnts[2],pnts[5],color);
			}
		}
		if(SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
			PfxVector3 center = 0.5f * (aabbMax + aabbMin);
			PfxVector3 half = 0.5f * (aabbMax - aabbMin);
			m_renderBoxFunc(shapeTransform * PfxTransform3::translation(center),half,m_colorFacetAabb);
		}
	}
	
	if(SCE_PFX_DRENDER_MESH_FLG_NORMAL & flag)
	{
		PfxVector3 normal = pfxReadVector3((PfxFloat*)&facet.m_normal);

		PfxVector3 pnts[36];
		
		pnts[0] = island->m_verts[facet.m_vertIds[0]];
		pnts[1] = island->m_verts[facet.m_vertIds[1]];
		pnts[2] = island->m_verts[facet.m_vertIds[2]];
		
		PfxVector3 p1((pnts[0] + pnts[1] + pnts[2])/3.0f);
		PfxVector3 p2(p1 + normal * m_normalLength);
	
		p1 = PfxVector3(shapeTransform * PfxPoint3(p1));
		p2 = PfxVector3(shapeTransform * PfxPoint3(p2));
		
		PfxVector3 white(1.0f, 0.0f, 0.0f);
		m_renderLineFunc(p1,p2,m_colorFacetNormal);
	}
}

void PfxDebugRender::renderLargeMeshFacet(const PfxLargeTriMesh *mesh, const PfxQuantizedTriMeshBvh *island, PfxUInt32 facetId, const PfxTransform3 &shapeTransform, const PfxUInt32 flag, const PfxVector3 &color)

{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderBoxFunc,"PfxDebugRender: render Box function is not set.\n");

	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;

	const PfxQuantizedFacetBvh &facet = island->m_facets[facetId];
	const PfxEdge *edge[3] = {
		&island->m_edges[facet.m_edgeIds[0]],
		&island->m_edges[facet.m_edgeIds[1]],
		&island->m_edges[facet.m_edgeIds[2]],
	};

	if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag || SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
		PfxVector3 normal = shapeTransform.getUpper3x3() * mesh->decodeNormal(facet.m_normal);
		float fthick = mesh->decodeFloat(facet.m_thickness);

		PfxVector3 pnts[6];
		pnts[0] = PfxVector3(shapeTransform * PfxPoint3(mesh->decodePosition(island->m_verts[facet.m_vertIds[0]])));
		pnts[1] = PfxVector3(shapeTransform * PfxPoint3(mesh->decodePosition(island->m_verts[facet.m_vertIds[1]])));
		pnts[2] = PfxVector3(shapeTransform * PfxPoint3(mesh->decodePosition(island->m_verts[facet.m_vertIds[2]])));
		pnts[3] = pnts[0]-fthick*normal;
		pnts[4] = pnts[1]-fthick*normal;
		pnts[5] = pnts[2]-fthick*normal;

		if(edge[0]->m_tilt > 0 || edge[1]->m_tilt > 0 || edge[2]->m_tilt > 0) {
			PfxVector3 sideNml[3];
			sideNml[0] = normalize(cross((pnts[1] - pnts[0]),normal));
			sideNml[1] = normalize(cross((pnts[2] - pnts[1]),normal));
			sideNml[2] = normalize(cross((pnts[0] - pnts[2]),normal));
			float angle0 = 0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f);
			float angle1 = 0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f);
			float angle2 = 0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f);
			sideNml[0] = cosf(angle0)*sideNml[0] - sinf(angle0)*normal;
			sideNml[1] = cosf(angle1)*sideNml[1] - sinf(angle1)*normal;
			sideNml[2] = cosf(angle2)*sideNml[2] - sinf(angle2)*normal;
			PfxMatrix3 mtx(0.0f);
			PfxVector3 vec(0.0f);
			mtx.setRow(0,sideNml[0]);
			mtx.setRow(1,sideNml[1]);
			mtx.setRow(2,sideNml[2]);
			vec[0] = dot(pnts[0],sideNml[0]);
			vec[1] = dot(pnts[1],sideNml[1]);
			vec[2] = dot(pnts[2],sideNml[2]);
			PfxVector3 intersection = inverse(mtx) * vec;
			float dist = -dot(intersection-pnts[0],normal);
			if(mesh->decodeFloat(facet.m_thickness) < dist) {
				float t = mesh->decodeFloat(facet.m_thickness) / dist;
				pnts[3] = pnts[0]+t*(intersection-pnts[0]);
				pnts[4] = pnts[1]+t*(intersection-pnts[1]);
				pnts[5] = pnts[2]+t*(intersection-pnts[2]);
			} else {
				pnts[3] = pnts[4] = pnts[5] = intersection;
			}
		}
		PfxVector3 aabbMin = minPerElem(pnts[5],minPerElem(pnts[4],minPerElem(pnts[3],minPerElem(pnts[2],minPerElem(pnts[1],pnts[0])))));
		PfxVector3 aabbMax = maxPerElem(pnts[5],maxPerElem(pnts[4],maxPerElem(pnts[3],maxPerElem(pnts[2],maxPerElem(pnts[1],pnts[0])))));

		if(SCE_PFX_DRENDER_MESH_FLG_EDGE & flag) {
			m_renderLineFunc(pnts[0],pnts[1],color);
			m_renderLineFunc(pnts[1],pnts[2],color);
			m_renderLineFunc(pnts[2],pnts[0],color);
			if(SCE_PFX_DRENDER_MESH_FLG_THICKNESS & flag) {
				m_renderLineFunc(pnts[3],pnts[4],color);
				m_renderLineFunc(pnts[4],pnts[5],color);
				m_renderLineFunc(pnts[5],pnts[3],color);
				m_renderLineFunc(pnts[0],pnts[3],color);
				m_renderLineFunc(pnts[1],pnts[4],color);
				m_renderLineFunc(pnts[2],pnts[5],color);
			}
		}
		if(SCE_PFX_DRENDER_MESH_FLG_FACET_AABB & flag) {
			PfxVector3 center = 0.5f * (aabbMax + aabbMin);
			PfxVector3 half = 0.5f * (aabbMax - aabbMin);
			m_renderBoxFunc(shapeTransform * PfxTransform3::translation(center),half,m_colorFacetAabb);
		}
	}

	if(SCE_PFX_DRENDER_MESH_FLG_NORMAL & flag)
	{
		PfxVector3 normal = mesh->decodeNormal(facet.m_normal);
		PfxVector3 pnts[3];
		
		pnts[0] = mesh->decodePosition(island->m_verts[facet.m_vertIds[0]]);
		pnts[1] = mesh->decodePosition(island->m_verts[facet.m_vertIds[1]]);
		pnts[2] = mesh->decodePosition(island->m_verts[facet.m_vertIds[2]]);

		PfxVector3 p1((pnts[0] + pnts[1] + pnts[2])/3.0f);
		PfxVector3 p2(p1 + normal * m_normalLength);

		p1 = PfxVector3(shapeTransform * PfxPoint3(p1));
		p2 = PfxVector3(shapeTransform * PfxPoint3(p2));
		
		PfxVector3 white(0.0f, 0.0f, 1.0f);
		m_renderLineFunc(p1,p2,m_colorFacetNormal);
	}
}

bool PfxDebugRender::isOutsideFrustum(const PfxVector4* planes, int numPlanes, const PfxVector3 &spherePos, PfxFloat radius)
{
	for(int i=0; i<numPlanes; i++)
	{
		const PfxVector3 plane = pfxReadVector3((PfxFloat*)&planes[i]);		
		const PfxFloat distance = dot(plane, spherePos) + planes[i].getW();
	
		if(distance < -radius)
			return true;
	}
	return false;
}

void PfxDebugRender::renderLargeMesh(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies,const PfxUInt32 flag)
{
	renderLargeMeshInFrustum(states, collidables, numRigidbodies, flag, NULL, 0);
}

void PfxDebugRender::renderLargeMeshInFrustum(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 numRigidbodies,const PfxUInt32 flag, const PfxVector4* planes, int numPlanes)
{
	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;

	PfxUInt32 vsize = m_visibleFlags.size();
	for(PfxUInt32 i=0;i<numRigidbodies;i++) {

		if(i < vsize && m_visibleFlags[i] == SCE_PFX_DRENDER_VISIBLE){
			const PfxRigidState &state = states[i];
			const PfxCollidable &coll = collidables[i];
			PfxTransform3 bodyTransform(state.getOrientation(),state.getPosition());

			for(PfxUInt32 j=0;j<coll.getNumShapes();j++) {
				const PfxShape &shape = coll.getShape(j);
				if (shape.getType() == kPfxShapeLargeTriMesh) {
					const PfxLargeTriMesh *mesh = shape.getLargeTriMesh();
					PfxTransform3 shapeTransform = bodyTransform * shape.getOffsetTransform();
					for(int l=0;l<mesh->m_numIslands;l++) {
						//mesh 
						// If we've given a frustum and the AABB for this island is outside the frustum, then let's just skip rendering this thing.
						// Sloppy check here, do the sphere since it's invariant.
						switch(numPlanes) {
						case 0:	
							break;
						case 1:
							{
								PfxVector3 aabbCenter,aabbHalf;
								getLargeMeshAABB(aabbCenter,aabbHalf,mesh, l, shapeTransform);
								const PfxVector3 testCenter = pfxReadVector3((PfxFloat*)&planes[0]);	
								const PfxFloat testRadius = planes[0].getW();
								const PfxFloat distance = lengthSqr(testCenter - aabbCenter);
								const PfxFloat r2 = (testRadius + length(aabbHalf));
								if(distance > r2*r2) continue;
							}
							break;
						default:
							{
								PfxVector3 aabbCenter,aabbHalf;
								getLargeMeshAABB(aabbCenter,aabbHalf,mesh, l, shapeTransform);
								if(isOutsideFrustum(planes, numPlanes, aabbCenter, length(aabbHalf)))
									continue;
							}
							break;
						}
						
						if(flag  & SCE_PFX_DRENDER_MESH_FLG_ISLAND) {
							PfxVector3 color;
							if(mesh->m_type & 0x02) {
								color = m_colorBVH[l%(sizeof(m_colorBVH)/sizeof(m_colorBVH[0]))];
							} else {
								color = m_colorFlatTri;
							}
							renderLargeMeshIsland(mesh,l,shapeTransform,color);
						}

						switch(mesh->m_type) {
							case SCE_PFX_LARGE_MESH_TYPE_EXPANDED_ARRAY:
								for(int f=0;f<((PfxExpandedTriMesh *)mesh->m_islands + l)->m_numFacets;f++) {
									PfxVector3 col =  m_colorRandom[f%SCE_PFX_DRENDER_RANDOM_COLOR_NUM];
									renderLargeMeshFacet(mesh, (const PfxExpandedTriMesh *)mesh->m_islands + l, f, shapeTransform, flag, col);
								}
							break;
							case SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_ARRAY:
								for(int f=0;f<((PfxQuantizedTriMesh *)mesh->m_islands + l)->m_numFacets;f++) {
									PfxVector3 col =  m_colorRandom[f%SCE_PFX_DRENDER_RANDOM_COLOR_NUM];
									renderLargeMeshFacet(mesh, (const PfxQuantizedTriMesh *)mesh->m_islands + l, f, shapeTransform, flag, col);
								}
							break;
							case SCE_PFX_LARGE_MESH_TYPE_EXPANDED_BVH:
								for(int f=0;f<((PfxExpandedTriMeshBvh *)mesh->m_islands + l)->m_numFacets;f++) {
									PfxVector3 col =  m_colorRandom[f%SCE_PFX_DRENDER_RANDOM_COLOR_NUM];
									renderLargeMeshFacet(mesh, (const PfxExpandedTriMeshBvh *)mesh->m_islands + l, f, shapeTransform, flag, col);
								}
							break;
							case SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_BVH:
								for(int f=0;f<((PfxQuantizedTriMeshBvh *)mesh->m_islands + l)->m_numFacets;f++) {
									PfxVector3 col =  m_colorRandom[f%SCE_PFX_DRENDER_RANDOM_COLOR_NUM];
									renderLargeMeshFacet(mesh, (const PfxQuantizedTriMeshBvh *)mesh->m_islands + l, f, shapeTransform, flag, col);
								}
							break;
						}
					}
				}
			}
		}
	}
}

void PfxDebugRender::renderLargeMeshByFlag(const PfxRigidState *states,const PfxCollidable *collidables,const PfxUInt32 flag)
{
	if(flag==SCE_PFX_DRENDER_MESH_FLG_NONE) return;
	for(unsigned int k=0;k<m_largeMeshFlagTables.size();k++) {
		int i = m_largeMeshFlagTables[k].rigidbodyId;

		const PfxRigidState &state = states[i];
		const PfxCollidable &coll = collidables[i];
		PfxTransform3 bodyTransform(state.getOrientation(),state.getPosition());
		for(PfxUInt32 j=0;j<coll.getNumShapes();j++) {
			const PfxShape &shape = coll.getShape(j);
			if (shape.getType() == kPfxShapeLargeTriMesh) {
				const PfxLargeTriMesh *mesh = shape.getLargeTriMesh();
				PfxTransform3 shapeTransform = bodyTransform * shape.getOffsetTransform();
				for(int l=0;l<mesh->m_numIslands;l++) {
					PfxVector3 col = m_colorRandom[l%SCE_PFX_DRENDER_RANDOM_COLOR_NUM];
					int cnt = 0;
					switch(mesh->m_type) {
						case SCE_PFX_LARGE_MESH_TYPE_EXPANDED_ARRAY:
							for(int f=0;f<((PfxExpandedTriMesh *)mesh->m_islands + l)->m_numFacets;f++) {
								if(m_largeMeshFlagTables[k].largeMeshFlagTable[SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*l+f] & flag) {
									renderLargeMeshFacet(mesh, (const PfxExpandedTriMesh *)mesh->m_islands + l, f, shapeTransform, flag, col);
									cnt++;
								}
							}
						break;
						case SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_ARRAY:
							for(int f=0;f<((PfxQuantizedTriMesh *)mesh->m_islands + l)->m_numFacets;f++) {
								if(m_largeMeshFlagTables[k].largeMeshFlagTable[SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*l+f] & flag) {
									renderLargeMeshFacet(mesh, (const PfxQuantizedTriMesh *)mesh->m_islands + l, f, shapeTransform, flag, col);
									cnt++;
								}
							}
						break;
						case SCE_PFX_LARGE_MESH_TYPE_EXPANDED_BVH:
							for(int f=0;f<((PfxExpandedTriMeshBvh *)mesh->m_islands + l)->m_numFacets;f++) {
								if(m_largeMeshFlagTables[k].largeMeshFlagTable[SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*l+f] & flag) {
									renderLargeMeshFacet(mesh, (const PfxExpandedTriMeshBvh *)mesh->m_islands + l, f, shapeTransform, flag, col);
									cnt++;
								}
							}
						break;
						case SCE_PFX_LARGE_MESH_TYPE_QUANTIZED_BVH:
							for(int f=0;f<((PfxQuantizedTriMeshBvh *)mesh->m_islands + l)->m_numFacets;f++) {
								if(m_largeMeshFlagTables[k].largeMeshFlagTable[SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*l+f] & flag) {
									renderLargeMeshFacet(mesh, (const PfxQuantizedTriMeshBvh *)mesh->m_islands + l, f, shapeTransform, flag, col);
									cnt++;
								}
							}
						break;
					}
					if(cnt > 0 && (flag & SCE_PFX_DRENDER_MESH_FLG_ISLAND)) {
						renderLargeMeshIsland(mesh,l,shapeTransform,col);
					}
				}
			}
		}
	}
}

void PfxDebugRender::resetVisible(const PfxUInt32 numRigidbodies)
{
	PfxUInt8 v = SCE_PFX_DRENDER_VISIBLE;
	m_visibleFlags.assign(numRigidbodies,v);
}

void PfxDebugRender::resetLargeMeshFlagTables(const PfxCollidable *collidables,const PfxUInt32 numRigidbodies)
{
	for(unsigned int k=0;k<m_largeMeshFlagTables.size();k++) {
		if (m_largeMeshFlagTables[k].largeMeshFlagTable) {
			SCE_PFX_UTIL_FREE(m_largeMeshFlagTables[k].largeMeshFlagTable);
		}
	}
	m_largeMeshFlagTables.clear();

	for(PfxUInt16 i=0;i<numRigidbodies;i++) {
		const PfxCollidable &coll = collidables[i];
		for(PfxUInt32 j=0;j<coll.getNumShapes();j++) {
			const PfxShape &shape = coll.getShape(j);
			if (shape.getType() == kPfxShapeLargeTriMesh) {
				unsigned int mem_size = sizeof(PfxUInt8)*SCE_PFX_DRENDER_ISLANDS_MAX_NUM*SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM;
				PfxUInt8 *largeMeshFlagTable = (PfxUInt8*)SCE_PFX_UTIL_ALLOC(16,mem_size);
				SCE_PFX_ALWAYS_ASSERT_MSG(largeMeshFlagTable,"PfxDebugRender: can not allocate largeMeshFlagTable.\n");
				memset(largeMeshFlagTable, 0, mem_size);
				PfxLargeMeshFlagTable ft;
				ft.rigidbodyId = i;
				ft.largeMeshFlagTable = largeMeshFlagTable;
				m_largeMeshFlagTables.push(ft);
			}
		}
	}
}

void PfxDebugRender::renderJoint(const PfxJoint *joints,const PfxRigidState *states,const PfxUInt32 numJoints)
{
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderPointFunc,"PfxDebugRender: render Point function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderLineFunc,"PfxDebugRender: render Line function is not set.\n");
	SCE_PFX_ALWAYS_ASSERT_MSG(m_renderArcFunc,"PfxDebugRender: render Arc function is not set.\n");

	float lower1, upper1, lower2, upper2;
	PfxVector3 anchorA, anchorB;
	PfxMatrix3 frameA, frameB;
	PfxVector3 vlow, vup, endpt;
	//joint
	PfxUInt32 vsize = m_visibleFlags.size();
	for(PfxUInt32 i=0;i<numJoints;i++) {
		PfxJoint joint = joints[i];
		if(!joint.m_active) continue;

		const PfxUInt32 idA = joint.m_rigidBodyIdA;
		const PfxUInt32 idB = joint.m_rigidBodyIdB;

		if((idA >= vsize || m_visibleFlags[idA] != SCE_PFX_DRENDER_VISIBLE)
		    && (idB >= vsize || m_visibleFlags[idB] != SCE_PFX_DRENDER_VISIBLE)){
			continue;
		}

		const PfxRigidState &stateA = states[idA];
		const PfxRigidState &stateB = states[idB];

		PfxMatrix3 wRotA(stateA.getOrientation());
		PfxMatrix3 wRotB(stateB.getOrientation());
		PfxVector3 wAnchorA = stateA.getPosition() + wRotA * joint.m_anchorA;
		PfxVector3 wAnchorB = stateB.getPosition() + wRotB * joint.m_anchorB;
		PfxMatrix3 wFrameA = wRotA * joint.m_frameA;
		PfxMatrix3 wFrameB = wRotB * joint.m_frameB;
		PfxMatrix4 wTransA(wFrameA,wAnchorA);
		PfxMatrix4 wTransB(wFrameB,wAnchorB);

		wFrameA *= m_scale;
		wFrameB *= m_scale;
		//point
		m_renderPointFunc(wAnchorA,m_colorJointAnchorA);
		m_renderPointFunc(wAnchorB,m_colorJointAnchorB);
		//axis
		m_renderLineFunc(wAnchorA,wAnchorA+wFrameA.getCol0(),m_colorJointAxisX);
		m_renderLineFunc(wAnchorA,wAnchorA+wFrameA.getCol1(),m_colorJointAxisY);
		m_renderLineFunc(wAnchorA,wAnchorA+wFrameA.getCol2(),m_colorJointAxisZ);
		m_renderLineFunc(wAnchorB,wAnchorB+wFrameB.getCol0(),m_colorJointAxisX);
		m_renderLineFunc(wAnchorB,wAnchorB+wFrameB.getCol1(),m_colorJointAxisY);
		m_renderLineFunc(wAnchorB,wAnchorB+wFrameB.getCol2(),m_colorJointAxisZ);

		switch(joint.m_type){
			case kPfxJointBall:
			case kPfxJointFix:
			case kPfxJointAnimation:
			break;
			case kPfxJointHinge:
				lower1 = joint.m_constraints[3].m_movableLowerLimit;
				upper1 = joint.m_constraints[3].m_movableUpperLimit;
				m_renderArcFunc(wAnchorA,wFrameA.getCol0(),wFrameA.getCol2(),1.0,lower1,upper1,m_colorJointArc1);
				break;
			case kPfxJointSlider:
				lower1 = joint.m_constraints[0].m_movableLowerLimit;
				upper1 = joint.m_constraints[0].m_movableUpperLimit;
				vlow = wAnchorA + wRotA * joint.m_frameA * PfxVector3(lower1, 0.0, 0.0);
				vup  = wAnchorA + wRotA * joint.m_frameA * PfxVector3(upper1, 0.0, 0.0);
				m_renderLineFunc(vlow, vup, PfxVector3(1.0,0.0,0.0));
				endpt = wFrameA.getCol2()*0.2*m_scale;
				m_renderLineFunc(vlow+endpt,vlow-endpt, PfxVector3(0.5,0.0,0.8));
				m_renderLineFunc(vup+endpt, vup-endpt,  PfxVector3(0.5,0.0,0.8));			
				break;
			case kPfxJointSwingtwist:	
				lower1 = joint.m_constraints[3].m_movableLowerLimit;
				upper1 = joint.m_constraints[3].m_movableUpperLimit;
				lower2 = joint.m_constraints[4].m_movableLowerLimit;
				upper2 = joint.m_constraints[4].m_movableUpperLimit;
				m_renderArcFunc(wAnchorA,wFrameA.getCol0(),wFrameA.getCol2(),0.5,  lower1, upper1,m_colorJointArc1); //twist
				m_renderArcFunc(wAnchorA,wFrameA.getCol2(),wFrameA.getCol0(),1.0,  lower2, upper2,m_colorJointArc2); //swing(+)
				m_renderArcFunc(wAnchorA,wFrameA.getCol2(),wFrameA.getCol0(),1.0, -lower2,-upper2,m_colorJointArc2); //swing(-)
				break;
			case kPfxJointUniversal:
				lower1 = joint.m_constraints[4].m_movableLowerLimit;
				upper1 = joint.m_constraints[4].m_movableUpperLimit;
				lower2 = joint.m_constraints[5].m_movableLowerLimit;
				upper2 = joint.m_constraints[5].m_movableUpperLimit;
				m_renderArcFunc(wAnchorA,wFrameA.getCol2(),wFrameA.getCol0(),1.0,lower1,upper1,m_colorJointArc1);
				m_renderArcFunc(wAnchorA,wFrameA.getCol1(),wFrameA.getCol0(),1.0,lower2,upper2,m_colorJointArc2);
			break;
			default:
				SCE_PFX_PRINTF("PfxDebugRender: joint type error\n");
			break;
		}
	}
}

PfxInt32 PfxDebugRender::getLargeMeshFlagTableId(const PfxUInt32 rigidbodyId)
{
	for(PfxUInt32 i=0;i<m_largeMeshFlagTables.size();i++) {
		if(m_largeMeshFlagTables[i].rigidbodyId == rigidbodyId) {
			return i;
		}
	}
	return SCE_PFX_ERR_OUT_OF_RANGE;
}

PfxInt32 PfxDebugRender::setLargeMeshFlag(const PfxUInt32 rigidbodyId,const PfxUInt32 islandId,const PfxUInt32 facetId,const PfxUInt8 flag)
{
	if(islandId>=SCE_PFX_DRENDER_ISLANDS_MAX_NUM) return SCE_PFX_ERR_OUT_OF_RANGE;
	if(facetId>=SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM) return SCE_PFX_ERR_OUT_OF_RANGE;
	if(flag>SCE_PFX_DRENDER_MESH_FLG_ALL) return SCE_PFX_ERR_OUT_OF_RANGE;

	for(PfxUInt32 k=0;k<m_largeMeshFlagTables.size();k++) {
		int t_idx = getLargeMeshFlagTableId(rigidbodyId);
		if(m_largeMeshFlagTables[t_idx].largeMeshFlagTable){
			unsigned int fct_idx = SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*islandId+facetId;
			m_largeMeshFlagTables[t_idx].largeMeshFlagTable[fct_idx] = flag;
		}
	}
	return SCE_PFX_OK;
}

PfxInt32 PfxDebugRender::getLargeMeshFlag(const PfxUInt32 rigidbodyId,const PfxUInt32 islandId,const PfxUInt32 facetId,PfxUInt8 &flag)
{
	if(islandId>=SCE_PFX_DRENDER_ISLANDS_MAX_NUM) return SCE_PFX_ERR_OUT_OF_RANGE;
	if(facetId>=SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM) return SCE_PFX_ERR_OUT_OF_RANGE;

	for(PfxUInt32 k=0;k<m_largeMeshFlagTables.size();k++) {
		int t_idx = getLargeMeshFlagTableId(rigidbodyId);
		if(m_largeMeshFlagTables[t_idx].largeMeshFlagTable){
			unsigned int fct_idx = SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM*islandId+facetId;
			flag = m_largeMeshFlagTables[t_idx].largeMeshFlagTable[fct_idx];
		}
	}
	return SCE_PFX_OK;
}

PfxInt32 PfxDebugRender::setVisible(const PfxUInt32 rigidbodyId,const PfxUInt8 flag)
{
	if(flag != SCE_PFX_DRENDER_INVISIBLE && flag != SCE_PFX_DRENDER_VISIBLE) return SCE_PFX_ERR_INVALID_FLAG;
	if(rigidbodyId>=m_visibleFlags.size()) return SCE_PFX_ERR_OUT_OF_RANGE;

	m_visibleFlags[rigidbodyId] = flag;
	return SCE_PFX_OK;
}

PfxInt32 PfxDebugRender::getVisible(const PfxUInt32 rigidbodyId,PfxUInt8 &flag)
{
	if(flag != SCE_PFX_DRENDER_INVISIBLE && flag != SCE_PFX_DRENDER_VISIBLE) return SCE_PFX_ERR_INVALID_FLAG;
	if(rigidbodyId>=m_visibleFlags.size()) return SCE_PFX_ERR_OUT_OF_RANGE;

	flag = m_visibleFlags[rigidbodyId];
	return SCE_PFX_OK;
}

PfxFloat PfxDebugRender::getScale()
{
	return m_scale;
}

void PfxDebugRender::setScale(PfxFloat scale)
{
	m_scale = scale;
}

void PfxDebugRender::getLargeMeshAABB(PfxVector3& aabbCenter, PfxVector3& aabbHalf, const PfxLargeTriMesh *mesh,PfxUInt32 islandIdx,const PfxTransform3 &shapeTransform)
{
	PfxAabb16 aabb;
	aabb.set32(0,0);
	aabb.set32(1,0);
	aabb.set32(2,0);
	aabb.set32(3,0);

	if(mesh->m_type & 0x02) {
		aabb = findIslandInBvh(mesh,islandIdx);
	} else {
		aabb = mesh->m_aabbList[islandIdx];
	}

	PfxVector3 aabbMin = mesh->getWorldPosition(PfxVecInt3(pfxGetXMin(aabb),pfxGetYMin(aabb),pfxGetZMin(aabb)));
	PfxVector3 aabbMax = mesh->getWorldPosition(PfxVecInt3(pfxGetXMax(aabb),pfxGetYMax(aabb),pfxGetZMax(aabb)));
	aabbCenter = PfxVector3(shapeTransform * PfxPoint3((aabbMin + aabbMax) * 0.5f));
	aabbHalf   = (aabbMax - aabbMin) * 0.5f;
}

#if 0
//for debug
void PfxDebugRender::debugPrintlargeMeshFlagTables()
{
	unsigned int table_len = SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM * SCE_PFX_DRENDER_ISLANDS_MAX_NUM;
	for(PfxUInt32 k=0;k<m_largeMeshFlagTables.size();k++) {
		if(m_largeMeshFlagTables[k].largeMeshFlagTable){
			SCE_PFX_PRINTF("\nPfxDebugRender::debugPrintlargeMeshFlagTables --------");
			for(unsigned int i=0;i<table_len;i++){
				if(i%SCE_PFX_DRENDER_FACETS_PER_ISLAND_NUM==0) SCE_PFX_PRINTF("\n");
				else if(i%8==0) SCE_PFX_PRINTF(" ");
				SCE_PFX_PRINTF("%d",m_largeMeshFlagTables[k].largeMeshFlagTable[i]);
			}
			SCE_PFX_PRINTF("\n\n");
		}
	}
}

void PfxDebugRender::debugPrintVisibleFlags()
{
	SCE_PFX_PRINTF("\nPfxDebugRender::debugPrintVisibleFlags --------\n");
	PfxUInt32 i = 0;
	for(i=0;i<m_visibleFlags.size();i++) {
		if(i%10==0) SCE_PFX_PRINTF("\n");
		SCE_PFX_PRINTF("%d",m_visibleFlags[i]);
	}
	SCE_PFX_PRINTF("\n\n");
}
#endif

} //namespace PhysicsEffects
} //namespace sce
