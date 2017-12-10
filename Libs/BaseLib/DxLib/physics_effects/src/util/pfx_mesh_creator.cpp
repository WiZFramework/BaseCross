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

#include "../../include/physics_effects/util/pfx_mesh_creator.h"
#include "../../include/physics_effects/util/pfx_array.h"
#include "../base_level/collision/pfx_intersect_common.h"

namespace sce {
namespace PhysicsEffects {

/**
 * check the value of flag in PfxCreateConvexMeshParam
 * 
 * @param param  a reference to the param to be investigated
 * 
 * @return SCE_PFX_OK if nothing wrong with the flag
 *         SCE_PFX_ERR_INVALID_FLAG if something went wrong
 */
PfxInt32 checkFlagsForConvexMesh(const PfxCreateConvexMeshParam& param)
{
	//	check flag ranged
	const PfxUInt32 all_flag =	~(	SCE_PFX_MESH_FLAG_NORMAL_FLIP|
									SCE_PFX_MESH_FLAG_16BIT_INDEX|
									SCE_PFX_MESH_FLAG_32BIT_INDEX|
									SCE_PFX_MESH_FLAG_AUTO_ELIMINATION
									);
	if ((param.flag&all_flag)!=0)
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	//	check method of indexing
	if ((param.flag&SCE_PFX_MESH_FLAG_16BIT_INDEX)&&(param.flag&SCE_PFX_MESH_FLAG_32BIT_INDEX))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	if ( ! (param.flag&SCE_PFX_MESH_FLAG_16BIT_INDEX || param.flag&SCE_PFX_MESH_FLAG_32BIT_INDEX) )
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}

	//	checking variable type of the indices of triangles
	if ((param.flag & SCE_PFX_MESH_FLAG_32BIT_INDEX) && (param.triangleStrideBytes<sizeof(PfxUInt32)*3))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	/*	should this operation been taken ? 
	if ((param.flag & SCE_PFX_MESH_FLAG_16BIT_INDEX) && (param.triangleStrideBytes>sizeof(PfxUInt16)*3))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	*/


	return SCE_PFX_OK;
}

PfxUInt32 checkLargeMeshParams(const PfxCreateLargeTriMeshParam &param)
{
	//	check the params based on numVerts
	PfxUInt32 max_index = 0;

	//	check the params based on numTriangles
	for(PfxUInt32 i=0;i<param.numTriangles;i++)
	{
		void *ids = (void*)((uintptr_t)param.triangles + param.triangleStrideBytes * i);

		PfxUInt32 idx[3];

		if(param.flag & SCE_PFX_MESH_FLAG_32BIT_INDEX)
		{
			idx[0] = ((PfxUInt32*)ids)[0];
			idx[1] = ((PfxUInt32*)ids)[1];
			idx[2] = ((PfxUInt32*)ids)[2];
		}
		else if(param.flag & SCE_PFX_MESH_FLAG_16BIT_INDEX)
		{
			idx[0] = ((PfxUInt16*)ids)[0];
			idx[1] = ((PfxUInt16*)ids)[1];
			idx[2] = ((PfxUInt16*)ids)[2];
		}

		for (PfxUInt32 d=0; d<3; ++d)
		{
			if (idx[d]>=param.numVerts)
			{
				if (param.flag & SCE_PFX_MESH_FLAG_OUTPUT_INFO)
				{
					SCE_PFX_PRINTF("found an invalid index while creating largemsh\n");
				}
				return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
			}
			max_index = (max_index<idx[d])?idx[d]:max_index;
		}
	}
	/*	this will not cause serious problem. but the users have to take care aboud the enormouse numver of vertices,
	if (max_index+1<param.numVerts)
	{
		if (param.flag & SCE_PFX_MESH_FLAG_OUTPUT_INFO)
		{
			SCE_PFX_PRINTF("found an exceeding the max number of vertice swhile creating largemsh\n");
		}
		return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
	}
	*/


	//	nothing wrong with the params
	return SCE_PFX_OK;
}

/**
 * check the value of flag in PfxCreateLargeTriMeshParam
 * 
 * @param param  a reference to the param to be investigated
 * 
 * @return SCE_PFX_OK if nothing wrong with the flag
 *         SCE_PFX_ERR_INVALID_FLAG if something went wrong
 */
PfxInt32 checkFlagsForLargeTriMesh(const PfxCreateLargeTriMeshParam& param)
{
	//	check flag ranged
	const PfxUInt32 all_flag =	param.flag >> 8;
	if (all_flag)
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	//	check method of indexing
	if ((param.flag&SCE_PFX_MESH_FLAG_16BIT_INDEX)&&(param.flag&SCE_PFX_MESH_FLAG_32BIT_INDEX))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	if ( ! (param.flag&SCE_PFX_MESH_FLAG_16BIT_INDEX || param.flag&SCE_PFX_MESH_FLAG_32BIT_INDEX) )
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}

	//	checking variable type of the indices of triangles
	if ((param.flag & SCE_PFX_MESH_FLAG_32BIT_INDEX) && (param.triangleStrideBytes<sizeof(PfxUInt32)*3))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	/*	should this operation been taken ? 
	if ((param.flag & SCE_PFX_MESH_FLAG_16BIT_INDEX) && (param.triangleStrideBytes>sizeof(PfxUInt16)*3))
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	*/


	return SCE_PFX_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 凸メッシュ作成時に使用する関数

PfxInt32 pfxCreateConvexMesh(PfxConvexMesh &convex,const PfxCreateConvexMeshParam &param)
{
	// zero clear
	memset(&convex,0,sizeof(PfxConvexMesh));

	//	check flag
	if (checkFlagsForConvexMesh(param)!=SCE_PFX_OK)
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	
	// Check input
	if(param.numVerts == 0 || param.numTriangles == 0 || !param.verts || !param.triangles)
		return SCE_PFX_ERR_INVALID_VALUE;
	
	if(param.numVerts > SCE_PFX_NUMMESHVERTICES || param.numTriangles > SCE_PFX_NUMMESHFACETS)
		return SCE_PFX_ERR_OUT_OF_RANGE;
	
	PfxArray<PfxVector3> vertList(param.numVerts);
	if(!vertList.ptr()) return SCE_PFX_ERR_OUT_OF_BUFFER;

	for(PfxUInt32 i=0;i<param.numVerts;i++) {
		PfxFloat *vtx = (PfxFloat*)((uintptr_t)param.verts + param.vertexStrideBytes * i);
		vertList.push(PfxVector3(vtx[0],vtx[1],vtx[2]));
	}

	PfxArray<PfxUInt32> facetList(param.numTriangles);
	if(!facetList.ptr()) return SCE_PFX_ERR_OUT_OF_BUFFER;

	for(PfxUInt32 i=0;i<param.numTriangles;i++) {
		void *ids = (void*)((uintptr_t)param.triangles + param.triangleStrideBytes * i);
		PfxUInt32 idx[3];
		
		if(param.flag & SCE_PFX_MESH_FLAG_32BIT_INDEX) {
			if(param.flag & SCE_PFX_MESH_FLAG_NORMAL_FLIP) {
				idx[0] = ((PfxUInt32*)ids)[2];
				idx[1] = ((PfxUInt32*)ids)[1];
				idx[2] = ((PfxUInt32*)ids)[0];
			}
			else {
				idx[0] = ((PfxUInt32*)ids)[0];
				idx[1] = ((PfxUInt32*)ids)[1];
				idx[2] = ((PfxUInt32*)ids)[2];
			}
		}
		else if(param.flag & SCE_PFX_MESH_FLAG_16BIT_INDEX) {
			if(param.flag & SCE_PFX_MESH_FLAG_NORMAL_FLIP) {
				idx[0] = ((PfxUInt16*)ids)[2];
				idx[1] = ((PfxUInt16*)ids)[1];
				idx[2] = ((PfxUInt16*)ids)[0];
			}
			else {
				idx[0] = ((PfxUInt16*)ids)[0];
				idx[1] = ((PfxUInt16*)ids)[1];
				idx[2] = ((PfxUInt16*)ids)[2];
			}
		}
		else {
			return SCE_PFX_ERR_INVALID_FLAG;
		}

		//	this code will check the overruning the buffers.
		if ( idx[0]>=SCE_PFX_NUMMESHVERTICES || idx[1]>=SCE_PFX_NUMMESHVERTICES || idx[2]>=SCE_PFX_NUMMESHVERTICES)
		{
			return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
		}

		if (param.flag & SCE_PFX_MESH_FLAG_AUTO_ELIMINATION)
		{
			//	eliminate the facets that has too small area
			PfxFloat area = lengthSqr(cross(vertList[idx[1]]-vertList[idx[0]],vertList[idx[2]]-vertList[idx[0]]));
			if (area<0.00001f) {
				continue;
			}
		}
		
		facetList.push(idx[0]);
		facetList.push(idx[1]);
		facetList.push(idx[2]);
	}

	convex.m_numVerts = vertList.size();
	convex.m_verts = (PfxVector3 *)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxVector3)*convex.m_numVerts);
	for(PfxUInt32 i=0;i<convex.m_numVerts;i++) {
		convex.m_verts[i] = vertList[i];
	}
	
	convex.m_numIndices = facetList.size();
	convex.m_indices = (PfxUInt16 *)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxUInt16)*convex.m_numIndices);
	for(PfxUInt32 i=0;i<convex.m_numIndices;i++) {
		convex.m_indices[i] = facetList[i];
	}

	convex.updateAABB();

	return SCE_PFX_OK;
}

void pfxReleaseConvexMesh(PfxConvexMesh &cmesh)
{
	SCE_PFX_UTIL_FREE(cmesh.m_verts);
	SCE_PFX_UTIL_FREE(cmesh.m_indices);
	
	cmesh.m_numVerts = 0;
	cmesh.m_numIndices = 0;
}

///////////////////////////////////////////////////////////////////////////////
// ラージメッシュ作成時に使用する構造体

struct PfxMcVert {
	PfxInt16 i;
	PfxInt16 flag;
	SCE_PFX_PADDING(1,12)
	PfxVector3  coord;
};

struct PfxMcEdge {
	PfxUInt32 vertId[2];  // 
	PfxUInt32 edgeId[2];  // 面におけるエッジのインデックス
	PfxUInt32 facetId[2]; // 
	PfxUInt32 numFacets;
	PfxUInt32 angleType;  // 辺の種類
	PfxFloat angle;		 // 辺の角度
	PfxMcEdge *next;
	
	PfxBool isAcuteAngle()
	{
		return angleType == SCE_PFX_EDGE_CONVEX && angle > 0.0f;
	}
};

struct PfxMcFacet {
	PfxMcVert *v[3];
	PfxMcEdge *e[3];
	PfxInt32 neighbor[3];	// 隣接面のインデックス
	PfxInt32 neighborEdgeId[3];	// 隣接面のエッジインデックス
	PfxFloat thickness;	// 厚み
	PfxFloat area;		// 面積
	PfxBool obsolete;
	SCE_PFX_PADDING(1,3)
	PfxUInt32  userData;
	PfxVector3 n;
	PfxVector3 aabbMin;
	PfxVector3 aabbMax;
};

struct PfxMcTriList {
	PfxMcFacet *facet;
	PfxMcTriList *next;

	PfxMcTriList()
	{
		facet = NULL;
		next = NULL;
	}
};

struct PfxMcEdgeEntry {
	PfxUInt8 vertId[2];
	PfxUInt8 facetId[2];
	PfxUInt8 numFacets;
	PfxUInt8 edgeNum[2];
	PfxUInt8 edgeId;
	SCE_PFX_PADDING(1,8)
	PfxVector3 dir;
	PfxMcEdgeEntry *next;
	SCE_PFX_PADDING(2,12)
};

struct PfxMcFacetLink {
	PfxInt32 baseEdgeId;
	PfxInt32 vid1;
	PfxInt32 vid2;
	PfxInt32 ifacetId;
	PfxInt32 iedgeId;
	PfxInt32 ofacetId;
	PfxInt32 oedgeId;
	
	PfxMcFacetLink(PfxInt32 baseEdgeId_,PfxInt32 vid1_,PfxInt32 vid2_,PfxInt32 ifacetId_,PfxInt32 iedgeId_,PfxInt32 ofacetId_,PfxInt32 oedgeId_)
	{
		baseEdgeId = baseEdgeId_;
		vid1 = vid1_;
		vid2 = vid2_;
		ifacetId = ifacetId_;
		iedgeId = iedgeId_;
		ofacetId = ofacetId_;
		oedgeId = oedgeId_;
	};
};

typedef PfxMcFacet* PfxMcFacetPtr;

struct PfxMcIslands {
	PfxArray<PfxMcFacetPtr> facetsInIsland[SCE_PFX_MAX_LARGETRIMESH_ISLANDS];
	PfxUInt32 numIslands;
	SCE_PFX_PADDING(1,12)
	
	PfxMcIslands()
	{
		numIslands = 0;
	}
	
	void add(PfxArray<PfxMcFacetPtr> &facets)
	{
		SCE_PFX_ALWAYS_ASSERT(numIslands < SCE_PFX_MAX_LARGETRIMESH_ISLANDS);
		SCE_PFX_ALWAYS_ASSERT(facetsInIsland[numIslands].ptr());
		facetsInIsland[numIslands++] = facets;
	}
};

struct PfxMcBvhNode {
	PfxVector3 aabbMin;
	PfxVector3 aabbMax;
	PfxUInt32 nodeId;
	PfxMcBvhNode *left,*right;
	SCE_PFX_PADDING(1,4)
};


///////////////////////////////////////////////////////////////////////////////
// ラージメッシュ作成時に使用する補助関数

static
bool intersect(const PfxMcFacet &facetA,const PfxMcFacet &facetB,PfxFloat &closestDistance)
{
	const PfxFloat epsilon = 0.00001f;

	PfxVector3 pA[3] = {
		facetA.v[0]->coord,
		facetA.v[1]->coord,
		facetA.v[2]->coord
	};
	
	PfxVector3 pB[3] = {
		facetB.v[0]->coord,
		facetB.v[1]->coord,
		facetB.v[2]->coord
	};

	// 面Bが面Aの厚みを考慮した範囲内に有るかどうかチェック

	// 上下面
	{
		PfxPlane planeA(facetA.n,pA[0]);
		PfxFloat dmin = SCE_PFX_FLT_MAX;
		PfxFloat dmax = -SCE_PFX_FLT_MAX;
		for(int i=0;i<3;i++) {
			PfxFloat d = planeA.onPlane(pB[i]);
			dmin = SCE_PFX_MIN(dmin,d);
			dmax = SCE_PFX_MAX(dmax,d);
		}
		
		if(dmin > -epsilon || dmax < -facetA.thickness) return false;
		
		// 面Aと面Bの最近接距離
		if(dmax > 0.0f) {
			// 面A,Bは交差
			return false;
		}
		else if(dmax > -epsilon) {
			// 隣接面
			return false;
		}
		else {
			closestDistance = -dmax;
		}
	}
	
	// 側面
	for(int p=0;p<3;p++) {
		PfxVector3 sideVec = normalize(cross((pA[(p+1)%3]-pA[p]),facetA.n));
		PfxPlane planeA(sideVec,pA[p]);

		PfxFloat dmin = SCE_PFX_FLT_MAX;
		for(int i=0;i<3;i++) {
			PfxFloat d = planeA.onPlane(pB[i]);
			dmin = SCE_PFX_MIN(dmin,d);
		}
		
		if(dmin > -epsilon) return false;
	}
	
	return true;
}

static
PfxInt32 divideMeshes(
	PfxUInt32 numFacetsLimit,PfxFloat islandsRatio,
	PfxMcIslands &islands,
	PfxArray<PfxMcFacetPtr> &facets,
	const PfxVector3 &center,const PfxVector3 &half)
{
	PfxFloat halfLimit = length(half) * islandsRatio;
	
	// 含まれる面数が規定値以下であれば、アイランドに登録
	if((facets.size() <= SCE_PFX_NUMMESHFACETS && length(half) < halfLimit) || 
		(facets.size() <= numFacetsLimit ) ) {
		if(islands.numIslands >= SCE_PFX_MAX_LARGETRIMESH_ISLANDS) {
			return SCE_PFX_ERR_OUT_OF_RANGE_ISLAND;
		}
		else {
			islands.add(facets);
			return SCE_PFX_OK;
		}
	}

	// さらに分割
	PfxVector3 newCenter0,newCenter1;
	PfxVector3 newHalf0,newHalf1;
	PfxArray<PfxMcFacetPtr> newFacets0(facets.size());
	PfxArray<PfxMcFacetPtr> newFacets1(facets.size());
	
	if(!(newFacets0.ptr()&&newFacets1.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;
	
	// 最も適切と思われる分離軸を探す
	int divAxis;
	{
		if(half[0] > half[1]) {
			if(half[0] > half[2]) {
				divAxis = 0;
			}
			else if(half[1] > half[2]) {
				divAxis = 1;
			}
			else {
				divAxis = 2;
			}
		}
		else {
			if(half[1] > half[2]) {
				divAxis = 1;
			}
			else if(half[0] > half[2]) {
				divAxis = 0;
			}
			else {
				divAxis = 2;
			}
		}
	}

	// 中心で分割して、さらに再帰的に処理を続ける
	{
		PfxVector3 movCenter(0.0f);
		movCenter[divAxis] = 0.5f*half[divAxis];
		
		newCenter0 = center + movCenter;
		newCenter1 = center - movCenter;
		newHalf0 = half;
		newHalf0[divAxis] *= 0.5f;
		newHalf1 = newHalf0;
	}
	
	// 新しいAABBに含まれる面をそれぞれの領域に分配
	for(PfxUInt32 f=0;f<facets.size();f++) {
		// 面のAABB
		PfxVector3 facetCenter = (facets[f]->aabbMin + facets[f]->aabbMax) * 0.5f;
		PfxVector3 facetHalf = (facets[f]->aabbMax - facets[f]->aabbMin) * 0.5f;
		
		// AABB判定
		if(!(pfxAbsf(newCenter0[divAxis]-facetCenter[divAxis]) > (newHalf0[divAxis]+facetHalf[divAxis]))) {
			// この面はAABB0に登録
			newFacets0.push(facets[f]);
		}
		else {
			// この面はAABB1に登録
			newFacets1.push(facets[f]);
		}
	}

	PfxInt32 ret = SCE_PFX_OK;

	if(newFacets0.size() < newFacets1.size()) {
		if(newFacets0.size() > 0)
			ret = divideMeshes(numFacetsLimit,islandsRatio,islands,newFacets0,newCenter0,newHalf0);
		if(ret == SCE_PFX_OK && newFacets1.size() > 0)
			ret = divideMeshes(numFacetsLimit,islandsRatio,islands,newFacets1,newCenter1,newHalf1);
	}
	else {
		if(newFacets1.size() > 0)
			ret = divideMeshes(numFacetsLimit,islandsRatio,islands,newFacets1,newCenter1,newHalf1);
		if(ret == SCE_PFX_OK && newFacets0.size() > 0)
			ret = divideMeshes(numFacetsLimit,islandsRatio,islands,newFacets0,newCenter0,newHalf0);
	}
	
	return ret;
}

static
PfxInt32 countIsland(const PfxArray<PfxMcFacetPtr> &facets,PfxUInt32 &numEdges,PfxUInt32 &numVerts)
{
	numEdges = 0;
	numVerts = 0;
	
	if(facets.empty()) return SCE_PFX_OK;
	
	PfxUInt32 vertsFlag[(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32];
	memset(vertsFlag,0,sizeof(PfxUInt32)*((SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32));
	
	PfxArray<PfxMcEdgeEntry*> edgeHead(facets.size()*3);
	PfxArray<PfxMcEdgeEntry> edgeList(facets.size()*3);
	
	if(!(edgeHead.ptr()&&edgeList.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;
	
	PfxMcEdgeEntry* nl = NULL;
	edgeHead.assign(facets.size()*3,nl);
	edgeList.assign(facets.size()*3,PfxMcEdgeEntry());
	
	PfxUInt32 vcnt = 0;
	PfxUInt32 ecnt = 0;
	for(PfxUInt32 f=0;f<facets.size();f++) {
		// Vertex
		for(int v=0;v<3;v++) {
			PfxMcVert *vert = facets[f]->v[v];
			PfxUInt32 idx = vert->i;
			PfxUInt32 mask = 1 << (idx & 31);
			if((vertsFlag[idx>>5] & mask) == 0) {
				if(vcnt >= SCE_PFX_NUMMESHVERTICES) {
					return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
				}
                vertsFlag[idx>>5] |= mask;
				vert->flag = vcnt++;// アイランド単位の頂点インデックス
			}
		}
		
		// Edge
		for(int v=0;v<3;v++) {
			PfxUInt32 viMin = SCE_PFX_MIN(facets[f]->v[v]->flag,facets[f]->v[(v+1)%3]->flag);
			PfxUInt32 viMax = SCE_PFX_MAX(facets[f]->v[v]->flag,facets[f]->v[(v+1)%3]->flag);
			PfxUInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(facets.size()*3));
			for(PfxMcEdgeEntry *e=edgeHead[key];;e=e->next) {
				if(!e) {
					edgeList[ecnt].vertId[0] = viMin;
					edgeList[ecnt].vertId[1] = viMax;
					edgeList[ecnt].numFacets = 1;
					edgeList[ecnt].next = edgeHead[key];
					edgeHead[key] = &edgeList[ecnt];
					
					if(ecnt >= SCE_PFX_NUMMESHEDGES) {
						return SCE_PFX_ERR_OUT_OF_RANGE_EDGE;
					}
					ecnt++;
					break;
				}
				
				if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
					SCE_PFX_ALWAYS_ASSERT(e->numFacets==1);
					e->numFacets = 2;
					break;
				}
			}
		}
	}
	
	numEdges = ecnt;
	numVerts = vcnt;
	
	return SCE_PFX_OK;
}


static
PfxInt32 submitIsland(const PfxCreateLargeTriMeshParam &param,PfxExpandedTriMesh &island,const PfxArray<PfxMcFacetPtr> &facets,const PfxLargeTriMesh &lmesh)
{
	(void) lmesh;

	if(facets.empty()) return SCE_PFX_OK;
	
	PfxUInt32 vertsFlag[(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32];
	memset(vertsFlag,0,sizeof(PfxUInt32)*((SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32));
	
	PfxArray<PfxMcEdgeEntry*> edgeHead(facets.size()*3);
	PfxArray<PfxMcEdgeEntry> edgeList(facets.size()*3);
	
	if(!(edgeHead.ptr()&&edgeList.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;
	
	PfxMcEdgeEntry* nl = NULL;
	edgeHead.assign(facets.size()*3,nl);
	edgeList.assign(facets.size()*3,PfxMcEdgeEntry());
	
	PfxUInt32 vcnt = 0;
	PfxUInt32 ecnt = 0;
	for(PfxUInt32 f=0;f<facets.size();f++) {
		PfxMcFacet &iFacet = *facets[f];
		PfxMcEdge *iEdge[3] = {
			iFacet.e[0],
			iFacet.e[1],
			iFacet.e[2],
		};

		PfxExpandedFacet &oFacet = island.m_facets[f];
		
		oFacet.m_center = (PfxFloat3)(0.5f * (iFacet.aabbMax + iFacet.aabbMin));
		oFacet.m_half = (PfxFloat3)(0.5f * (iFacet.aabbMax - iFacet.aabbMin) + PfxVector3(0.00001f)); // Slightly stretch to avoid collision hole
		oFacet.m_normal = (PfxFloat3)iFacet.n;
		oFacet.m_thickness = iFacet.thickness;
		
		// Vertex
		for(int v=0;v<3;v++) {
			PfxMcVert *vert = facets[f]->v[v];
			PfxUInt32 idx = vert->i;
			PfxUInt32 mask = 1 << (idx & 31);
			if((vertsFlag[idx>>5] & mask) == 0) {
				if(vcnt >= SCE_PFX_NUMMESHVERTICES) {
					return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
				}
				vertsFlag[idx>>5] |= mask;
				SCE_PFX_ALWAYS_ASSERT(vcnt < island.m_numVerts);
				vert->flag = vcnt++;// アイランド単位の頂点インデックス
				island.m_verts[vert->flag] = (PfxFloat3)vert->coord;
			}

			oFacet.m_vertIds[v] = (PfxUInt8)vert->flag;
		}
		
		// Edge
		for(int v=0;v<3;v++) {
			PfxUInt32 viMin = SCE_PFX_MIN(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
			PfxUInt32 viMax = SCE_PFX_MAX(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
			PfxUInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(island.m_numFacets*3));
			for(PfxMcEdgeEntry *e=edgeHead[key];;e=e->next) {
				if(!e) {
					edgeList[ecnt].vertId[0] = viMin;
					edgeList[ecnt].vertId[1] = viMax;
					edgeList[ecnt].facetId[0] = f;
					edgeList[ecnt].numFacets = 1;
					edgeList[ecnt].edgeNum[0] = v;
					edgeList[ecnt].edgeId = ecnt;
					if(viMin == oFacet.m_vertIds[v]) {
						edgeList[ecnt].dir = normalize(facets[f]->v[(v+1)%3]->coord-facets[f]->v[v]->coord);
					}
					else {
						edgeList[ecnt].dir = normalize(facets[f]->v[v]->coord-facets[f]->v[(v+1)%3]->coord);
					}
					edgeList[ecnt].next = edgeHead[key];
					edgeHead[key] = &edgeList[ecnt];
					
					PfxEdge edge;
					edge.m_angleType = iEdge[v]->angleType;
					// 厚み角の設定 0～πを0～255の整数値に変換して格納
					if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS)
						edge.m_tilt = (PfxUInt8)((iEdge[v]->angle/(0.5f*SCE_PFX_PI))*255.0f);
					edge.m_vertId[0] = viMin;
					edge.m_vertId[1] = viMax;
					
					oFacet.m_edgeIds[v] = ecnt;
					island.m_edges[ecnt] = edge;
					ecnt++;
					SCE_PFX_ALWAYS_ASSERT(ecnt <= island.m_numEdges);
					break;
				}
				
				if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
					SCE_PFX_ALWAYS_ASSERT(e->numFacets==1);
					e->facetId[1] = f;
					e->edgeNum[1] = v;
					e->numFacets = 2;
					oFacet.m_edgeIds[v] = e->edgeId;
					break;
				}
			}
		}

		oFacet.m_userData = facets[f]->userData;
	}
	
	return SCE_PFX_OK;
}

static
PfxInt32 submitIsland(const PfxCreateLargeTriMeshParam &param,PfxExpandedTriMeshBvh &island,const PfxArray<PfxMcFacetPtr> &facets,const PfxLargeTriMesh &lmesh)
{
	SCE_PFX_ALWAYS_ASSERT(island.m_numFacets == facets.size());
	
	if(facets.empty()) return SCE_PFX_OK;
	
	// BVHの構築
	{
		PfxMcBvhNode *bvhNodes = (PfxMcBvhNode*)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMcBvhNode)*(island.m_numFacets*2-1));
		if(!bvhNodes) return SCE_PFX_ERR_OUT_OF_BUFFER;

		PfxArray<PfxUInt32> bvhNodeIndices(island.m_numFacets*2-1);
		if(!bvhNodeIndices.ptr()) return SCE_PFX_ERR_OUT_OF_BUFFER;

		// AABB配列を作成する
		PfxUInt32 numBvhNodes = facets.size();
		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			bvhNodes[i].aabbMin = facets[i]->aabbMin;
			bvhNodes[i].aabbMax = facets[i]->aabbMax;
			bvhNodes[i].nodeId = i;
			bvhNodes[i].left = NULL;
			bvhNodes[i].right = NULL;
			bvhNodeIndices.push(i);
		}

		// 最下層から順に最もAABBサイズの小さくなる組み合わせを検出する ※O(n^2)
		while(bvhNodeIndices.size() > 1) {
			// AABBが最小となるペアを検出
			PfxUInt32 i=0,j=0;
			{
				PfxUInt32 pMin=0,qMin=0;
				PfxFloat volumeMin = SCE_PFX_FLT_MAX;
				for(unsigned int p=0;p<bvhNodeIndices.size();p++) {
					for(unsigned int q=p+1;q<bvhNodeIndices.size();q++) {
						PfxMcBvhNode &nodeA = bvhNodes[bvhNodeIndices[p]];
						PfxMcBvhNode &nodeB = bvhNodes[bvhNodeIndices[q]];
						PfxVector3 aabbMin = minPerElem(nodeA.aabbMin,nodeB.aabbMin);
						PfxVector3 aabbMax = maxPerElem(nodeA.aabbMax,nodeB.aabbMax);
						PfxVector3 extent = aabbMax - aabbMin;
						PfxFloat volume = extent[0] * extent[1] * extent[2];
						if(volume < volumeMin) {
							volumeMin = volume;
							pMin = p;
							qMin = q;
						}
					}
				}
				i = pMin;
				j = qMin;

				SCE_PFX_ALWAYS_ASSERT(i!=j);
			}

			// 新しいノードを作成
			PfxUInt32 newId = numBvhNodes++;
			PfxMcBvhNode &newNode = bvhNodes[newId];
			PfxMcBvhNode &leftNode = bvhNodes[bvhNodeIndices[i]];
			PfxMcBvhNode &rightNode = bvhNodes[bvhNodeIndices[j]];
			newNode.aabbMin = minPerElem(leftNode.aabbMin,rightNode.aabbMin);
			newNode.aabbMax = maxPerElem(leftNode.aabbMax,rightNode.aabbMax);
			newNode.nodeId = newId;
			newNode.left = &leftNode;
			newNode.right = &rightNode;

			// ペアのノードを除去
			bvhNodeIndices.remove(SCE_PFX_MAX(i,j));
			bvhNodeIndices.remove(SCE_PFX_MIN(i,j));

			// 新しいノードを登録
			bvhNodeIndices.push(newId);
		}

		//PfxMcBvhNode *root = &bvhNodes[bvhNodeIndices[0]];
		//printBvh(root);

		//SCE_PFX_PRINTF("numFacets %d numBvhNodes %d\n",island.m_numFacets,numBvhNodes);
		
		island.m_bvhRootId = bvhNodeIndices[0];
		
		// BVHをエンコード
		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			PfxMcBvhNode &node = bvhNodes[i];

			// Quantize
			PfxAabb16 aabb;
			
			PfxVecInt3 aabbMinL,aabbMaxL;
			lmesh.getLocalPosition(node.aabbMin,node.aabbMax,aabbMinL,aabbMaxL);
			
			pfxSetXMin(aabb,aabbMinL.getX());
			pfxSetXMax(aabb,aabbMaxL.getX());
			pfxSetYMin(aabb,aabbMinL.getY());
			pfxSetYMax(aabb,aabbMaxL.getY());
			pfxSetZMin(aabb,aabbMinL.getZ());
			pfxSetZMax(aabb,aabbMaxL.getZ());
			
			PfxUInt32 flag = (node.left?0x40000000:0)|(node.right?0x80000000:0);
			PfxUInt32 leftId = node.left?node.left->nodeId:0;
			PfxUInt32 rightId = node.right?node.right->nodeId:0;
			PfxUInt32 packedId = (node.nodeId<<20)|(leftId<<10)|(rightId);
			
			aabb.set32(3,flag|packedId);
			
			island.m_bvhNodes[i] = aabb;
		}
		
		SCE_PFX_UTIL_FREE(bvhNodes);
	}
	
	// アイランド作成
	{
		PfxUInt32 vertsFlag[(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32];
		memset(vertsFlag,0,sizeof(PfxUInt32)*((SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32));

		PfxArray<PfxMcEdgeEntry*> edgeHead(facets.size()*3);
		PfxArray<PfxMcEdgeEntry> edgeList(facets.size()*3);

		if(!(edgeHead.ptr()&&edgeList.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;

		PfxMcEdgeEntry* nl = NULL;
		edgeHead.assign(facets.size()*3,nl);
		edgeList.assign(facets.size()*3,PfxMcEdgeEntry());
		
		PfxUInt32 vcnt = 0;
		PfxUInt32 ecnt = 0;
		for(PfxUInt32 f=0;f<facets.size();f++) {
			PfxMcFacet &iFacet = *facets[f];
			PfxMcEdge *iEdge[3] = {
				iFacet.e[0],
				iFacet.e[1],
				iFacet.e[2],
			};

			PfxExpandedFacetBvh &oFacet = island.m_facets[f];
		
			oFacet.m_normal = (PfxFloat3)iFacet.n;
			oFacet.m_thickness = iFacet.thickness;
			
			// Vertex
			for(int v=0;v<3;v++) {
				PfxMcVert *vert = facets[f]->v[v];
				PfxUInt32 idx = vert->i;
				PfxUInt32 mask = 1 << (idx & 31);
				if((vertsFlag[idx>>5] & mask) == 0) {
					if(vcnt >= SCE_PFX_NUMMESHVERTICES) {
						return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
					}
					vertsFlag[idx>>5] |= mask;
					SCE_PFX_ALWAYS_ASSERT(vcnt < island.m_numVerts);
					vert->flag = vcnt++;// アイランド単位の頂点インデックス
					island.m_verts[vert->flag] = (PfxFloat3)vert->coord;
				}

				oFacet.m_vertIds[v] = (PfxUInt8)vert->flag;
			}
			
			// Edge
			for(int v=0;v<3;v++) {
				PfxUInt32 viMin = SCE_PFX_MIN(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
				PfxUInt32 viMax = SCE_PFX_MAX(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
				PfxUInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(island.m_numFacets*3));
				for(PfxMcEdgeEntry *e=edgeHead[key];;e=e->next) {
					if(!e) {
						edgeList[ecnt].vertId[0] = viMin;
						edgeList[ecnt].vertId[1] = viMax;
						edgeList[ecnt].facetId[0] = f;
						edgeList[ecnt].numFacets = 1;
						edgeList[ecnt].edgeNum[0] = v;
						edgeList[ecnt].edgeId = ecnt;
						if(viMin == oFacet.m_vertIds[v]) {
							edgeList[ecnt].dir = normalize(facets[f]->v[(v+1)%3]->coord-facets[f]->v[v]->coord);
						}
						else {
							edgeList[ecnt].dir = normalize(facets[f]->v[v]->coord-facets[f]->v[(v+1)%3]->coord);
						}
						edgeList[ecnt].next = edgeHead[key];
						edgeHead[key] = &edgeList[ecnt];
						
						PfxEdge edge;
						edge.m_angleType = iEdge[v]->angleType;
						// 厚み角の設定 0～πを0～255の整数値に変換して格納
						if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS)
							edge.m_tilt = (PfxUInt8)((iEdge[v]->angle/(0.5f*SCE_PFX_PI))*255.0f);
						edge.m_vertId[0] = viMin;
						edge.m_vertId[1] = viMax;
						
						oFacet.m_edgeIds[v] = ecnt;
						island.m_edges[ecnt] = edge;
						ecnt++;
						SCE_PFX_ALWAYS_ASSERT(ecnt <= island.m_numEdges);
						break;
					}
					
					if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
						SCE_PFX_ALWAYS_ASSERT(e->numFacets==1);
						e->facetId[1] = f;
						e->edgeNum[1] = v;
						e->numFacets = 2;
						oFacet.m_edgeIds[v] = e->edgeId;
						break;
					}
				}
			}

			oFacet.m_userData = facets[f]->userData;
		}
		
		SCE_PFX_ALWAYS_ASSERT(island.m_numEdges == ecnt);
	}
	
	return SCE_PFX_OK;
}

static
PfxInt32 submitIsland(const PfxCreateLargeTriMeshParam &param,PfxQuantizedTriMesh &island,const PfxArray<PfxMcFacetPtr> &facets,const PfxLargeTriMesh &lmesh)
{
	if(facets.empty()) return SCE_PFX_OK;
	
	PfxUInt32 vertsFlag[(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32];
	memset(vertsFlag,0,sizeof(PfxUInt32)*((SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32));
	
	PfxArray<PfxMcEdgeEntry*> edgeHead(facets.size()*3);
	PfxArray<PfxMcEdgeEntry> edgeList(facets.size()*3);
	
	if(!(edgeHead.ptr()&&edgeList.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;
	
	PfxMcEdgeEntry* nl = NULL;
	edgeHead.assign(facets.size()*3,nl);
	edgeList.assign(facets.size()*3,PfxMcEdgeEntry());
	
	PfxQuantize3 q(2,2,2);
	PfxVector3 stretch = lmesh.decodeVector(q);

	PfxUInt32 vcnt = 0;
	PfxUInt32 ecnt = 0;
	for(PfxUInt32 f=0;f<facets.size();f++) {
		PfxMcFacet &iFacet = *facets[f];
		PfxMcEdge *iEdge[3] = {
			iFacet.e[0],
			iFacet.e[1],
			iFacet.e[2],
		};

		PfxQuantizedFacet &oFacet = island.m_facets[f];
		
		PfxVector3 center = 0.5f * (iFacet.aabbMax + iFacet.aabbMin);
		PfxVector3 half = 0.5f * (iFacet.aabbMax - iFacet.aabbMin) + stretch; // Slightly stretch facet's size to avoid collision hole
		
		oFacet.m_center = lmesh.quantizePosition(center);
		oFacet.m_half = lmesh.quantizeVector(half);
		oFacet.m_normal = lmesh.quantizeNormal(iFacet.n);
		oFacet.m_thickness = lmesh.quantizeFloat(iFacet.thickness);

		// Vertex
		for(int v=0;v<3;v++) {
			PfxMcVert *vert = facets[f]->v[v];
			PfxUInt32 idx = vert->i;
			PfxUInt32 mask = 1 << (idx & 31);
			if((vertsFlag[idx>>5] & mask) == 0) {
				if(vcnt >= SCE_PFX_NUMMESHVERTICES) {
					return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
				}
				vertsFlag[idx>>5] |= mask;
				SCE_PFX_ALWAYS_ASSERT(vcnt < island.m_numVerts);
				vert->flag = vcnt++;// アイランド単位の頂点インデックス
				island.m_verts[vert->flag] = lmesh.quantizePosition(vert->coord);
			}

			oFacet.m_vertIds[v] = (PfxUInt8)vert->flag;
		}
		//量子化後の面積チェック
		const PfxFloat epsilon = 0.00001f;
		PfxVector3 tp0 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[0]]);
		PfxVector3 tp1 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[1]]);
		PfxVector3 tp2 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[2]]);
		PfxFloat area = lengthSqr(cross(tp1-tp0,tp2-tp0));
		if(area<epsilon)
		{
			return SCE_PFX_ERR_ZERO_AREA_FACET;
		}
		
		// Edge
		for(int v=0;v<3;v++) {
			PfxUInt32 viMin = SCE_PFX_MIN(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
			PfxUInt32 viMax = SCE_PFX_MAX(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
			PfxUInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(island.m_numFacets*3));
			for(PfxMcEdgeEntry *e=edgeHead[key];;e=e->next) {
				if(!e) {
					edgeList[ecnt].vertId[0] = viMin;
					edgeList[ecnt].vertId[1] = viMax;
					edgeList[ecnt].facetId[0] = f;
					edgeList[ecnt].numFacets = 1;
					edgeList[ecnt].edgeNum[0] = v;
					edgeList[ecnt].edgeId = ecnt;
					if(viMin == oFacet.m_vertIds[v]) {
						edgeList[ecnt].dir = normalize(facets[f]->v[(v+1)%3]->coord-facets[f]->v[v]->coord);
					}
					else {
						edgeList[ecnt].dir = normalize(facets[f]->v[v]->coord-facets[f]->v[(v+1)%3]->coord);
					}
					edgeList[ecnt].next = edgeHead[key];
					edgeHead[key] = &edgeList[ecnt];
					
					PfxEdge edge;
					edge.m_angleType = iEdge[v]->angleType;
					// 厚み角の設定 0～πを0～255の整数値に変換して格納
					if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS)
						edge.m_tilt = (PfxUInt8)((iEdge[v]->angle/(0.5f*SCE_PFX_PI))*255.0f);
					edge.m_vertId[0] = viMin;
					edge.m_vertId[1] = viMax;
					
					oFacet.m_edgeIds[v] = ecnt;
					island.m_edges[ecnt] = edge;
					ecnt++;
					SCE_PFX_ALWAYS_ASSERT(ecnt <= island.m_numEdges);
					break;
				}
				
				if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
					SCE_PFX_ALWAYS_ASSERT(e->numFacets==1);
					e->facetId[1] = f;
					e->edgeNum[1] = v;
					e->numFacets = 2;
					oFacet.m_edgeIds[v] = e->edgeId;
					break;
				}
			}
		}

		oFacet.m_userData = facets[f]->userData;
	}

	// Slightly stretch edge of the island to avoid collision hole
	for(PfxUInt32 e=0;e<ecnt;e++) {
		if(edgeList[e].numFacets == 1) {
			PfxMcFacet &iFacet = *facets[edgeList[e].facetId[0]];
			PfxVector3 center = 0.5f * (iFacet.aabbMax + iFacet.aabbMin);
			
			int vfId0 = edgeList[e].edgeNum[0];
			int vfId1 = (edgeList[e].edgeNum[0]+1)%3;
			
			PfxVector3 p0 = iFacet.v[vfId0]->coord;
			PfxVector3 p1 = iFacet.v[vfId1]->coord;
			
			p0 += normalize(p0 - center) * maxElem(stretch);
			p1 += normalize(p1 - center) * maxElem(stretch);
			
			island.m_verts[iFacet.v[vfId0]->flag] = lmesh.quantizePosition(p0);
			island.m_verts[iFacet.v[vfId1]->flag] = lmesh.quantizePosition(p1);
		}
	}
	
	return SCE_PFX_OK;
}

static
PfxInt32 submitIsland(const PfxCreateLargeTriMeshParam &param,PfxQuantizedTriMeshBvh &island,const PfxArray<PfxMcFacetPtr> &facets,const PfxLargeTriMesh &lmesh)
{
	SCE_PFX_ALWAYS_ASSERT(island.m_numFacets == facets.size());
	
	if(facets.empty()) return SCE_PFX_OK;
	
	// BVHの構築
	{
		PfxMcBvhNode *bvhNodes = (PfxMcBvhNode*)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMcBvhNode)*(island.m_numFacets*2-1));
		if(!bvhNodes) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		PfxArray<PfxUInt32> bvhNodeIndices(island.m_numFacets*2-1);
		if(!bvhNodeIndices.ptr()) return SCE_PFX_ERR_OUT_OF_BUFFER;

		// AABB配列を作成する
		PfxUInt32 numBvhNodes = facets.size();
		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			bvhNodes[i].aabbMin = facets[i]->aabbMin;
			bvhNodes[i].aabbMax = facets[i]->aabbMax;
			bvhNodes[i].nodeId = i;
			bvhNodes[i].left = NULL;
			bvhNodes[i].right = NULL;
			bvhNodeIndices.push(i);
		}

		// 最下層から順に最もAABBサイズの小さくなる組み合わせを検出する ※O(n^2)
		while(bvhNodeIndices.size() > 1) {
			// AABBが最小となるペアを検出
			PfxUInt32 i=0,j=0;
			{
				PfxUInt32 pMin=0,qMin=0;
				PfxFloat volumeMin = SCE_PFX_FLT_MAX;
				for(unsigned int p=0;p<bvhNodeIndices.size();p++) {
					for(unsigned int q=p+1;q<bvhNodeIndices.size();q++) {
						PfxMcBvhNode &nodeA = bvhNodes[bvhNodeIndices[p]];
						PfxMcBvhNode &nodeB = bvhNodes[bvhNodeIndices[q]];
						PfxVector3 aabbMin = minPerElem(nodeA.aabbMin,nodeB.aabbMin);
						PfxVector3 aabbMax = maxPerElem(nodeA.aabbMax,nodeB.aabbMax);
						PfxVector3 extent = aabbMax - aabbMin;
						PfxFloat volume = extent[0] * extent[1] * extent[2];
						if(volume < volumeMin) {
							volumeMin = volume;
							pMin = p;
							qMin = q;
						}
					}
				}
				i = pMin;
				j = qMin;

				SCE_PFX_ALWAYS_ASSERT(i!=j);
			}

			// 新しいノードを作成
			PfxUInt32 newId = numBvhNodes++;
			PfxMcBvhNode &newNode = bvhNodes[newId];
			PfxMcBvhNode &leftNode = bvhNodes[bvhNodeIndices[i]];
			PfxMcBvhNode &rightNode = bvhNodes[bvhNodeIndices[j]];
			newNode.aabbMin = minPerElem(leftNode.aabbMin,rightNode.aabbMin);
			newNode.aabbMax = maxPerElem(leftNode.aabbMax,rightNode.aabbMax);
			newNode.nodeId = newId;
			newNode.left = &leftNode;
			newNode.right = &rightNode;

			// ペアのノードを除去
			bvhNodeIndices.remove(SCE_PFX_MAX(i,j));
			bvhNodeIndices.remove(SCE_PFX_MIN(i,j));

			// 新しいノードを登録
			bvhNodeIndices.push(newId);
		}

		//PfxMcBvhNode *root = &bvhNodes[bvhNodeIndices[0]];
		//printBvh(root);

		//SCE_PFX_PRINTF("numFacets %d numBvhNodes %d\n",island.m_numFacets,numBvhNodes);
		
		island.m_bvhRootId = bvhNodeIndices[0];
		
		// BVHをエンコード
		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			PfxMcBvhNode &node = bvhNodes[i];

			// Quantizedion
			PfxAabb16 aabb;

			PfxVecInt3 aabbMinL,aabbMaxL;
			lmesh.getLocalPosition(node.aabbMin,node.aabbMax,aabbMinL,aabbMaxL);
			
			pfxSetXMin(aabb,aabbMinL.getX());
			pfxSetXMax(aabb,aabbMaxL.getX());
			pfxSetYMin(aabb,aabbMinL.getY());
			pfxSetYMax(aabb,aabbMaxL.getY());
			pfxSetZMin(aabb,aabbMinL.getZ());
			pfxSetZMax(aabb,aabbMaxL.getZ());
			
			PfxUInt32 flag = (node.left?0x40000000:0)|(node.right?0x80000000:0);
			PfxUInt32 leftId = node.left?node.left->nodeId:0;
			PfxUInt32 rightId = node.right?node.right->nodeId:0;
			PfxUInt32 packedId = (node.nodeId<<20)|(leftId<<10)|(rightId);
			
			SCE_PFX_ASSERT((leftId&0xfffffc00)==0);
			SCE_PFX_ASSERT((rightId&0xfffffc00)==0);
			SCE_PFX_ASSERT((node.nodeId&0xfffffc00)==0);
			
			aabb.set32(3,flag|packedId);
			
			island.m_bvhNodes[i] = aabb;
		}
		
		SCE_PFX_UTIL_FREE(bvhNodes);
	}
	
	// アイランド作成
	{
		PfxUInt32 vertsFlag[(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32];
		memset(vertsFlag,0,sizeof(PfxUInt32)*((SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS*3+31)/32));

		PfxArray<PfxMcEdgeEntry*> edgeHead(facets.size()*3);
		PfxArray<PfxMcEdgeEntry> edgeList(facets.size()*3);
		
		if(!(edgeHead.ptr()&&edgeList.ptr())) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		PfxMcEdgeEntry* nl = NULL;
		edgeHead.assign(facets.size()*3,nl);
		edgeList.assign(facets.size()*3,PfxMcEdgeEntry());
		
		PfxUInt32 vcnt = 0;
		PfxUInt32 ecnt = 0;
		for(PfxUInt32 f=0;f<facets.size();f++) {
			PfxMcFacet &iFacet = *facets[f];
			PfxMcEdge *iEdge[3] = {
				iFacet.e[0],
				iFacet.e[1],
				iFacet.e[2],
			};

			PfxQuantizedFacetBvh &oFacet = island.m_facets[f];
		
			oFacet.m_normal = lmesh.quantizeNormal(iFacet.n);
			oFacet.m_thickness = lmesh.quantizeFloat(iFacet.thickness);
			
			// Vertex
			for(int v=0;v<3;v++) {
				PfxMcVert *vert = facets[f]->v[v];
				PfxUInt32 idx = vert->i;
				PfxUInt32 mask = 1 << (idx & 31);
				if((vertsFlag[idx>>5] & mask) == 0) {
					if(vcnt >= SCE_PFX_NUMMESHVERTICES) {
						return SCE_PFX_ERR_OUT_OF_RANGE_VERTEX;
					}
					vertsFlag[idx>>5] |= mask;
					SCE_PFX_ALWAYS_ASSERT(vcnt < island.m_numVerts);
					vert->flag = vcnt++;// アイランド単位の頂点インデックス
					island.m_verts[vert->flag] = lmesh.quantizePosition(vert->coord);
				}

				oFacet.m_vertIds[v] = (PfxUInt8)vert->flag;
			}
			//量子化後の面積チェック
			const PfxFloat epsilon = 0.00001f;
			PfxVector3 tp0 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[0]]);
			PfxVector3 tp1 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[1]]);
			PfxVector3 tp2 = lmesh.decodePosition(island.m_verts[oFacet.m_vertIds[2]]);
			PfxFloat area = lengthSqr(cross(tp1-tp0,tp2-tp0));
			if(area<epsilon)
			{
				return SCE_PFX_ERR_ZERO_AREA_FACET;
			}

			// Edge
			for(int v=0;v<3;v++) {
				PfxUInt32 viMin = SCE_PFX_MIN(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
				PfxUInt32 viMax = SCE_PFX_MAX(oFacet.m_vertIds[v],oFacet.m_vertIds[(v+1)%3]);
				PfxUInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(island.m_numFacets*3));
				for(PfxMcEdgeEntry *e=edgeHead[key];;e=e->next) {
					if(!e) {
						edgeList[ecnt].vertId[0] = viMin;
						edgeList[ecnt].vertId[1] = viMax;
						edgeList[ecnt].facetId[0] = f;
						edgeList[ecnt].numFacets = 1;
						edgeList[ecnt].edgeNum[0] = v;
						edgeList[ecnt].edgeId = ecnt;
						if(viMin == oFacet.m_vertIds[v]) {
							edgeList[ecnt].dir = normalize(facets[f]->v[(v+1)%3]->coord-facets[f]->v[v]->coord);
						}
						else {
							edgeList[ecnt].dir = normalize(facets[f]->v[v]->coord-facets[f]->v[(v+1)%3]->coord);
						}
						edgeList[ecnt].next = edgeHead[key];
						edgeHead[key] = &edgeList[ecnt];
						
						PfxEdge edge;
						edge.m_angleType = iEdge[v]->angleType;
						// 厚み角の設定 0～πを0～255の整数値に変換して格納
						if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS)
							edge.m_tilt = (PfxUInt8)((iEdge[v]->angle/(0.5f*SCE_PFX_PI))*255.0f);
						edge.m_vertId[0] = viMin;
						edge.m_vertId[1] = viMax;
						
						oFacet.m_edgeIds[v] = ecnt;
						island.m_edges[ecnt] = edge;
						ecnt++;
						SCE_PFX_ALWAYS_ASSERT(ecnt <= island.m_numEdges);
						break;
					}
					
					if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
						SCE_PFX_ALWAYS_ASSERT(e->numFacets==1);
						e->facetId[1] = f;
						e->edgeNum[1] = v;
						e->numFacets = 2;
						oFacet.m_edgeIds[v] = e->edgeId;
						break;
					}
				}
			}

			oFacet.m_userData = facets[f]->userData;
		}
		
		SCE_PFX_ALWAYS_ASSERT(island.m_numEdges == ecnt);

		PfxQuantize3 q(2,2,2);
		PfxVector3 stretch = lmesh.decodeVector(q);

		// Slightly stretch edge of the island to avoid collision hole
		for(PfxUInt32 e=0;e<ecnt;e++) {
			if(edgeList[e].numFacets == 1) {
				PfxMcFacet &iFacet = *facets[edgeList[e].facetId[0]];
				PfxVector3 center = 0.5f * (iFacet.aabbMax + iFacet.aabbMin);
				
				int vfId0 = edgeList[e].edgeNum[0];
				int vfId1 = (edgeList[e].edgeNum[0]+1)%3;
				
				PfxVector3 p0 = iFacet.v[vfId0]->coord;
				PfxVector3 p1 = iFacet.v[vfId1]->coord;
				
				p0 += normalize(p0 - center) * maxElem(stretch);
				p1 += normalize(p1 - center) * maxElem(stretch);
				
				island.m_verts[iFacet.v[vfId0]->flag] = lmesh.quantizePosition(p0);
				island.m_verts[iFacet.v[vfId1]->flag] = lmesh.quantizePosition(p1);
			}
		}
	}

	return SCE_PFX_OK;
}

///////////////////////////////////////////////////////////////////////////////
// ラージメッシュ

PfxInt32 pfxCreateLargeTriMesh(PfxLargeTriMesh &lmesh,const PfxCreateLargeTriMeshParam &param)
{
	// zero clear
	memset(&lmesh,0,sizeof(PfxLargeTriMesh));

	//	check flag
	if (checkFlagsForLargeTriMesh(param)!=SCE_PFX_OK)
	{
		return SCE_PFX_ERR_INVALID_FLAG;
	}
	// Check input
	if(param.numVerts == 0 || param.numTriangles == 0 || !param.verts || !param.triangles)
	{
		return SCE_PFX_ERR_INVALID_VALUE;
	}

	if (param.numVerts>(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHVERTICES) || 
		param.numTriangles>(SCE_PFX_MAX_LARGETRIMESH_ISLANDS*SCE_PFX_NUMMESHFACETS))
	{
		return SCE_PFX_ERR_OUT_OF_RANGE;
	}
	
	if(param.islandsRatio < 0.0f || param.islandsRatio > 1.0f)
	{
		return SCE_PFX_ERR_OUT_OF_RANGE;
	}
	
	if(param.numFacetsLimit == 0 || param.numFacetsLimit > SCE_PFX_NUMMESHFACETS)
	{
		return SCE_PFX_ERR_OUT_OF_RANGE;
	}
	
	PfxUInt32 checkparam = checkLargeMeshParams(param);
	if (checkparam!=SCE_PFX_OK)
	{
		return checkparam;
	}
	const PfxFloat epsilon = 0.00001f;
	
	PfxArray<PfxMcVert>  vertList(param.numVerts);		// 頂点配列
	PfxArray<PfxMcFacet> facetList(param.numTriangles);	// 面配列
	PfxArray<PfxMcEdge>  edgeList(param.numTriangles*3);// エッジ配列
	PfxArray<PfxMcEdge*> edgeHead(param.numTriangles*3);
	
	if(!(vertList.ptr()&&facetList.ptr()&&edgeList.ptr()&&edgeHead.ptr())) {
		return SCE_PFX_ERR_OUT_OF_BUFFER;
	}
	
	//J 頂点配列作成
	for(PfxUInt32 i=0;i<param.numVerts;i++) {
		PfxFloat *vtx = (PfxFloat*)((uintptr_t)param.verts + param.vertexStrideBytes * i);
		PfxMcVert mcv;
		mcv.flag = 0;
		mcv.i = i;
		mcv.coord = pfxReadVector3(vtx);
		vertList.push(mcv);
	}
	
	// 面配列作成
	for(PfxUInt32 i=0;i<param.numTriangles;i++) {
		void *ids = (void*)((uintptr_t)param.
							triangles + param.triangleStrideBytes * i);
		
		PfxUInt32 idx[3];
		
		if(param.flag & SCE_PFX_MESH_FLAG_32BIT_INDEX) {
			if(param.flag & SCE_PFX_MESH_FLAG_NORMAL_FLIP) {
				idx[0] = ((PfxUInt32*)ids)[2];
				idx[1] = ((PfxUInt32*)ids)[1];
				idx[2] = ((PfxUInt32*)ids)[0];
			}
			else {
				idx[0] = ((PfxUInt32*)ids)[0];
				idx[1] = ((PfxUInt32*)ids)[1];
				idx[2] = ((PfxUInt32*)ids)[2];
			}
		}
		else if(param.flag & SCE_PFX_MESH_FLAG_16BIT_INDEX) {
			if(param.flag & SCE_PFX_MESH_FLAG_NORMAL_FLIP) {
				idx[0] = ((PfxUInt16*)ids)[2];
				idx[1] = ((PfxUInt16*)ids)[1];
				idx[2] = ((PfxUInt16*)ids)[0];
			}
			else {
				idx[0] = ((PfxUInt16*)ids)[0];
				idx[1] = ((PfxUInt16*)ids)[1];
				idx[2] = ((PfxUInt16*)ids)[2];
			}
		}
		else {
			return SCE_PFX_ERR_INVALID_FLAG;
		}
		
		const PfxVector3 pnts[3] = {
			vertList[idx[0]].coord,
			vertList[idx[1]].coord,
			vertList[idx[2]].coord,
		};

		// 面積が０の面を排除
		PfxFloat area = lengthSqr(cross(pnts[1]-pnts[0],pnts[2]-pnts[0]));

		if((param.flag & SCE_PFX_MESH_FLAG_AUTO_ELIMINATION) && area < epsilon) {
			continue;
		}

		PfxMcFacet facet;
		facet.v[0] = &vertList[idx[0]];
		facet.v[1] = &vertList[idx[1]];
		facet.v[2] = &vertList[idx[2]];
		facet.e[0] = facet.e[1] = facet.e[2] = NULL;
		facet.n = normalize(cross(pnts[2]-pnts[1],pnts[0]-pnts[1]));
		facet.area = area;
		facet.obsolete = false;
		facet.thickness = param.defaultThickness;
		facet.neighbor[0] = facet.neighbor[1] = facet.neighbor[2] = -1;
		facet.neighborEdgeId[0] = facet.neighborEdgeId[1] = facet.neighborEdgeId[2] = -1;
		facet.userData = param.userData ? param.userData[i] : 0;

		facetList.push(facet);
	}

	const PfxUInt32 numTriangles = facetList.size();

	{
		PfxArray<PfxMcTriList> triEntry(numTriangles*3);
		PfxArray<PfxMcTriList*> triHead(param.numVerts);	// 頂点から面への参照リスト
		PfxInt32 cnt = 0;
		
		if(!(triEntry.ptr()&&triHead.ptr())) {
			return SCE_PFX_ERR_OUT_OF_BUFFER;
		}
		
		PfxMcTriList* nl = NULL;
		triEntry.assign(numTriangles*3,PfxMcTriList());
		triHead.assign(param.numVerts,nl);//	the capacity of trihead should be "numVerts", beacause there will be the vertices that are not held by the facets.
		
		// 頂点から面への参照リストを作成
		for(PfxUInt32 i=0;i<numTriangles;i++) {
			for(PfxUInt32 v=0;v<3;v++) {
				PfxUInt32 vertId = facetList[i].v[v]->i;
				triEntry[cnt].facet = &facetList[i];
				triEntry[cnt].next = triHead[vertId];
				triHead[vertId] = &triEntry[cnt++];
			}
		}
		
		// 同一頂点をまとめる
		if(param.flag & SCE_PFX_MESH_FLAG_AUTO_ELIMINATION) {
			for(PfxUInt32 i=0;i<param.numVerts;i++) {
				if(vertList[i].flag == 1) continue;
				for(PfxUInt32 j=i+1;j<param.numVerts;j++) {
					if(vertList[j].flag == 1) continue;

					PfxFloat lenSqr = lengthSqr(vertList[i].coord-vertList[j].coord);
					
					if(lenSqr < epsilon) {
						//SCE_PFX_PRINTF("same position %d,%d\n",i,j);
						vertList[j].flag = 1; // 同一点なのでフラグを立てる
						if (triHead[j])//	this is needed beacause if there will be the vertices that are not heald by the facets.
						{
							for(PfxMcTriList *f=triHead[j];f!=NULL;f=f->next) {
								for(PfxInt32 k=0;k<3;k++) {
									if(f->facet->v[k] == &vertList[j]) {
										f->facet->v[k] = &vertList[i]; // 頂点を付け替える
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// 接続面間の角度を算出して面にセット
	PfxMcEdge *nl = NULL;
	edgeHead.assign(numTriangles*3,nl);
	edgeList.assign(numTriangles*3,PfxMcEdge());
	
	// エッジ配列の作成
	PfxUInt32 ecnt = 0;
	for(PfxUInt32 i=0;i<numTriangles;i++) {
		PfxMcFacet &f = facetList[i];
		
		for(PfxUInt32 v=0;v<3;v++) {
			uintptr_t vp1 = ((uintptr_t)f.v[v]-(uintptr_t)&vertList[0])/sizeof(PfxMcVert);
			uintptr_t vp2 = ((uintptr_t)f.v[(v+1)%3]-(uintptr_t)&vertList[0])/sizeof(PfxMcVert);
			PfxUInt32 viMin = (PfxUInt32)SCE_PFX_MIN(vp1,vp2);
			PfxUInt32 viMax = (PfxUInt32)SCE_PFX_MAX(vp1,vp2);
			PfxInt32 key = ((0x8da6b343*viMin+0xd8163841*viMax)%(numTriangles*3));
			for(PfxMcEdge *e = edgeHead[key];;e=e->next) {
				if(!e) {
					edgeList[ecnt].vertId[0] = viMin;
					edgeList[ecnt].vertId[1] = viMax;
					edgeList[ecnt].facetId[0] = i;
					edgeList[ecnt].edgeId[0] = v;
					edgeList[ecnt].numFacets = 1;
					edgeList[ecnt].next = edgeHead[key];
					edgeList[ecnt].angleType = SCE_PFX_EDGE_CONVEX;
					edgeList[ecnt].angle = 0.0f;
					edgeHead[key] = &edgeList[ecnt];
					f.e[v] = &edgeList[ecnt];
					ecnt++;
					break;
				}
				
				if(e->vertId[0] == viMin && e->vertId[1] == viMax) {
					if(e->numFacets > 1) {
						// return SCE_PFX_ERR_EDGE_SHARED_BY_3;
						// 3つ以上の面を共有⇒3つ目以降は無視
						if(param.flag & SCE_PFX_MESH_FLAG_OUTPUT_INFO) {
							SCE_PFX_PRINTF("Warning : An edge shared by over 2 triangles is invalid.\n");
							f.obsolete = true;
						}
					}
					else {
						e->facetId[1] = i;
						e->edgeId[1] = v;
						e->numFacets = 2;
						f.e[v] = e;
						f.neighbor[v] = e->facetId[0];
						f.neighborEdgeId[v] = e->edgeId[0];
						facetList[e->facetId[0]].neighbor[e->edgeId[0]] = i;
						facetList[e->facetId[0]].neighborEdgeId[e->edgeId[0]] = e->edgeId[1];
					}
				}
			}
		}
	}
	
	// 角度を計算
	for(PfxUInt32 i=0;i<numTriangles;i++) {
		PfxMcFacet &facetA = facetList[i];
		if(facetA.obsolete) continue;
		
		PfxQueue<PfxMcFacetLink> cqueue(ecnt);
		
		if(!cqueue.ptr()) {
			return SCE_PFX_ERR_OUT_OF_BUFFER;
		}
		
		for(PfxUInt32 j=0;j<3;j++) {
			if(facetA.neighbor[j] >= 0 && !facetList[facetA.neighbor[j]].obsolete) {
				cqueue.push(PfxMcFacetLink(
					j,
					facetA.e[j]->vertId[0],facetA.e[j]->vertId[1],
					i,j,
					facetA.neighbor[j],facetA.neighborEdgeId[j]));
			}
		}

		while(!cqueue.empty()) {
			PfxMcFacetLink link = cqueue.front();
			cqueue.pop();
			
			PfxMcFacet &ofacet = facetList[link.ofacetId];
			PfxMcEdge *edge = ofacet.e[link.oedgeId];
			
			// facetAとのなす角を計算
			{
				// 面に含まれるが、このエッジに含まれない点
				PfxUInt32 ids[3] = {2,0,1};
				PfxVector3 v1 = facetA.v[ids[link.baseEdgeId]]->coord;
				PfxVector3 v2 = ofacet.v[ids[link.oedgeId]]->coord;
				
				// エッジの凹凸判定
				PfxVector3 midPnt = (v1 + v2) * 0.5f;
				PfxVector3 pntOnEdge = facetA.v[link.baseEdgeId]->coord;
				
				PfxFloat chk1 = dot(facetA.n,midPnt-pntOnEdge);
				PfxFloat chk2 = dot(ofacet.n,midPnt-pntOnEdge);
				PfxFloat chk3 = dot(facetA.n,ofacet.n);

				if(chk3 > 1.0f - epsilon) {
					if(link.ifacetId == i) edge->angleType = SCE_PFX_EDGE_FLAT;
				}
				else if(chk1 < -epsilon && chk2 < -epsilon) {
					if(link.ifacetId == i) edge->angleType = SCE_PFX_EDGE_CONVEX;

					// 厚み角の判定に使う角度をセット
					edge->angle = 0.5f*acosf(dot(facetA.n,ofacet.n));
				}
				else if(chk1 > epsilon && chk2 > epsilon) {
					if(link.ifacetId == i) edge->angleType = SCE_PFX_EDGE_CONCAVE;

					// 厚み角の判定に使う角度をセット
					edge->angle = 0.5f*acosf(chk3);
				}
			}
			
			// 次の接続面を登録（コメントアウトすると頂点で接続された面を考慮しない）
			if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS) {
				PfxInt32 nextEdgeId = (link.oedgeId+1)%3;
				PfxMcEdge *nextEdge = ofacet.e[nextEdgeId];
				if(ofacet.neighbor[nextEdgeId] >= 0 && ofacet.neighbor[nextEdgeId] != i && !facetList[ofacet.neighbor[nextEdgeId]].obsolete &&
				  ((PfxInt32)nextEdge->vertId[0] == link.vid1 || (PfxInt32)nextEdge->vertId[0] == link.vid2 || 
				   (PfxInt32)nextEdge->vertId[1] == link.vid1 || (PfxInt32)nextEdge->vertId[1] == link.vid2) ) {
					cqueue.push(PfxMcFacetLink(
						link.baseEdgeId,
						link.vid1,link.vid2,
						link.ofacetId,link.iedgeId,
						ofacet.neighbor[nextEdgeId],ofacet.neighborEdgeId[nextEdgeId]));
				}
				nextEdgeId = (link.oedgeId+2)%3;
				nextEdge = ofacet.e[nextEdgeId];
				if(ofacet.neighbor[nextEdgeId] >= 0 && ofacet.neighbor[nextEdgeId] != i && !facetList[ofacet.neighbor[nextEdgeId]].obsolete &&
				  ((PfxInt32)nextEdge->vertId[0] == link.vid1 || (PfxInt32)nextEdge->vertId[0] == link.vid2 || 
				   (PfxInt32)nextEdge->vertId[1] == link.vid1 || (PfxInt32)nextEdge->vertId[1] == link.vid2) ) {
					cqueue.push(PfxMcFacetLink(
						link.baseEdgeId,
						link.vid1,link.vid2,
						link.ofacetId,link.iedgeId,
						ofacet.neighbor[nextEdgeId],ofacet.neighborEdgeId[nextEdgeId]));
				}
			}
		}
	}
	
	// 面に厚みを付ける
	if(param.flag & SCE_PFX_MESH_FLAG_AUTO_THICKNESS) {
		for(PfxUInt32 i=0;i<numTriangles;i++) {
			PfxMcFacet &facetA = facetList[i];
			if(facetA.obsolete) continue;
			
			for(PfxUInt32 j=0;j<numTriangles;j++) {
				// 隣接面は比較対象にしない
				if( i==j ||
					j == (PfxInt32)facetA.e[0]->facetId[0] ||
					j == (PfxInt32)facetA.e[0]->facetId[1] ||
					j == (PfxInt32)facetA.e[1]->facetId[0] ||
					j == (PfxInt32)facetA.e[1]->facetId[1] ||
					j == (PfxInt32)facetA.e[2]->facetId[0] ||
					j == (PfxInt32)facetA.e[2]->facetId[1]) {
					continue;
				}
				
				PfxMcFacet &facetB = facetList[j];
				if(facetB.obsolete) continue;
				
				// 交差判定
				PfxFloat closestDistance=0;
				if(intersect(facetA,facetB,closestDistance)) {
					// 最近接距離/2を厚みとして採用
					facetA.thickness = SCE_PFX_MAX(param.defaultThickness,SCE_PFX_MIN(facetA.thickness,closestDistance * 0.5f));
				}
			}

		}
	}

	// 面の面積によって３種類に分類する
	PfxFloat areaMin=SCE_PFX_FLT_MAX,areaMax=-SCE_PFX_FLT_MAX;
	for(PfxUInt32 f=0;f<(PfxUInt32)numTriangles;f++) {
		PfxMcFacet &facet = facetList[f];
		if(facet.obsolete) continue;
		
		PfxVector3 pnts[6] = {
			facet.v[0]->coord,
			facet.v[1]->coord,
			facet.v[2]->coord,
			facet.v[0]->coord - facet.thickness * facet.n,
			facet.v[1]->coord - facet.thickness * facet.n,
			facet.v[2]->coord - facet.thickness * facet.n,
		};
		
		areaMin = SCE_PFX_MIN(areaMin,facet.area);
		areaMax = SCE_PFX_MAX(areaMax,facet.area);
		
		// 面のAABBを算出
		facetList[f].aabbMin = minPerElem(pnts[5],minPerElem(pnts[4],minPerElem(pnts[3],minPerElem(pnts[2],minPerElem(pnts[1],pnts[0])))));
		facetList[f].aabbMax = maxPerElem(pnts[5],maxPerElem(pnts[4],maxPerElem(pnts[3],maxPerElem(pnts[2],maxPerElem(pnts[1],pnts[0])))));
		
	}

	PfxFloat areaDiff = (areaMax-areaMin)/3.0f;
	PfxFloat areaLevel0,areaLevel1;
	areaLevel0 = areaMin + areaDiff;
	areaLevel1 = areaMin + areaDiff * 2.0f;

	PfxArray<PfxMcFacetPtr> facetsLv0(numTriangles);
	PfxArray<PfxMcFacetPtr> facetsLv1(numTriangles);
	PfxArray<PfxMcFacetPtr> facetsLv2(numTriangles);

	if(!(facetsLv0.ptr()&&facetsLv1.ptr()&&facetsLv2.ptr())) {
		return SCE_PFX_ERR_OUT_OF_BUFFER;
	}

	for(PfxUInt32 f=0;f<numTriangles;f++) {
		PfxMcFacet *facet = &facetList[f];
		if(facet->obsolete) continue;
		
		PfxFloat area = facet->area;
		
		if(area < areaLevel0) {
			facetsLv0.push(facet);
		}
		else if(area > areaLevel1) {
			facetsLv2.push(facet);
		}
		else {
			facetsLv1.push(facet);
		}
	}

	// アイランドの配列
	PfxMcIslands islands;
	PfxVector3 largeMeshAabbMin(SCE_PFX_FLT_MAX);
	PfxVector3 largeMeshAabbMax(-SCE_PFX_FLT_MAX);

	// レベル毎にアイランドを作成
	if(!facetsLv0.empty()) {
		// 全体のAABBを求める
		PfxVector3 aabbMin,aabbMax,center,half;
		aabbMin = facetsLv0[0]->aabbMin;
		aabbMax = facetsLv0[0]->aabbMax;
		for(PfxUInt32 f=1;f<facetsLv0.size();f++) {
			aabbMin = minPerElem(facetsLv0[f]->aabbMin,aabbMin);
			aabbMax = maxPerElem(facetsLv0[f]->aabbMax,aabbMax);
		}
		center = ( aabbMin + aabbMax ) * 0.5f;
		half = ( aabbMax - aabbMin ) * 0.5f;

		// 再帰的に処理
		PfxInt32 ret = divideMeshes(
			param.numFacetsLimit,param.islandsRatio,
			islands,
			facetsLv0,
			center,half);
		
		if(ret != SCE_PFX_OK) return ret;
		
		largeMeshAabbMin = minPerElem(largeMeshAabbMin,aabbMin);
		largeMeshAabbMax = maxPerElem(largeMeshAabbMax,aabbMax);
	}

	if(!facetsLv1.empty()) {
		// 全体のAABBを求める
		PfxVector3 aabbMin,aabbMax,center,half;
		aabbMin =facetsLv1[0]->aabbMin;
		aabbMax = facetsLv1[0]->aabbMax;
		for(PfxUInt32 f=1;f<facetsLv1.size();f++) {
			aabbMin = minPerElem(facetsLv1[f]->aabbMin,aabbMin);
			aabbMax = maxPerElem(facetsLv1[f]->aabbMax,aabbMax);
		}
		center = ( aabbMin + aabbMax ) * 0.5f;
		half = ( aabbMax - aabbMin ) * 0.5f;

		// 再帰的に処理
		PfxInt32 ret = divideMeshes(
			param.numFacetsLimit,param.islandsRatio,
			islands,
			facetsLv1,
			center,half);

		if(ret != SCE_PFX_OK) return ret;

		largeMeshAabbMin = minPerElem(largeMeshAabbMin,aabbMin);
		largeMeshAabbMax = maxPerElem(largeMeshAabbMax,aabbMax);
	}

	if(!facetsLv2.empty()) {
		// 全体のAABBを求める
		PfxVector3 aabbMin,aabbMax,center,half;
		aabbMin =facetsLv2[0]->aabbMin;
		aabbMax = facetsLv2[0]->aabbMax;
		for(PfxUInt32 f=1;f<facetsLv2.size();f++) {
			aabbMin = minPerElem(facetsLv2[f]->aabbMin,aabbMin);
			aabbMax = maxPerElem(facetsLv2[f]->aabbMax,aabbMax);
		}
		center = ( aabbMin + aabbMax ) * 0.5f;
		half = ( aabbMax - aabbMin ) * 0.5f;

		// 再帰的に処理
		PfxInt32 ret = divideMeshes(
			param.numFacetsLimit,param.islandsRatio,
			islands,
			facetsLv2,
			center,half);

		if(ret != SCE_PFX_OK) return ret;

		largeMeshAabbMin = minPerElem(largeMeshAabbMin,aabbMin);
		largeMeshAabbMax = maxPerElem(largeMeshAabbMax,aabbMax);
	}

	// ラージメッシュのオフセットとサイズを設定
	lmesh.m_offset = ( largeMeshAabbMax + largeMeshAabbMin ) * 0.5f;
	lmesh.m_half = ( largeMeshAabbMax - largeMeshAabbMin ) * 0.5f;
	lmesh.m_length = length(lmesh.m_half);

	// Check Islands
	//for(PfxInt32 i=0;i<islands.numIslands;i++) {
	//	SCE_PFX_PRINTF("island %d\n",i);
	//	for(PfxInt32 f=0;f<islands.facetsInIsland[i].size();f++) {
	//		PfxMcFacet *facet = islands.facetsInIsland[i][f];
	//		SCE_PFX_PRINTF("   %d %d %d\n",facet->v[0]->i,facet->v[1]->i,facet->v[2]->i);
	//	}
	//}

	//	building bv-tree or aabb array
	if (param.flag & SCE_PFX_MESH_FLAG_USE_BVH)
	{
		// BVHノード配列の作成
		PfxMcBvhNode *bvhNodes = (PfxMcBvhNode*)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMcBvhNode)*(islands.numIslands*2-1));
		
		if(!bvhNodes) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		PfxArray<PfxUInt32> bvhNodeIndices(islands.numIslands*2-1);
		
		if(!bvhNodeIndices.ptr()) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		// AABB配列を作成する
		PfxUInt32 numBvhNodes = islands.numIslands;
		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			PfxVector3 aabbMin = islands.facetsInIsland[i][0]->aabbMin;
			PfxVector3 aabbMax = islands.facetsInIsland[i][0]->aabbMax;
			for(PfxUInt32 j=1;j<islands.facetsInIsland[i].size();j++) {
				aabbMin = minPerElem(islands.facetsInIsland[i][j]->aabbMin,aabbMin);
				aabbMax = maxPerElem(islands.facetsInIsland[i][j]->aabbMax,aabbMax);
			}
			bvhNodes[i].aabbMin = aabbMin;
			bvhNodes[i].aabbMax = aabbMax;
			bvhNodes[i].nodeId = i;
			bvhNodes[i].left = NULL;
			bvhNodes[i].right = NULL;

			bvhNodeIndices.push(i);
		}

		// 最下層から順に最もAABBサイズの小さくなる組み合わせを検出する ※O(n^2)
		while(bvhNodeIndices.size() > 1) {
			// AABBが最小となるペアを検出
			PfxUInt32 i=0,j=0;
			{
				PfxUInt32 pMin=0,qMin=0;
				PfxFloat volumeMin = SCE_PFX_FLT_MAX;
				for(unsigned int p=0;p<bvhNodeIndices.size();p++) {
					for(unsigned int q=p+1;q<bvhNodeIndices.size();q++) {
						PfxMcBvhNode &nodeA = bvhNodes[bvhNodeIndices[p]];
						PfxMcBvhNode &nodeB = bvhNodes[bvhNodeIndices[q]];
						PfxVector3 aabbMin = minPerElem(nodeA.aabbMin,nodeB.aabbMin);
						PfxVector3 aabbMax = maxPerElem(nodeA.aabbMax,nodeB.aabbMax);
						PfxVector3 extent = aabbMax - aabbMin;
						PfxFloat volume = extent[0] * extent[1] * extent[2];
						if(volume < volumeMin) {
							volumeMin = volume;
							pMin = p;
							qMin = q;
						}
					}
				}
				i = pMin;
				j = qMin;

				SCE_PFX_ALWAYS_ASSERT(i!=j);
			}

			// 新しいノードを作成
			PfxUInt32 newId = numBvhNodes++;
			PfxMcBvhNode &newNode = bvhNodes[newId];
			PfxMcBvhNode &leftNode = bvhNodes[bvhNodeIndices[i]];
			PfxMcBvhNode &rightNode = bvhNodes[bvhNodeIndices[j]];
			newNode.aabbMin = minPerElem(leftNode.aabbMin,rightNode.aabbMin);
			newNode.aabbMax = maxPerElem(leftNode.aabbMax,rightNode.aabbMax);
			newNode.nodeId = newId;
			newNode.left = &leftNode;
			newNode.right = &rightNode;

			// ペアのノードを除去
			bvhNodeIndices.remove(SCE_PFX_MAX(i,j));
			bvhNodeIndices.remove(SCE_PFX_MIN(i,j));

			// 新しいノードを登録
			bvhNodeIndices.push(newId);
		}

		//PfxMcBvhNode *root = &bvhNodes[bvhNodeIndices[0]];
		//printBvh(root);

		// BVHをエンコード

		lmesh.m_bvhNodes = (PfxAabb16*)SCE_PFX_UTIL_ALLOC(128,sizeof(PfxAabb16)*numBvhNodes);
		lmesh.m_numBvhNodes = numBvhNodes;
		lmesh.m_bvhRootId = bvhNodeIndices[0];

		if(!lmesh.m_bvhNodes) return SCE_PFX_ERR_OUT_OF_BUFFER;

		for(PfxUInt32 i=0;i<numBvhNodes;i++) {
			PfxMcBvhNode &node = bvhNodes[i];
			
			// Quantize
			PfxAabb16 aabb;
			
			PfxVecInt3 aabbMinL,aabbMaxL;
			lmesh.getLocalPosition(node.aabbMin,node.aabbMax,aabbMinL,aabbMaxL);
			
			pfxSetXMin(aabb,aabbMinL.getX());
			pfxSetXMax(aabb,aabbMaxL.getX());
			pfxSetYMin(aabb,aabbMinL.getY());
			pfxSetYMax(aabb,aabbMaxL.getY());
			pfxSetZMin(aabb,aabbMinL.getZ());
			pfxSetZMax(aabb,aabbMaxL.getZ());

			PfxUInt32 flag = (node.left?0x40000000:0)|(node.right?0x80000000:0);
			PfxUInt32 leftId = node.left?node.left->nodeId:0;
			PfxUInt32 rightId = node.right?node.right->nodeId:0;
			PfxUInt32 packedId = (node.nodeId<<20)|(leftId<<10)|(rightId);

			SCE_PFX_ASSERT((leftId&0xfffffc00)==0);
			SCE_PFX_ASSERT((rightId&0xfffffc00)==0);
			SCE_PFX_ASSERT((node.nodeId&0xfffffc00)==0);

			aabb.set32(3,flag|packedId);
			
			lmesh.m_bvhNodes[i] = aabb;
		}

		SCE_PFX_UTIL_FREE(bvhNodes);
	}
	else {
		lmesh.m_aabbList = (PfxAabb16*)SCE_PFX_UTIL_ALLOC(128,sizeof(PfxAabb16)*islands.numIslands);
		
		if(!lmesh.m_aabbList) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		// AABB配列を作成する
		PfxUInt32 numIslands = islands.numIslands;
		for(PfxUInt32 i=0;i<numIslands;i++) {
			PfxVector3 aabbMin(SCE_PFX_FLT_MAX),aabbMax(-SCE_PFX_FLT_MAX);
			for(PfxUInt32 j=0;j<islands.facetsInIsland[i].size();j++) {
				aabbMin = minPerElem(islands.facetsInIsland[i][j]->aabbMin,aabbMin);
				aabbMax = maxPerElem(islands.facetsInIsland[i][j]->aabbMax,aabbMax);
			}
			
			PfxVecInt3 aabbMinL,aabbMaxL;
			lmesh.getLocalPosition(aabbMin,aabbMax,aabbMinL,aabbMaxL);
			
			pfxSetXMin(lmesh.m_aabbList[i],aabbMinL.getX());
			pfxSetXMax(lmesh.m_aabbList[i],aabbMaxL.getX());
			pfxSetYMin(lmesh.m_aabbList[i],aabbMinL.getY());
			pfxSetYMax(lmesh.m_aabbList[i],aabbMaxL.getY());
			pfxSetZMin(lmesh.m_aabbList[i],aabbMinL.getZ());
			pfxSetZMax(lmesh.m_aabbList[i],aabbMaxL.getZ());
		}
	}
	
	// ラージメッシュの作成
	if(islands.numIslands <= SCE_PFX_MAX_LARGETRIMESH_ISLANDS) {
		// ラージメッシュのタイプを判定する
		lmesh.m_type = 0;
		if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
			lmesh.m_type |= 0x01;
		}
		if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
			lmesh.m_type |= 0x02;
		}
		
		if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				lmesh.m_islands	= SCE_PFX_UTIL_ALLOC(128,sizeof(PfxQuantizedTriMeshBvh)*islands.numIslands);
				if(!lmesh.m_islands) return SCE_PFX_ERR_OUT_OF_BUFFER;
				memset(lmesh.m_islands,0,sizeof(PfxQuantizedTriMeshBvh)*islands.numIslands);
			}
			else {
				lmesh.m_islands	= SCE_PFX_UTIL_ALLOC(128,sizeof(PfxExpandedTriMeshBvh)*islands.numIslands);
				if(!lmesh.m_islands) return SCE_PFX_ERR_OUT_OF_BUFFER;
				memset(lmesh.m_islands,0,sizeof(PfxExpandedTriMeshBvh)*islands.numIslands);
			}
			lmesh.m_numIslands = islands.numIslands;
		}
		else {
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				lmesh.m_islands = SCE_PFX_UTIL_ALLOC(128,sizeof(PfxQuantizedTriMesh)*islands.numIslands);
				if(!lmesh.m_islands) return SCE_PFX_ERR_OUT_OF_BUFFER;
				memset(lmesh.m_islands,0,sizeof(PfxQuantizedTriMesh)*islands.numIslands);
			}
			else {
				lmesh.m_islands = SCE_PFX_UTIL_ALLOC(128,sizeof(PfxExpandedTriMesh)*islands.numIslands);
				if(!lmesh.m_islands) return SCE_PFX_ERR_OUT_OF_BUFFER;
				memset(lmesh.m_islands,0,sizeof(PfxExpandedTriMesh)*islands.numIslands);
			}
			lmesh.m_numIslands = islands.numIslands;
		}
		
		PfxUInt32 facetBuffBytes=0,edgeBuffBytes=0,vertexBuffBytes=0,bvhNodeBuffBytes=0;
		for(PfxUInt32 i=0;i<islands.numIslands;i++) {
			// 頂点、エッジ数をアイランドにセット
			PfxUInt32 numFacets = islands.facetsInIsland[i].size();
			PfxUInt32 numEdges=0,numVerts=0;
			PfxInt32 ret = countIsland(islands.facetsInIsland[i],numEdges,numVerts);
			if(ret != SCE_PFX_OK) return ret;
			
			if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
				if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
					PfxQuantizedTriMeshBvh *island = ((PfxQuantizedTriMeshBvh*)lmesh.m_islands)+i;
					facetBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantizedFacetBvh)*numFacets);
					bvhNodeBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxAabb16)*(numFacets*2-1));
					island->m_numFacets = numFacets;
					island->m_numEdges = numEdges;
					island->m_numVerts = numVerts;
				}
				else {
					PfxExpandedTriMeshBvh *island = ((PfxExpandedTriMeshBvh*)lmesh.m_islands)+i;
					facetBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxExpandedFacetBvh)*numFacets);
					bvhNodeBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxAabb16)*(numFacets*2-1));
					island->m_numFacets = numFacets;
					island->m_numEdges = numEdges;
					island->m_numVerts = numVerts;
				}
			}
			else {
				if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
					PfxQuantizedTriMesh *island = ((PfxQuantizedTriMesh*)lmesh.m_islands)+i;
					facetBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantizedFacet)*numFacets);
					island->m_numFacets = numFacets;
					island->m_numEdges = numEdges;
					island->m_numVerts = numVerts;
				}
				else {
					PfxExpandedTriMesh *island = ((PfxExpandedTriMesh*)lmesh.m_islands)+i;
					facetBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxExpandedFacet)*numFacets);
					island->m_numFacets = numFacets;
					island->m_numEdges = numEdges;
					island->m_numVerts = numVerts;
				}
			}
			
			edgeBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxEdge)*numEdges);
			
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				vertexBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantize3)*numVerts);
			}
			else {
				vertexBuffBytes += SCE_PFX_BYTES_ALIGN16(sizeof(PfxFloat3)*numVerts);
			}
		}
		
		// 共有バッファを確保
		lmesh.m_facetBuffer = SCE_PFX_UTIL_ALLOC(128,facetBuffBytes);
		lmesh.m_edgeBuffer = SCE_PFX_UTIL_ALLOC(128,edgeBuffBytes);
		lmesh.m_vertexBuffer = SCE_PFX_UTIL_ALLOC(128,vertexBuffBytes);
		lmesh.m_bvhNodeBuffer = SCE_PFX_UTIL_ALLOC(128,bvhNodeBuffBytes);
		lmesh.m_facetBuffBytes	= facetBuffBytes;
		lmesh.m_edgeBuffBytes	= edgeBuffBytes;
		lmesh.m_vertexBuffBytes	= vertexBuffBytes;
		lmesh.m_bvhNodeBuffBytes= bvhNodeBuffBytes;
		
		if(!(lmesh.m_facetBuffer&&lmesh.m_edgeBuffer&&lmesh.m_vertexBuffer&&lmesh.m_bvhNodeBuffer)) return SCE_PFX_ERR_OUT_OF_BUFFER;
		
		// 共有バッファを各アイランドに割り当てる
		if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
			PfxUInt8 *ptrFacetBuffer = (PfxUInt8*)lmesh.m_facetBuffer;
			PfxUInt8 *ptrEdgeBuffer = (PfxUInt8*)lmesh.m_edgeBuffer;
			PfxUInt8 *ptrVertexBuffer = (PfxUInt8*)lmesh.m_vertexBuffer;
			PfxUInt8 *ptrBvhNodeBuffer = (PfxUInt8*)lmesh.m_bvhNodeBuffer;
			
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxQuantizedTriMeshBvh *island = ((PfxQuantizedTriMeshBvh*)lmesh.m_islands)+i;
					island->m_facets = (PfxQuantizedFacetBvh*)ptrFacetBuffer;
					island->m_edges = (PfxEdge*)ptrEdgeBuffer;
					island->m_verts = (PfxQuantize3*)ptrVertexBuffer;
					island->m_bvhNodes = (PfxAabb16*)ptrBvhNodeBuffer;
					
					ptrFacetBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantizedFacetBvh)*island->m_numFacets);
					ptrEdgeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxEdge)*island->m_numEdges);
					ptrVertexBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantize3)*island->m_numVerts);
					ptrBvhNodeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxAabb16)*(island->m_numFacets*2-1));
					
					SCE_PFX_ASSERT(((uintptr_t)ptrFacetBuffer - (uintptr_t)lmesh.m_facetBuffer) <= facetBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrEdgeBuffer - (uintptr_t)lmesh.m_edgeBuffer) <= edgeBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrVertexBuffer - (uintptr_t)lmesh.m_vertexBuffer) <= vertexBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrBvhNodeBuffer - (uintptr_t)lmesh.m_bvhNodeBuffer) <= bvhNodeBuffBytes);
				}
			}
			else {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxExpandedTriMeshBvh *island = ((PfxExpandedTriMeshBvh*)lmesh.m_islands)+i;
					island->m_facets = (PfxExpandedFacetBvh*)ptrFacetBuffer;
					island->m_edges = (PfxEdge*)ptrEdgeBuffer;
					island->m_verts = (PfxFloat3*)ptrVertexBuffer;
					island->m_bvhNodes = (PfxAabb16*)ptrBvhNodeBuffer;
					
					ptrFacetBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxExpandedFacetBvh)*island->m_numFacets);
					ptrEdgeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxEdge)*island->m_numEdges);
					ptrVertexBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxFloat3)*island->m_numVerts);
					ptrBvhNodeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxAabb16)*(island->m_numFacets*2-1));
					
					SCE_PFX_ASSERT(((uintptr_t)ptrFacetBuffer - (uintptr_t)lmesh.m_facetBuffer) <= facetBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrEdgeBuffer - (uintptr_t)lmesh.m_edgeBuffer) <= edgeBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrVertexBuffer - (uintptr_t)lmesh.m_vertexBuffer) <= vertexBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrBvhNodeBuffer - (uintptr_t)lmesh.m_bvhNodeBuffer) <= bvhNodeBuffBytes);
				}
			}
		}
		else {
			PfxUInt8 *ptrFacetBuffer = (PfxUInt8*)lmesh.m_facetBuffer;
			PfxUInt8 *ptrEdgeBuffer = (PfxUInt8*)lmesh.m_edgeBuffer;
			PfxUInt8 *ptrVertexBuffer = (PfxUInt8*)lmesh.m_vertexBuffer;
			
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxQuantizedTriMesh *island = ((PfxQuantizedTriMesh*)lmesh.m_islands)+i;
					island->m_facets = (PfxQuantizedFacet*)ptrFacetBuffer;
					island->m_edges = (PfxEdge*)ptrEdgeBuffer;
					island->m_verts = (PfxQuantize3*)ptrVertexBuffer;
					
					ptrFacetBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantizedFacet)*island->m_numFacets);
					ptrEdgeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxEdge)*island->m_numEdges);
					ptrVertexBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxQuantize3)*island->m_numVerts);
					
					SCE_PFX_ASSERT(((uintptr_t)ptrFacetBuffer - (uintptr_t)lmesh.m_facetBuffer) <= facetBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrEdgeBuffer - (uintptr_t)lmesh.m_edgeBuffer) <= edgeBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrVertexBuffer - (uintptr_t)lmesh.m_vertexBuffer) <= vertexBuffBytes);
				}
			}
			else {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxExpandedTriMesh *island = ((PfxExpandedTriMesh*)lmesh.m_islands)+i;
					island->m_facets = (PfxExpandedFacet*)ptrFacetBuffer;
					island->m_edges = (PfxEdge*)ptrEdgeBuffer;
					island->m_verts = (PfxFloat3*)ptrVertexBuffer;
					
					ptrFacetBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxExpandedFacet)*island->m_numFacets);
					ptrEdgeBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxEdge)*island->m_numEdges);
					ptrVertexBuffer += SCE_PFX_BYTES_ALIGN16(sizeof(PfxFloat3)*island->m_numVerts);
					
					SCE_PFX_ASSERT(((uintptr_t)ptrFacetBuffer - (uintptr_t)lmesh.m_facetBuffer) <= facetBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrEdgeBuffer - (uintptr_t)lmesh.m_edgeBuffer) <= edgeBuffBytes);
					SCE_PFX_ASSERT(((uintptr_t)ptrVertexBuffer - (uintptr_t)lmesh.m_vertexBuffer) <= vertexBuffBytes);
				}
			}
		}
		
		// アイランドを作成
		PfxUInt32 maxFacets=0,maxVerts=0,maxEdges=0;
		
		if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxQuantizedTriMeshBvh *island = ((PfxQuantizedTriMeshBvh*)lmesh.m_islands)+i;
					PfxInt32 ret = submitIsland(param,*island,islands.facetsInIsland[i],lmesh);
					if(ret != SCE_PFX_OK) {
						pfxReleaseLargeTriMesh(lmesh);
						return ret;
					}
					
					maxFacets = SCE_PFX_MAX(maxFacets,island->m_numFacets);
					maxVerts = SCE_PFX_MAX(maxVerts,island->m_numVerts);
					maxEdges = SCE_PFX_MAX(maxEdges,island->m_numEdges);
				}
			}
			else {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxExpandedTriMeshBvh *island = ((PfxExpandedTriMeshBvh*)lmesh.m_islands)+i;
					PfxInt32 ret = submitIsland(param,*island,islands.facetsInIsland[i],lmesh);
					if(ret != SCE_PFX_OK) {
						pfxReleaseLargeTriMesh(lmesh);
						return ret;
					}
					
					maxFacets = SCE_PFX_MAX(maxFacets,island->m_numFacets);
					maxVerts = SCE_PFX_MAX(maxVerts,island->m_numVerts);
					maxEdges = SCE_PFX_MAX(maxEdges,island->m_numEdges);
				}
			}
		}
		else {
			if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxQuantizedTriMesh *island = ((PfxQuantizedTriMesh*)lmesh.m_islands)+i;
					PfxInt32 ret = submitIsland(param,*island,islands.facetsInIsland[i],lmesh);
					if(ret != SCE_PFX_OK) {
						pfxReleaseLargeTriMesh(lmesh);
						return ret;
					}
					
					maxFacets = SCE_PFX_MAX(maxFacets,island->m_numFacets);
					maxVerts = SCE_PFX_MAX(maxVerts,island->m_numVerts);
					maxEdges = SCE_PFX_MAX(maxEdges,island->m_numEdges);
				}
			}
			else {
				for(PfxUInt32 i=0;i<islands.numIslands;i++) {
					PfxExpandedTriMesh *island = ((PfxExpandedTriMesh*)lmesh.m_islands)+i;
					PfxInt32 ret = submitIsland(param,*island,islands.facetsInIsland[i],lmesh);
					if(ret != SCE_PFX_OK) {
						pfxReleaseLargeTriMesh(lmesh);
						return ret;
					}
					
					maxFacets = SCE_PFX_MAX(maxFacets,island->m_numFacets);
					maxVerts = SCE_PFX_MAX(maxVerts,island->m_numVerts);
					maxEdges = SCE_PFX_MAX(maxEdges,island->m_numEdges);
				}
			}
		}
		
		if(param.flag & SCE_PFX_MESH_FLAG_OUTPUT_INFO) {
			SCE_PFX_PRINTF("generate completed!\n\tinput mesh verts %u triangles %u\n\tislands %u max triangles %u verts %u edges %u\n",
				param.numVerts,param.numTriangles,(PfxUInt32)lmesh.m_numIslands,maxFacets,maxVerts,maxEdges);
			
			PfxUInt32 totalbytes = sizeof(PfxLargeTriMesh);
			SCE_PFX_PRINTF("large mesh        %u bytes\n",sizeof(PfxLargeTriMesh));

			if(param.flag & SCE_PFX_MESH_FLAG_USE_BVH) {
				PfxUInt32 bvhIslandBytes;
				if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
					bvhIslandBytes = sizeof(PfxQuantizedTriMeshBvh) * islands.numIslands;
				}
				else {
					bvhIslandBytes = sizeof(PfxExpandedTriMeshBvh) * islands.numIslands;
				}
				PfxUInt32 bvhTreeBytes = sizeof(PfxAabb16) * lmesh.m_numBvhNodes;
				totalbytes += bvhIslandBytes + bvhTreeBytes + facetBuffBytes + edgeBuffBytes + vertexBuffBytes + bvhNodeBuffBytes;
				SCE_PFX_PRINTF("bvh tree          %u bytes\n",bvhTreeBytes);
				SCE_PFX_PRINTF("bvh island buffer %u bytes\n",bvhIslandBytes);
				SCE_PFX_PRINTF("facet buffer      %u bytes\n",facetBuffBytes);
				SCE_PFX_PRINTF("edge buffer       %u bytes\n",edgeBuffBytes);
				SCE_PFX_PRINTF("vertex buffer     %u bytes\n",vertexBuffBytes);
				SCE_PFX_PRINTF("bvh node buffer   %u bytes\n",bvhNodeBuffBytes);
			}
			else {
				PfxUInt32 islandBytes;
				if(param.flag & SCE_PFX_MESH_FLAG_USE_QUANTIZED) {
					islandBytes = sizeof(PfxQuantizedTriMesh) * lmesh.m_numIslands;
				}
				else {
					islandBytes = sizeof(PfxExpandedTriMesh) * lmesh.m_numIslands;
				}
				PfxUInt32 aabbListBytes = sizeof(PfxAabb16) * lmesh.m_numIslands;
				totalbytes += aabbListBytes + islandBytes + facetBuffBytes + edgeBuffBytes + vertexBuffBytes;
				SCE_PFX_PRINTF("aabb buffer       %u bytes\n",aabbListBytes);
				SCE_PFX_PRINTF("island buffer     %u bytes\n",islandBytes);
				SCE_PFX_PRINTF("facet buffer      %u bytes\n",facetBuffBytes);
				SCE_PFX_PRINTF("edge buffer       %u bytes\n",edgeBuffBytes);
				SCE_PFX_PRINTF("vertex buffer     %u bytes\n",vertexBuffBytes);
			}
			SCE_PFX_PRINTF("----------------------------\n");
			SCE_PFX_PRINTF("total             %u bytes\n",totalbytes);
		}
	}
	else {
		pfxReleaseLargeTriMesh(lmesh);
		return SCE_PFX_ERR_OUT_OF_RANGE_ISLAND;
	}

	//SCE_PFX_ALWAYS_ASSERT( (param.flag & SCE_PFX_MESH_FLAG_USE_BVH) ? lmesh.m_bvhNodes&&lmesh.m_islands : lmesh.m_islands );

	return SCE_PFX_OK;
}

void pfxReleaseLargeTriMesh(PfxLargeTriMesh &lmesh)
{
	SCE_PFX_UTIL_FREE(lmesh.m_facetBuffer);
	SCE_PFX_UTIL_FREE(lmesh.m_edgeBuffer);
	SCE_PFX_UTIL_FREE(lmesh.m_vertexBuffer);
	SCE_PFX_UTIL_FREE(lmesh.m_bvhNodeBuffer);
	SCE_PFX_UTIL_FREE(lmesh.m_aabbList);
	SCE_PFX_UTIL_FREE(lmesh.m_islands);
	SCE_PFX_UTIL_FREE(lmesh.m_bvhNodes);

	lmesh.m_numIslands = 0;
	lmesh.m_numBvhNodes = 0;
}

} //namespace PhysicsEffects
} //namespace sce
