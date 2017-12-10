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
#include "../../../include/physics_effects/base_level/base/pfx_vec_utils.h"
#include "pfx_simplex_solver.h"

namespace sce {
namespace PhysicsEffects {

#define SCE_PFX_GJK_EPSILON			1e-04f
#define SCE_PFX_GJK_MARGIN			0.025f
#define SCE_PFX_GJK_ITERATION_MAX	10
#define SCE_PFX_EPA_ITERATION_MAX	10

///////////////////////////////////////////////////////////////////////////////
// Support Function

typedef void (*PfxGetSupportVertexFunc)(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
typedef void (*PfxGetCenterFunc)(void *shape,PfxVector3 &center);

///////////////////////////////////////////////////////////////////////////////
// Gjk

enum ePfxGjkResult {
	kPfxGjkResultOk = 0,
	kPfxGjkResultIntersect,
	kPfxGjkResultInvalid,
};

class PfxSimplexSolver;

class PfxGjkSolver
{
private:
	static const PfxUInt32 MAX_VERTS = 128;
	static const PfxUInt32 MAX_EDGES = 128;
	static const PfxUInt32 MAX_FACETS = 64;
	
	PfxSimplexSolver m_simplex;

	// 面
	struct Facet {
		PfxVector3 normal;		// 面の法線
		PfxVector3 closest;		// 原点からの最近接点
		PfxUInt32 obsolete;		// 廃棄
		PfxFloat distSqr;		// 最近接距離の二乗
		PfxInt32 j[3];			// 隣接面から見たIndex
		Facet *adj[3];			// 隣接面
		PfxInt8	v[3];			// 面を構成する頂点
		SCE_PFX_PADDING(1,13)
	};
	
	// エッジ
	struct Edge {
		Facet *f;
		PfxInt32 i;
		Edge() {}
		Edge(Facet *f_,PfxInt32 i_)
		{
			f = f_;
			i= i_;
		}
	};
	
	PfxVector3 g_vertsP[MAX_VERTS];
	PfxVector3 g_vertsQ[MAX_VERTS];
	PfxVector3 g_vertsW[MAX_VERTS];
	Facet g_facets[MAX_FACETS];
	Facet *g_facetsHead[MAX_FACETS];
	Edge  g_edges[MAX_EDGES];

	PfxVector3 *vertsP;
	PfxVector3 *vertsQ;
	PfxVector3 *vertsW;
	Facet *facets;
	Facet **facetsHead;
	Edge  *edges;
	
	int numVerts;
	int numEdges;
	int numFacets;
	int numFacetsHead;
	
	void *shapeA;
	void *shapeB;
	PfxGetSupportVertexFunc getSupportVertexShapeA;
	PfxGetSupportVertexFunc getSupportVertexShapeB;

	SCE_PFX_PADDING(1,8)

	Facet *addFacet(int v1,int v2,int v3);

	inline void linkFacets(Facet *f1,int e1,Facet *f2,int e2);
	bool silhouette(Facet *facet,int i,PfxVector3 &w);

	inline bool originInTetrahedron(const PfxVector3& p0,const PfxVector3& p1,const PfxVector3& p2,const PfxVector3& p3);

	PfxInt32 detectPenetrationDepth(
		PfxFloat &distance,
		const PfxTransform3 &transformAB,const PfxTransform3 &transformBA,
		PfxVector3 &pA,PfxVector3 &pB,PfxVector3 &normal);

public:
	PfxGjkSolver();
	~PfxGjkSolver();
	
	void setup(void *sA,void *sB,PfxGetSupportVertexFunc fA,PfxGetSupportVertexFunc fB);
	
	PfxInt32 collide(PfxFloat &distance, PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
					const PfxTransform3 & transformA,
					const PfxTransform3 & transformB,
					PfxFloat distanceThreshold = SCE_PFX_FLT_MAX);

	PfxInt32 closest(PfxFloat &distance, PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
					const PfxTransform3 & transformA,
					const PfxTransform3 & transformB,
					PfxFloat distanceThreshold = SCE_PFX_FLT_MAX);
};

inline
void PfxGjkSolver::linkFacets(Facet *f1,int e1,Facet *f2,int e2)
{
	f1->adj[e1] = f2;
	f2->adj[e2] = f1;
	f1->j[e1] = e2;
	f2->j[e2] = e1;
}

inline
bool PfxGjkSolver::originInTetrahedron(const PfxVector3& p0,const PfxVector3& p1,const PfxVector3& p2,const PfxVector3& p3)
{
	PfxVector3 n0 = cross((p1-p0),(p2-p0));
	PfxVector3 n1 = cross((p2-p1),(p3-p1));
	PfxVector3 n2 = cross((p3-p2),(p0-p2));
	PfxVector3 n3 = cross((p0-p3),(p1-p3));

	return 
		dot(n0,p0) * dot(n0,p3-p0) <= 0.0f &&
		dot(n1,p1) * dot(n1,p0-p1) <= 0.0f &&
		dot(n2,p2) * dot(n2,p1-p2) <= 0.0f &&
		dot(n3,p3) * dot(n3,p2-p3) <= 0.0f;
}

///////////////////////////////////////////////////////////////////////////////
// Minkowski Protal Refinement Algorithm

class PfxMprSolver
{
private:
	void *shapeA;
	void *shapeB;
	PfxGetSupportVertexFunc getSupportVertexShapeA;
	PfxGetSupportVertexFunc getSupportVertexShapeB;
	PfxGetCenterFunc getCenterA;
	PfxGetCenterFunc getCenterB;

public:
	void setup(void *sA,void *sB,PfxGetSupportVertexFunc fsA,PfxGetSupportVertexFunc fsB,PfxGetCenterFunc fcA,PfxGetCenterFunc fcB);
	
	PfxFloat collide( PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
					const PfxTransform3 & transformA,
					const PfxTransform3 & transformB,
					PfxFloat distanceThreshold = SCE_PFX_FLT_MAX);
};

} //namespace PhysicsEffects
} //namespace sce

