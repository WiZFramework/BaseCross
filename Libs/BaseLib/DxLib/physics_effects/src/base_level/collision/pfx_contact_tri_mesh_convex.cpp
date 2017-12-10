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

#include "../../../include/physics_effects/base_level/base/pfx_vec_utils.h"
#include "pfx_contact_tri_mesh_convex.h"
#include "pfx_intersect_common.h"
#include "pfx_mesh_common.h"
#include "pfx_decoded_tri_mesh.h"
#include "pfx_gjk_solver.h"
#include "pfx_gjk_support_func.h"

namespace sce {
namespace PhysicsEffects {

static
bool pfxContactTriangleConvex(PfxContactCache &contacts,PfxUInt32 facetId,
							const PfxVector3 &normal,const PfxVector3 &p0,const PfxVector3 &p1,const PfxVector3 &p2,
							const PfxFloat thickness,const PfxFloat angle0,const PfxFloat angle1,const PfxFloat angle2,
							PfxUInt32 edgeChk,
							const PfxConvexMesh &convex,const PfxTransform3 &transformB)
{
	// GJKの精度を確保するため、トライアングルの中心を基準にとる
	PfxVector3 centerA = (p0+p1+p2)/3.0f;
	PfxTransform3 transformA = PfxTransform3::translation(centerA);
	
	PfxVector3 facetPnts[6] = {
		p0-centerA,p1-centerA,p2-centerA,
		p0-centerA-thickness*normal,
		p1-centerA-thickness*normal,
		p2-centerA-thickness*normal
	};
	

	PfxPoint3 pA(0.0f),pB(0.0f);
	PfxVector3 nml(0.0f);
	PfxGjkSolver gjk;

	gjk.setup((void*)facetPnts,(void*)&convex,pfxGetSupportVertexTriangleWithThickness,pfxGetSupportVertexConvex);
	PfxFloat d;
	PfxInt32 ret = gjk.collide(d,nml,pA,pB,transformA,transformB,SCE_PFX_FLT_MAX);
	if(ret != kPfxGjkResultOk || d >= 0.0f) return false;

	pA += centerA;
	pB += centerA;
	
	PfxVector3 pointsOnTriangle = PfxVector3(pA);
	PfxVector3 pointsOnConvex = PfxVector3(pB);
	PfxVector3 axis = nml;
	
	// 面上の最近接点が凸エッジ上でない場合は法線を変える
	if( (((edgeChk&0x03)==0)&&pfxPointOnLine(pointsOnTriangle,p0,p1)) ||
		(((edgeChk&0x0c)==0)&&pfxPointOnLine(pointsOnTriangle,p1,p2)) ||
		(((edgeChk&0x30)==0)&&pfxPointOnLine(pointsOnTriangle,p2,p0)) ) {
		axis=-normal;
	}
	
	PfxSubData subData;
	subData.setFacetId(facetId);
	contacts.addContactPoint(-length(pointsOnTriangle-pointsOnConvex),axis,pA,pB,subData);
	
	return true;
}

PfxInt32 pfxContactTriMeshConvex(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMesh *meshA,
	const PfxConvexMesh &convexB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold)
{
	(void) distanceThreshold;

	//-------------------------------------------
	// 判定する面を絞り込む.

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = { 0 };

	PfxVector3 center = transformB.getTranslation();
	PfxVector3 half = absPerElem(transformB.getUpper3x3()) * convexB.m_half;
	PfxQuantize3 quantizedMinHalf = pfxHalve(largeMeshA->quantizePosition(center - half));
	PfxQuantize3 quantizedMaxHalf = pfxHalve(largeMeshA->quantizePosition(center + half));
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,quantizedMinHalf+quantizedMaxHalf,quantizedMaxHalf-quantizedMinHalf,selFacets);

	if(numSelFacets == 0) {
		return 0;
	}
	
	//-------------------------------------------
	// 面ごとに衝突を検出
	
	PfxDecodedTriMesh decodedMesh;
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxQuantizedFacet &facet = meshA->m_facets[selFacets[f]];
		
		// デコード
		PfxDecodedFacet decodedFacet;
		const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

		decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
		decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

	decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
	decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

	for(int v=0;v<3;v++) {
		if(!decodedMesh.isDecodedVertex(vId[v])) {
			decodedMesh.m_verts[vId[v]] = largeMeshA->decodePosition(meshA->m_verts[vId[v]]);
			decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
		}
	}

		PfxVector3 facetNormal = decodedFacet.m_normal;

		PfxVector3 facetPntsA[3] = {
			decodedMesh.m_verts[vId[0]],
			decodedMesh.m_verts[vId[1]],
			decodedMesh.m_verts[vId[2]],
		};

		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);

		pfxContactTriangleConvex(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							decodedFacet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,convexB,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxQuantizedFacet &facet = meshA->m_facets[subData.getFacetId()];
		
		PfxTriangle triangleA(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);

		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}
	
	return contacts.getNumContacts();
}

PfxInt32 pfxContactTriMeshConvex(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMesh *meshA,
	const PfxConvexMesh &convexB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold)
{
	(void) largeMeshA;
	(void) distanceThreshold;

	//-------------------------------------------
	// 判定する面を絞り込む.

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = { 0 };
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,
		transformB.getTranslation(),
		absPerElem(transformB.getUpper3x3())*convexB.m_half,selFacets);

	if(numSelFacets == 0) {
		return 0;
	}
	
	//-------------------------------------------
	// 面ごとに衝突を検出
	
	PfxContactCache localContacts;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxExpandedFacet &facet = meshA->m_facets[selFacets[f]];

		PfxVector3 facetNormal = pfxReadVector3((PfxFloat*)&facet.m_normal);

		PfxVector3 facetPntsA[3] = {
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]],
		};

		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);
		
		pfxContactTriangleConvex(localContacts,selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							facet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,convexB,transformB);
	}
	
	for(int i=0;i<localContacts.getNumContacts();i++) {
		PfxSubData subData = localContacts.getSubData(i);
		
		const PfxExpandedFacet &facet = meshA->m_facets[subData.getFacetId()];
		
		PfxTriangle triangleA(
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}
	
	return contacts.getNumContacts();
}


PfxInt32 pfxContactTriMeshConvex(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMeshBvh *meshA,
	const PfxConvexMesh &convexB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold )
{
	(void) distanceThreshold;

	//	building aabb of the capsule within A local
	PfxVector3 _aabbB(convexB.m_half);

	PfxVecInt3 aabbMinL,aabbMaxL;

	largeMeshA->getLocalPosition(
		(transformB.getTranslation() - absPerElem(transformB.getUpper3x3()) * _aabbB),
		(transformB.getTranslation() + absPerElem(transformB.getUpper3x3()) * _aabbB),
		aabbMinL,aabbMaxL);

	PfxAabb16 aabbB;

	pfxSetXMin(aabbB,aabbMinL.getX());
	pfxSetXMax(aabbB,aabbMaxL.getX());
	pfxSetYMin(aabbB,aabbMinL.getY());
	pfxSetYMax(aabbB,aabbMaxL.getY());
	pfxSetZMin(aabbB,aabbMinL.getZ());
	pfxSetZMax(aabbB,aabbMaxL.getZ());

	//	operate back-tracking through Largemsh's BV-tree
	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,aabbB,selFacets);

	//	operating "separating hyperplane theorem"
	PfxDecodedTriMesh decodedMesh;
	PfxContactCache localContacts;

	for(PfxUInt32 f = 0; f < numSelFacets; f++ ) {
		const PfxQuantizedFacetBvh &facet = meshA->m_facets[selFacets[f]];
		
		// デコード
		PfxDecodedFacet decodedFacet;
		const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

		decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
		decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

		decodedFacet.m_normal = largeMeshA->decodeNormal(facet.m_normal);
		decodedFacet.m_thickness = largeMeshA->decodeFloat(facet.m_thickness);

		for(int v=0;v<3;v++) {
			if(!decodedMesh.isDecodedVertex(vId[v])) {
				decodedMesh.m_verts[vId[v]] = largeMeshA->decodePosition(meshA->m_verts[vId[v]]);
				decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
			}
		}
		
		PfxVector3 facetNormal = decodedFacet.m_normal;
		
		PfxVector3 facetPntsA[3] = {
			decodedMesh.m_verts[vId[0]],
			decodedMesh.m_verts[vId[1]],
			decodedMesh.m_verts[vId[2]],
		};
		
		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);

		pfxContactTriangleConvex(localContacts, selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							decodedFacet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,convexB,transformB);
	}

	for(int i=0;i<localContacts.getNumContacts();i++)
	{
		PfxSubData subData = localContacts.getSubData(i);

		const PfxQuantizedFacetBvh &facet = meshA->m_facets[subData.getFacetId()];

		PfxTriangle triangleA(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);

		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}

	return contacts.getNumContacts();
}

PfxInt32 pfxContactTriMeshConvex(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMeshBvh *meshA,
	const PfxConvexMesh &convexB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold )
{
	(void) distanceThreshold;

	//	building aabb of the capsule within A local
	PfxVector3 _aabbB(convexB.m_half);

	PfxVecInt3 aabbMinL,aabbMaxL;

	largeMeshA->getLocalPosition(
		(transformB.getTranslation() - absPerElem(transformB.getUpper3x3()) * _aabbB),
		(transformB.getTranslation() + absPerElem(transformB.getUpper3x3()) * _aabbB),
		aabbMinL,aabbMaxL);

	PfxAabb16 aabbB;

	pfxSetXMin(aabbB,aabbMinL.getX());
	pfxSetXMax(aabbB,aabbMaxL.getX());
	pfxSetYMin(aabbB,aabbMinL.getY());
	pfxSetYMax(aabbB,aabbMaxL.getY());
	pfxSetZMin(aabbB,aabbMinL.getZ());
	pfxSetZMax(aabbB,aabbMaxL.getZ());

	//	operate back-tracking through Largemsh's BV-tree
	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};
	PfxUInt32 numSelFacets = pfxGatherFacets(meshA,aabbB,selFacets);

	//	operating "separating hyperplane theorem"
	PfxContactCache localContacts;

	for(PfxUInt32 f = 0; f < numSelFacets; f++ ) {
		const PfxExpandedFacetBvh &facet = meshA->m_facets[selFacets[f]];

		PfxVector3 facetNormal = pfxReadVector3((PfxFloat*)&facet.m_normal);
		
		PfxVector3 facetPntsA[3] = {
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]],
		};
		
		const PfxEdge *edge[3] = {
			&meshA->m_edges[facet.m_edgeIds[0]],
			&meshA->m_edges[facet.m_edgeIds[1]],
			&meshA->m_edges[facet.m_edgeIds[2]],
		};
		
		PfxUInt32 edgeChk = edge[0]->m_angleType | (edge[1]->m_angleType << 2) | (edge[2]->m_angleType << 4);

		pfxContactTriangleConvex(localContacts, selFacets[f],
							facetNormal,facetPntsA[0],facetPntsA[1],facetPntsA[2],
							facet.m_thickness,
							0.5f*SCE_PFX_PI*(edge[0]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[1]->m_tilt/255.0f),
							0.5f*SCE_PFX_PI*(edge[2]->m_tilt/255.0f),
							edgeChk,convexB,transformB);
	}

	for(int i=0;i<localContacts.getNumContacts();i++)
	{
		PfxSubData subData = localContacts.getSubData(i);

		const PfxExpandedFacetBvh &facet = meshA->m_facets[subData.getFacetId()];

		PfxTriangle triangleA(
			meshA->m_verts[facet.m_vertIds[0]],
			meshA->m_verts[facet.m_vertIds[1]],
			meshA->m_verts[facet.m_vertIds[2]]);

		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(PfxVector3(localContacts.getLocalPointA(i)),triangleA,s,t);
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setUserData(facet.m_userData);
		
		contacts.addContactPoint(
			localContacts.getDistance(i),
			localContacts.getNormal(i),
			localContacts.getLocalPointA(i),
			localContacts.getLocalPointB(i),
			subData);
	}

	return contacts.getNumContacts();
}


} //namespace PhysicsEffects
} //namespace sce

