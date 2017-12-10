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

#include "../../../include/physics_effects/base_level/collision/pfx_large_tri_mesh.h"
#include "pfx_intersect_common.h"
#include "pfx_mesh_common.h"
#include "pfx_decoded_tri_mesh.h"
#include "pfx_intersect_ray_large_tri_mesh.h"


namespace sce {
namespace PhysicsEffects {


static SCE_PFX_FORCE_INLINE
PfxBool pfxIntersectRayTriMesh(
	const PfxLargeTriMesh *largeMesh,const PfxQuantizedTriMesh *mesh,
	const PfxVector3 &rayStart,const PfxVector3 &rayDir,
	PfxUInt32 facetMode,PfxFloat &variable,PfxVector3 &normal,PfxSubData &subData)
{
	bool ret = false;
	PfxFloat nearest_t = variable;
	PfxVector3 nearest_nml(0.0f);
	PfxUInt32 nearest_f = 0;

	//-------------------------------------------
	// 判定する面を絞り込む

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};

	PfxVector3 aabbMin = minPerElem(rayStart,rayStart + rayDir);
	PfxVector3 aabbMax = maxPerElem(rayStart,rayStart + rayDir);
	PfxQuantize3 quantizedMinHalf = pfxHalve(largeMesh->quantizePosition(aabbMin));
	PfxQuantize3 quantizedMaxHalf = pfxHalve(largeMesh->quantizePosition(aabbMax));
	PfxUInt32 numSelFacets = pfxGatherFacets(mesh,quantizedMinHalf+quantizedMaxHalf,quantizedMaxHalf-quantizedMinHalf,selFacets);

	if(numSelFacets == 0) {
		return 0;
	}
	
	//-------------------------------------------
	// レイ交差判定
	
	PfxDecodedTriMesh decodedMesh;
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxQuantizedFacet &facet = mesh->m_facets[selFacets[f]];

		PfxVector3 facetCenter =  largeMesh->decodePosition(facet.m_center);
		PfxVector3 facetHalf =  largeMesh->decodeVector(facet.m_half);

		PfxFloat cur_t = 1.0f;

		if( !pfxIntersectRayAABBFast(rayStart,rayDir,facetCenter,facetHalf,cur_t) )
			continue;
		
		if( nearest_t <= cur_t ) continue;

		// デコード
		const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

	for(int v=0;v<3;v++) {
		if(!decodedMesh.isDecodedVertex(vId[v])) {
			decodedMesh.m_verts[vId[v]] = largeMesh->decodePosition(mesh->m_verts[vId[v]]);
			decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
		}
	}

		PfxTriangle triangle(
			decodedMesh.m_verts[vId[0]],
			decodedMesh.m_verts[vId[1]],
			decodedMesh.m_verts[vId[2]]);
		
		if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_ONLY && 
			pfxIntersectRayTriangleWithoutBackFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = largeMesh->decodeNormal(facet.m_normal);
			ret = true;
		}
		else if(facetMode == SCE_PFX_RAY_FACET_MODE_BACK_ONLY && 
			pfxIntersectRayTriangleWithoutFrontFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = largeMesh->decodeNormal(facet.m_normal);
			ret = true;
		}
		else if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_AND_BACK && 
			pfxIntersectRayTriangle(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = largeMesh->decodeNormal(facet.m_normal);
			ret = true;
		}
	}
	
	if(ret) {
		// 面のローカル座標を算出
		const PfxQuantizedFacet &facet = mesh->m_facets[nearest_f];

		PfxTriangle triangle(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(rayStart+t*rayDir,triangle,s,t);
		
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setFacetId(nearest_f);
		subData.setUserData(facet.m_userData);
		
		variable = nearest_t;
		normal = nearest_nml;
	}

	return ret;
}

static SCE_PFX_FORCE_INLINE
PfxBool pfxIntersectRayTriMesh(
	const PfxLargeTriMesh *largeMesh,const PfxExpandedTriMesh *mesh,
	const PfxVector3 &rayStart,const PfxVector3 &rayDir,
	PfxUInt32 facetMode,PfxFloat &variable,PfxVector3 &normal,PfxSubData &subData)
{
	(void) largeMesh;
	bool ret = false;
	PfxFloat nearest_t = variable;
	PfxFloat3 nearest_nml(0.0f);
	PfxUInt32 nearest_f = 0;

	//-------------------------------------------
	// 判定する面を絞り込む

	PfxUInt8 SCE_PFX_ALIGNED(16) selFacets[SCE_PFX_NUMMESHFACETS] = {0};

	PfxVector3 aabbMin = minPerElem(rayStart,rayStart + rayDir);
	PfxVector3 aabbMax = maxPerElem(rayStart,rayStart + rayDir);
	PfxUInt32 numSelFacets = pfxGatherFacets(mesh,0.5f*(aabbMin+aabbMax),0.5f*(aabbMax-aabbMin),selFacets);

	if(numSelFacets == 0) {
		return 0;
	}

	//-------------------------------------------
	// レイ交差判定
	
	for(PfxUInt32 f = 0; f < numSelFacets; f++) {
		const PfxExpandedFacet &facet = mesh->m_facets[selFacets[f]];
		
		PfxFloat cur_t = 1.0f;

		PfxVector3 cnt = pfxReadVector3((PfxFloat*)&facet.m_center);
		PfxVector3 hlf = pfxReadVector3((PfxFloat*)&facet.m_half);
		if( !pfxIntersectRayAABBFast(rayStart,rayDir,cnt,hlf,cur_t) )
			continue;
		
		if( nearest_t <= cur_t ) continue;

		PfxTriangle triangle(
			mesh->m_verts[facet.m_vertIds[0]],
			mesh->m_verts[facet.m_vertIds[1]],
			mesh->m_verts[facet.m_vertIds[2]]);
		
		if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_ONLY && 
			pfxIntersectRayTriangleWithoutBackFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = facet.m_normal;
			ret = true;
		}
		else if(facetMode == SCE_PFX_RAY_FACET_MODE_BACK_ONLY && 
			pfxIntersectRayTriangleWithoutFrontFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = facet.m_normal;
			ret = true;
		}
		else if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_AND_BACK && 
			pfxIntersectRayTriangle(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
			nearest_f = selFacets[f];
			nearest_t = cur_t;
			nearest_nml = facet.m_normal;
			ret = true;
		}
	}
	
	if(ret) {
		// 面のローカル座標を算出
		const PfxExpandedFacet &facet = mesh->m_facets[nearest_f];

		PfxTriangle triangle(
			mesh->m_verts[facet.m_vertIds[0]],
			mesh->m_verts[facet.m_vertIds[1]],
			mesh->m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(rayStart+t*rayDir,triangle,s,t);
		
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setFacetId(nearest_f);
		subData.setUserData(facet.m_userData);
		
		variable = nearest_t;
		normal = nearest_nml;
	}

	return ret;
}

static SCE_PFX_FORCE_INLINE
PfxBool pfxIntersectRayTriMesh(
	const PfxLargeTriMesh *largeMesh,const PfxQuantizedTriMeshBvh *mesh,
	const PfxVector3 &rayStart,const PfxVector3 &rayDir,
	PfxUInt32 facetMode,PfxFloat &variable,PfxVector3 &normal,PfxSubData &subData)
{
	bool ret = false;
	PfxFloat nearest_t = variable;
	PfxVector3 nearest_nml(0.0f);
	PfxUInt32 nearest_f = 0;

	PfxBvhStack< PfxAabb16 > bvhStack;
	bvhStack.push( mesh->m_bvhNodes[mesh->m_bvhRootId] );

	PfxDecodedTriMesh decodedMesh;

	while(!bvhStack.isEmpty()) {
		PfxAabb16 aabbA = bvhStack.pop();
		
		PfxVecInt3 minA(pfxGetXMin(aabbA),pfxGetYMin(aabbA),pfxGetZMin(aabbA));
		PfxVecInt3 maxA(pfxGetXMax(aabbA),pfxGetYMax(aabbA),pfxGetZMax(aabbA));

		PfxVector3 aabbMin,aabbMax;

		aabbMin = largeMesh->getWorldPosition(minA);
		aabbMax = largeMesh->getWorldPosition(maxA);

		PfxFloat cur_t = 1.0f;

		if(!pfxIntersectRayAABBFast(rayStart,rayDir,(aabbMax+aabbMin)*0.5f,(aabbMax-aabbMin)*0.5f,cur_t)) {
			continue;
		}

		if( nearest_t <= cur_t ) continue;

		PfxUInt32 flag = aabbA.get32(3) & 0xc0000000;
		PfxUInt32 packedId = aabbA.get32(3) & 0x3fffffff;
		PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;;
		PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
		PfxUInt32 rightId = packedId&0x000003ff;

		if(flag != 0) {
			bvhStack.push(mesh->m_bvhNodes[rightId]);
			bvhStack.push(mesh->m_bvhNodes[leftId]);
			continue;
		}

		{
			const PfxQuantizedFacetBvh &facet = mesh->m_facets[nodeId];
			
			// デコード
			PfxDecodedFacet decodedFacet;
			const PfxUInt32 vId[3] = {facet.m_vertIds[0],facet.m_vertIds[1],facet.m_vertIds[2]};

		for(int v=0;v<3;v++) {
			if(!decodedMesh.isDecodedVertex(vId[v])) {
				decodedMesh.m_verts[vId[v]] = largeMesh->decodePosition(mesh->m_verts[vId[v]]);
				decodedMesh.m_decodedVertex[vId[v]>>5] |= 1 << (vId[v]&0x1f);
			}
		}
			
			PfxTriangle triangle(
				decodedMesh.m_verts[vId[0]],
				decodedMesh.m_verts[vId[1]],
				decodedMesh.m_verts[vId[2]]);
			
			if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_ONLY && 
				pfxIntersectRayTriangleWithoutBackFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = largeMesh->decodeNormal(facet.m_normal);
				ret = true;
			}
			else if(facetMode == SCE_PFX_RAY_FACET_MODE_BACK_ONLY && 
				pfxIntersectRayTriangleWithoutFrontFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = largeMesh->decodeNormal(facet.m_normal);
				ret = true;
			}
			else if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_AND_BACK && 
				pfxIntersectRayTriangle(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = largeMesh->decodeNormal(facet.m_normal);
				ret = true;
			}
		}
	}
	
	if(ret) {
		// 面のローカル座標を算出
		const PfxQuantizedFacetBvh &facet = mesh->m_facets[nearest_f];

		PfxTriangle triangle(
			decodedMesh.m_verts[facet.m_vertIds[0]],
			decodedMesh.m_verts[facet.m_vertIds[1]],
			decodedMesh.m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(rayStart+t*rayDir,triangle,s,t);
		
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setFacetId(nearest_f);
		subData.setUserData(facet.m_userData);
		
		variable = nearest_t;
		normal = nearest_nml;
	}

	return ret;
}

static SCE_PFX_FORCE_INLINE
PfxBool pfxIntersectRayTriMesh(
	const PfxLargeTriMesh *largeMesh,const PfxExpandedTriMeshBvh *mesh,
	const PfxVector3 &rayStart,const PfxVector3 &rayDir,
	PfxUInt32 facetMode,PfxFloat &variable,PfxVector3 &normal,PfxSubData &subData)
{
	bool ret = false;
	PfxFloat nearest_t = variable;
	PfxFloat3 nearest_nml(0.0f);
	PfxUInt32 nearest_f = 0;

	PfxBvhStack< PfxAabb16 > bvhStack;
	bvhStack.push( mesh->m_bvhNodes[mesh->m_bvhRootId] );

	while(!bvhStack.isEmpty()) {
		PfxAabb16 aabbA = bvhStack.pop();
		
		PfxVecInt3 minA(pfxGetXMin(aabbA),pfxGetYMin(aabbA),pfxGetZMin(aabbA));
		PfxVecInt3 maxA(pfxGetXMax(aabbA),pfxGetYMax(aabbA),pfxGetZMax(aabbA));

		PfxVector3 aabbMin,aabbMax;

		aabbMin = largeMesh->getWorldPosition(minA);
		aabbMax = largeMesh->getWorldPosition(maxA);

		PfxFloat cur_t = 1.0f;

		if(!pfxIntersectRayAABBFast(rayStart,rayDir,(aabbMax+aabbMin)*0.5f,(aabbMax-aabbMin)*0.5f,cur_t)) {
			continue;
		}

		if( nearest_t <= cur_t ) continue;

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

		{
			const PfxExpandedFacetBvh &facet = mesh->m_facets[nodeId];
			
			PfxTriangle triangle(
				mesh->m_verts[facet.m_vertIds[0]],
				mesh->m_verts[facet.m_vertIds[1]],
				mesh->m_verts[facet.m_vertIds[2]]);
			
			if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_ONLY && 
				pfxIntersectRayTriangleWithoutBackFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = facet.m_normal;
				ret = true;
			}
			else if(facetMode == SCE_PFX_RAY_FACET_MODE_BACK_ONLY && 
				pfxIntersectRayTriangleWithoutFrontFace(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = facet.m_normal;
				ret = true;
			}
			else if(facetMode == SCE_PFX_RAY_FACET_MODE_FRONT_AND_BACK && 
				pfxIntersectRayTriangle(rayStart,rayDir,triangle,cur_t) && cur_t < nearest_t) {
				nearest_f = nodeId;
				nearest_t = cur_t;
				nearest_nml = facet.m_normal;
				ret = true;
			}
		}
	}
	
	if(ret) {
		// 面のローカル座標を算出
		const PfxExpandedFacetBvh &facet = mesh->m_facets[nearest_f];

		PfxTriangle triangle(
			mesh->m_verts[facet.m_vertIds[0]],
			mesh->m_verts[facet.m_vertIds[1]],
			mesh->m_verts[facet.m_vertIds[2]]);
		
		PfxFloat s=0.0f,t=0.0f;
		pfxGetLocalCoords(rayStart+t*rayDir,triangle,s,t);
		
		subData.m_type = PfxSubData::MESH_INFO;
		subData.setFacetLocalS(s);
		subData.setFacetLocalT(t);
		subData.setFacetId(nearest_f);
		subData.setUserData(facet.m_userData);
		
		variable = nearest_t;
		normal = nearest_nml;
	}

	return ret;
}

PfxBool pfxIntersectRayLargeTriMeshBvh(const PfxRayInput &ray,PfxRayOutput &out,const PfxLargeTriMesh *largeMesh,const PfxTransform3 &transform)
{
	PfxBool ret = false;

	// レイをラージメッシュのローカル座標へ変換
	PfxTransform3 transformLMesh = orthoInverse(transform);
	PfxVector3 rayStartPosition = transformLMesh.getUpper3x3() * ray.m_startPosition + transformLMesh.getTranslation();
	PfxVector3 rayDirection = transformLMesh.getUpper3x3() * ray.m_direction;
	PfxVector3 rayEndPosition = rayStartPosition+rayDirection;

	PfxVecInt3 s,e,aabbMinL,aabbMaxL;

	largeMesh->getLocalPosition(minPerElem(rayStartPosition,rayEndPosition),maxPerElem(rayStartPosition,rayEndPosition),aabbMinL,aabbMaxL);

	{
		PfxBvhStack<PfxAabb16> bvhStack;
		bvhStack.push( largeMesh->m_bvhNodes[largeMesh->m_bvhRootId] );

		while( ! bvhStack.isEmpty() )
		{
			PfxAabb16 aabbB = bvhStack.pop();

			//	tend to jump out ASAP
			if(aabbMaxL.getX() < pfxGetXMin(aabbB) || aabbMinL.getX() > pfxGetXMax(aabbB)) continue;
			if(aabbMaxL.getY() < pfxGetYMin(aabbB) || aabbMinL.getY() > pfxGetYMax(aabbB)) continue;
			if(aabbMaxL.getZ() < pfxGetZMin(aabbB) || aabbMinL.getZ() > pfxGetZMax(aabbB)) continue;

			PfxVecInt3 bmin(pfxGetXMin(aabbB),pfxGetYMin(aabbB),pfxGetZMin(aabbB));
			PfxVecInt3 bmax(pfxGetXMax(aabbB),pfxGetYMax(aabbB),pfxGetZMax(aabbB));

			PfxVector3 aabbMin,aabbMax;

			aabbMin = largeMesh->getWorldPosition(bmin);
			aabbMax = largeMesh->getWorldPosition(bmax);

			PfxFloat tmpVariable = 1.0f;
			PfxVector3 tmpNormal;

			if( !pfxIntersectRayAABBFast( rayStartPosition,rayDirection, (aabbMax+aabbMin)*0.5f, (aabbMax-aabbMin)*0.5f, tmpVariable) )
			{
				continue;
			}

			if( out.m_variable <= tmpVariable )
			{
				continue;
			}

			//	informations about back-tracking trees
			PfxUInt32 flag = aabbB.get32(3) & 0xc0000000;
			PfxUInt32 packedId = aabbB.get32(3) & 0x3fffffff;
			PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;
			PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
			PfxUInt32 rightId = packedId&0x000003ff;

			//	if it's not leaf, proceed to the next nodes.
			if ( flag != 0 )
			{
				bvhStack.push( largeMesh->m_bvhNodes[rightId]);
				bvhStack.push( largeMesh->m_bvhNodes[leftId]);
				continue;
			}

			//	now, check whether this one intersects the island
			PfxSubData subData;
			tmpVariable = out.m_variable;
			
			if(largeMesh->m_type & 0x01) {
				if( pfxIntersectRayTriMesh(largeMesh,(PfxQuantizedTriMeshBvh*)largeMesh->m_islands + nodeId,rayStartPosition,rayDirection,ray.m_facetMode,tmpVariable,tmpNormal,subData) &&
					tmpVariable < out.m_variable ) {
					out.m_contactFlag = true;
					out.m_variable = tmpVariable;
					out.m_contactPoint = ray.m_startPosition + tmpVariable * ray.m_direction;
					out.m_contactNormal = transform.getUpper3x3() * tmpNormal;
					subData.setIslandId( nodeId );
					out.m_subData = subData;
					ret = true;
				}
			}
			else {
				if( pfxIntersectRayTriMesh(largeMesh,(PfxExpandedTriMeshBvh*)largeMesh->m_islands + nodeId,rayStartPosition,rayDirection,ray.m_facetMode,tmpVariable,tmpNormal,subData) &&
					tmpVariable < out.m_variable ) {
					out.m_contactFlag = true;
					out.m_variable = tmpVariable;
					out.m_contactPoint = ray.m_startPosition + tmpVariable * ray.m_direction;
					out.m_contactNormal = transform.getUpper3x3() * tmpNormal;
					subData.setIslandId( nodeId );
					out.m_subData = subData;
					ret = true;
				}
			}
		}
	}

	return ret;
}

PfxBool pfxIntersectRayLargeTriMeshArray(const PfxRayInput &ray,PfxRayOutput &out,const PfxLargeTriMesh *largeMesh,const PfxTransform3 &transform)
{
	PfxBool ret = false;
	
	// レイをラージメッシュのローカル座標へ変換
	PfxTransform3 transformLMesh = orthoInverse(transform);
	PfxVector3 rayStartPosition = transformLMesh.getUpper3x3() * ray.m_startPosition + transformLMesh.getTranslation();
	PfxVector3 rayDirection = transformLMesh.getUpper3x3() * ray.m_direction;
	
	PfxVecInt3 s,e,aabbMinL,aabbMaxL;

	s = largeMesh->getLocalPosition(rayStartPosition);
	e = largeMesh->getLocalPosition(rayStartPosition+rayDirection);

	aabbMinL = minPerElem(s,e);
	aabbMaxL = maxPerElem(s,e);
	
	PfxUInt32 numIslands = largeMesh->m_numIslands;
	
	{
	for(PfxUInt32 i=0;i<numIslands;i++) {
		PfxAabb16 aabbB = largeMesh->m_aabbList[i];
		if(aabbMaxL.getX() < pfxGetXMin(aabbB) || aabbMinL.getX() > pfxGetXMax(aabbB)) continue;
		if(aabbMaxL.getY() < pfxGetYMin(aabbB) || aabbMinL.getY() > pfxGetYMax(aabbB)) continue;
		if(aabbMaxL.getZ() < pfxGetZMin(aabbB) || aabbMinL.getZ() > pfxGetZMax(aabbB)) continue;

			PfxVector3 aabbMin,aabbMax;
			aabbMin = largeMesh->getWorldPosition(PfxVecInt3((PfxFloat)pfxGetXMin(aabbB),(PfxFloat)pfxGetYMin(aabbB),(PfxFloat)pfxGetZMin(aabbB)));
			aabbMax = largeMesh->getWorldPosition(PfxVecInt3((PfxFloat)pfxGetXMax(aabbB),(PfxFloat)pfxGetYMax(aabbB),(PfxFloat)pfxGetZMax(aabbB)));

			PfxFloat tmpVariable = 1.0f;
			PfxVector3 tmpNormal;

			if( !pfxIntersectRayAABBFast(
				rayStartPosition,rayDirection,
				(aabbMax+aabbMin)*0.5f,
				(aabbMax-aabbMin)*0.5f,
				tmpVariable) )
				continue;
			
			if( out.m_variable <= tmpVariable ) continue;

			// アイランドとの交差チェック
			void *island=NULL;
		if(largeMesh->m_type & 0x01) {
			island = ((PfxQuantizedTriMesh*)largeMesh->m_islands) + i;
		}
		else {
			island = ((PfxExpandedTriMesh*)largeMesh->m_islands) + i;
		}
			
			PfxSubData subData;
			tmpVariable = out.m_variable;

			if(largeMesh->m_type & 0x01) {
				if( pfxIntersectRayTriMesh(largeMesh,(PfxQuantizedTriMesh*)island,rayStartPosition,rayDirection,ray.m_facetMode,tmpVariable,tmpNormal,subData) &&
					tmpVariable < out.m_variable ) {
					out.m_contactFlag = true;
					out.m_variable = tmpVariable;
					out.m_contactPoint = ray.m_startPosition + tmpVariable * ray.m_direction;
					out.m_contactNormal = transform.getUpper3x3() * tmpNormal;
					subData.setIslandId(i);
					out.m_subData = subData;
					ret = true;
				}
			}
			else {
				if( pfxIntersectRayTriMesh(largeMesh,(PfxExpandedTriMesh*)island,rayStartPosition,rayDirection,ray.m_facetMode,tmpVariable,tmpNormal,subData) &&
					tmpVariable < out.m_variable ) {
					out.m_contactFlag = true;
					out.m_variable = tmpVariable;
					out.m_contactPoint = ray.m_startPosition + tmpVariable * ray.m_direction;
					out.m_contactNormal = transform.getUpper3x3() * tmpNormal;
					subData.setIslandId(i);
					out.m_subData = subData;
					ret = true;
				}
			}

		}
	}

	return ret;
}

PfxBool pfxIntersectRayLargeTriMesh(const PfxRayInput &ray,PfxRayOutput &out,const void *shape,const PfxTransform3 &transform)
{
	const PfxLargeTriMesh *largeMesh = (PfxLargeTriMesh*)shape;
	
	if(largeMesh->isUsingBvh()) {
		return pfxIntersectRayLargeTriMeshBvh( ray, out, largeMesh, transform );
	}
	
	return pfxIntersectRayLargeTriMeshArray( ray, out, largeMesh, transform );
}

} //namespace PhysicsEffects
} //namespace sce
