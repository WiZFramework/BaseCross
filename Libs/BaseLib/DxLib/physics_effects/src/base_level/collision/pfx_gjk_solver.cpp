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

#include "pfx_gjk_solver.h"
#include "pfx_intersect_common.h"

#include "../../../include/physics_effects/base_level/base/pfx_perf_counter.h"


namespace sce {
namespace PhysicsEffects {

///////////////////////////////////////////////////////////////////////////////
// GJK


template <class T, int maxData = 20>
struct PfxGjkStack {
	T data[maxData];
	bool isOk;
	SCE_PFX_PADDING(1,3)
	int cur;

	PfxGjkStack() : isOk(true),cur(0) {}

	void push(const T &d)
	{
		if(cur < maxData) {
			data[cur++] = d;
		}
		else {
			isOk = false;
		}
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
// Allocate Buffers

PfxGjkSolver::PfxGjkSolver() : shapeA(NULL),shapeB(NULL)
{
vertsP = g_vertsP;
vertsQ = g_vertsQ;
vertsW = g_vertsW;
facets = g_facets;
facetsHead = g_facetsHead;
edges = g_edges;
}

PfxGjkSolver::~PfxGjkSolver()
{
}

void PfxGjkSolver::setup(void *sA,void *sB,PfxGetSupportVertexFunc fA,PfxGetSupportVertexFunc fB)
{
	shapeA = sA;
	shapeB = sB;
	getSupportVertexShapeA = fA;
	getSupportVertexShapeB = fB;
}

PfxGjkSolver::Facet *PfxGjkSolver::addFacet(int v1,int v2,int v3)
{
	if(SCE_PFX_UNLIKELY(numFacets >= (int)MAX_FACETS))
		return NULL;

	Facet &facet = facets[numFacets];

	PfxVector3 V1 = vertsW[v1];
	PfxVector3 V2 = vertsW[v2];
	PfxVector3 V3 = vertsW[v3];
	facet.obsolete = 0;
	facet.v[0] = v1;
	facet.v[1] = v2;
	facet.v[2] = v3;

	PfxVector3 normal = cross(V3-V1,V2-V1);
	
	PfxFloat l = lengthSqr(normal);

	if(l < SCE_PFX_GJK_EPSILON * SCE_PFX_GJK_EPSILON) {
		return NULL;
	}

normal /= sqrtf(l);
	
	PfxFloat closestLength = dot(V1,normal);
	facet.distSqr = closestLength * closestLength;
	facet.closest = closestLength * normal;
	facet.normal = normal;

	facetsHead[numFacetsHead++] = &facet;
	numFacets++;

	return &facet;
}

///////////////////////////////////////////////////////////////////////////////
// Construct Silhouette

bool PfxGjkSolver::silhouette(Facet *facet,int i,PfxVector3 &w)
{
	PfxGjkStack<Edge> gs;

	gs.push(Edge(facet,i));

	do {
		Edge stk = gs.pop();
		Facet *ft = stk.f;

		if(ft->obsolete==0) {
			// wから見えるかどうかを判定
			if(dot(ft->normal,w-ft->closest) < 0.0f) {
				// 見えないのでエッジを登録
				if((unsigned int)numEdges >= MAX_EDGES) return false;
				edges[numEdges] = stk;
				numEdges++;
			}
			else {
				ft->obsolete = 1;
				gs.push(Edge(ft->adj[(stk.i+2)%3],ft->j[(stk.i+2)%3]));
				gs.push(Edge(ft->adj[(stk.i+1)%3],ft->j[(stk.i+1)%3]));
			}
		}
	} while(SCE_PFX_UNLIKELY(!gs.isEmpty()));

	return gs.isOk;
}

///////////////////////////////////////////////////////////////////////////////
// Detect Penetration Depth (EPA)

PfxInt32 PfxGjkSolver::detectPenetrationDepth(
	PfxFloat &distance,
	const PfxTransform3 &transformAB,const PfxTransform3 &transformBA,
	PfxVector3 &pA,PfxVector3 &pB,PfxVector3 &normal)
{
	SCE_PFX_PUSH_MARKER("EPA");

	PfxMatrix3 matrixAB,matrixBA;
	PfxVector3 offsetAB,offsetBA;
	
	// Bローカル->Aローカル変換
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	
	// Aローカル->Bローカル変換
	matrixBA = transformBA.getUpper3x3();
	offsetBA = transformBA.getTranslation();

	int epaIterationCount = 0;
	PfxFloat distance_ = SCE_PFX_FLT_MAX;

	numFacets = 0;
	numFacetsHead = 0;

	// 初期状態の判定
	SCE_PFX_PUSH_MARKER("EPA init");
	
	if(m_simplex.numVertices <= 1) {
		SCE_PFX_POP_MARKER();
		SCE_PFX_POP_MARKER();
		return kPfxGjkResultInvalid;
	}
	else if(m_simplex.numVertices == 2) {
		PfxVector3 v0 = m_simplex.W[0];
		PfxVector3 v1 = m_simplex.W[1];
		PfxVector3 dir = normalize(v1-v0);
		PfxMatrix3 rot = PfxMatrix3::rotation(2.0943951023932f,dir);//120 deg
		int axis;
		if(dir[0] < dir[1]) {
			if(dir[0] < dir[2]) {
				axis = 0;
			}
			else {
				axis = 2;
			}
		}
		else {
			if(dir[1] < dir[2]) {
				axis = 1;
			}
			else {
				axis = 2;
			}
		}
		PfxVector3 vec(0.0f);
		vec[axis] = 1.0f;

		PfxVector3 aux[3];
		aux[0] = cross(dir,vec);
		aux[1] = rot * aux[0];
		aux[2] = rot * aux[1];

		PfxVector3 p[3],q[3],w[3];

		for(int i=0;i<3;i++) {
			PfxVector3 pInA,qInB;
			getSupportVertexShapeA(shapeA,aux[i],pInA);
			getSupportVertexShapeB(shapeB,matrixBA * (-aux[i]),qInB);
			p[i] = pInA;
			q[i] = offsetAB + matrixAB * qInB;
			w[i] = p[i] - q[i];
			vertsP[i] = p[i];
			vertsQ[i] = q[i];
			vertsW[i] = w[i];
		}

		if(originInTetrahedron(w[0],w[1],w[2],v0)) {
			vertsP[3] = m_simplex.P[0];
			vertsQ[3] = m_simplex.Q[0];
			vertsW[3] = m_simplex.W[0];
			numVerts = 4;
		}
		else if(originInTetrahedron(w[0],w[1],w[2],v1)){
			vertsP[3] = m_simplex.P[1];
			vertsQ[3] = m_simplex.Q[1];
			vertsW[3] = m_simplex.W[1];
			numVerts = 4;
		}
		else {
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			return kPfxGjkResultInvalid;
		}
	}
	else if(m_simplex.numVertices == 3) {
		numVerts = 3;
		for(int i=0;i<numVerts;i++) {
			vertsP[i] = m_simplex.P[i];
			vertsQ[i] = m_simplex.Q[i];
			vertsW[i] = m_simplex.W[i];
		}

		PfxVector3 p[2],q[2],w[2];

		{
			PfxVector3 v = cross(vertsW[2]-vertsW[0],vertsW[1]-vertsW[0]);
			PfxVector3 pInA,qInB;
			getSupportVertexShapeA(shapeA,v,pInA);
			getSupportVertexShapeB(shapeB,matrixBA * (-v),qInB);
			p[0] = pInA;
			q[0] = offsetAB + matrixAB * qInB;
			w[0] = p[0] - q[0];
			getSupportVertexShapeA(shapeA,(-v),pInA);
			getSupportVertexShapeB(shapeB,matrixBA * v,qInB);
			p[1] = pInA;
			q[1] = offsetAB + matrixAB * qInB;
			w[1] = p[1] - q[1];
		}

		if(originInTetrahedron(vertsW[0],vertsW[1],vertsW[2],w[0])) {
			vertsP[3] = p[0];
			vertsQ[3] = q[0];
			vertsW[3] = w[0];
			numVerts = 4;
		}
		else if(originInTetrahedron(vertsW[0],vertsW[1],vertsW[2],w[1])){
			vertsP[3] = p[1];
			vertsQ[3] = q[1];
			vertsW[3] = w[1];
			numVerts = 4;
		}
		else {
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			return kPfxGjkResultInvalid;
		}
	}
	else {
		numVerts = 4;
		for(int i=0;i<numVerts;i++) {
			vertsP[i] = m_simplex.P[i];
			vertsQ[i] = m_simplex.Q[i];
			vertsW[i] = m_simplex.W[i];
		}
	}

	SCE_PFX_POP_MARKER();

	if(numVerts != 4) {
		return kPfxGjkResultInvalid;
	}

	// 原点が単体の内部にあるかどうかを判定
	if(SCE_PFX_UNLIKELY(!originInTetrahedron(vertsW[0],vertsW[1],vertsW[2],vertsW[3]))) {
		SCE_PFX_POP_MARKER();
		return kPfxGjkResultInvalid;
	}

	// 面の向きをチェック
	if(dot(-vertsW[0],cross(vertsW[2]-vertsW[0],vertsW[1]-vertsW[0])) > 0.0f) {
		PfxVector3 vertsP1,vertsQ1,vertsW1;
		PfxVector3 vertsP3,vertsQ3,vertsW3;
		vertsQ1=vertsQ[1];vertsW1=vertsW[1];vertsP1=vertsP[1];
		vertsQ3=vertsQ[3];vertsW3=vertsW[3];vertsP3=vertsP[3];
		vertsQ[1]=vertsQ3;vertsW[1]=vertsW3;vertsP[1]=vertsP3;
		vertsQ[3]=vertsQ1;vertsW[3]=vertsW1;vertsP[3]=vertsP1;
	}

	{
		Facet *f0 = addFacet(0,1,2);
		Facet *f1 = addFacet(0,3,1);
		Facet *f2 = addFacet(0,2,3);
		Facet *f3 = addFacet(1,3,2);

		if(SCE_PFX_UNLIKELY(!f0 || !f1 || !f2 || !f3)) {
			SCE_PFX_POP_MARKER();
			return kPfxGjkResultInvalid;
		}

		linkFacets(f0,0,f1,2);
		linkFacets(f0,1,f3,2);
		linkFacets(f0,2,f2,0);
		linkFacets(f1,0,f2,2);
		linkFacets(f1,1,f3,0);
		linkFacets(f2,1,f3,1);
	}
	
	// 探索
	Facet *facetMin = NULL;
	
	SCE_PFX_PUSH_MARKER("EPA loop");
	do {
		// 原点から一番近い点を算出し、そのベクトルと支点を返す
		SCE_PFX_PUSH_MARKER("find closest");
		int minFacetIdx = 0;
		{
			PfxFloat minDistSqr = facetsHead[0]->distSqr;
			facetMin = facetsHead[0];
			for(int i=1;i<numFacetsHead;i++) {
				if(facetsHead[i]->obsolete==0 && facetsHead[i]->distSqr < minDistSqr) {
					minDistSqr = facetsHead[i]->distSqr;
					facetMin = facetsHead[i];
					minFacetIdx = i;
				}
			}
		}
		
		// リストからはずす
		facetsHead[minFacetIdx] = facetsHead[--numFacetsHead];
		SCE_PFX_POP_MARKER();
		SCE_PFX_PUSH_MARKER("get support");
		PfxVector3 pInA(0.0f),qInB(0.0f);
		getSupportVertexShapeA(shapeA,facetMin->normal,pInA);
		getSupportVertexShapeB(shapeB,matrixBA * (-facetMin->normal),qInB);
		PfxVector3 p = pInA;
		PfxVector3 q = offsetAB + matrixAB * qInB;
		PfxVector3 w = p - q;
		PfxVector3 v = facetMin->closest;
		SCE_PFX_POP_MARKER();
		
		// 最近接点チェック
		PfxFloat l0 = length(v);
		PfxFloat l1 = dot(facetMin->normal,w);

		if((l1 - l0) < SCE_PFX_GJK_EPSILON) {
			break;
		}

		// 求めた点を追加して面を分割
		{
			if((PfxUInt32)numVerts >= MAX_VERTS) break;
			
			int vId = numVerts++;
			vertsP[vId] = p;
			vertsQ[vId] = q;
			vertsW[vId] = w;

			facetMin->obsolete = 1;

			numEdges = 0;

			SCE_PFX_PUSH_MARKER("silhouette");
			if(!silhouette(facetMin->adj[0],facetMin->j[0],w) || 
			   !silhouette(facetMin->adj[1],facetMin->j[1],w) ||
			   !silhouette(facetMin->adj[2],facetMin->j[2],w)) {
				SCE_PFX_POP_MARKER();
				break;
			}
			SCE_PFX_POP_MARKER();

			if(SCE_PFX_UNLIKELY(numEdges == 0)) break;

			SCE_PFX_PUSH_MARKER("link facets");
			bool edgeCheck = true;
			
			Facet *firstFacet,*lastFacet;
			{
				Edge &edge = edges[0];
				int v0 = edge.f->v[(edge.i+1)%3];
				int v1 = edge.f->v[edge.i];
				firstFacet = addFacet(v0,v1,vId);
				if(SCE_PFX_UNLIKELY(!firstFacet)) {
					SCE_PFX_POP_MARKER();
					edgeCheck = false;
					break;
				}
				linkFacets(edge.f,edge.i,firstFacet,0);
				lastFacet = firstFacet;
			}

			for(int e=1;e<numEdges;e++) {
				Edge &edge = edges[e];
				int v0 = edge.f->v[(edge.i+1)%3];
				int v1 = edge.f->v[edge.i];
				Facet *f = addFacet(v0,v1,vId);
				if(SCE_PFX_UNLIKELY(!f)) {
					edgeCheck=false;
					break;
				}
				linkFacets(edge.f,edge.i,f,0);
				linkFacets(f,2,lastFacet,1);
				lastFacet = f;
			}
			SCE_PFX_POP_MARKER();
			
			if(SCE_PFX_UNLIKELY(!edgeCheck)) {
				break;
			}

			linkFacets(lastFacet,1,firstFacet,2);
		}

		epaIterationCount++;
		if(SCE_PFX_UNLIKELY(epaIterationCount > SCE_PFX_EPA_ITERATION_MAX || numFacetsHead == 0)) {
			break;
		}
	} while(1);
	SCE_PFX_POP_MARKER();
	
	if(!facetMin) {
		return kPfxGjkResultInvalid;
	}
	
	// 衝突点計算
	int v1 = facetMin->v[0];
	int v2 = facetMin->v[1];
	int v3 = facetMin->v[2];

#if 0
	PfxMatrix3 mtx(vertsW[v1],vertsW[v2],vertsW[v3]);
	PfxFloat det = determinant(mtx);
	if(SCE_PFX_UNLIKELY(det * det < SCE_PFX_GJK_EPSILON * SCE_PFX_GJK_EPSILON)) {
		return kPfxGjkResultInvalid;
	}
	PfxVector3 uvw = inverse(mtx) * facetMin->closest;
	pA = PfxMatrix3(vertsP[v1],vertsP[v2],vertsP[v3]) * uvw;
	pB = PfxMatrix3(vertsQ[v1],vertsQ[v2],vertsQ[v3]) * uvw;
#else
	PfxVector3 tmp0 = vertsW[v2]-vertsW[v1];
	PfxVector3 tmp1 = vertsW[v3]-vertsW[v1];
	PfxVector3 tmp2 = facetMin->closest-vertsW[v1];
	PfxFloat d00 = dot(tmp0,tmp0);
	PfxFloat d01 = dot(tmp0,tmp1);
	PfxFloat d11 = dot(tmp1,tmp1);
	PfxFloat d20 = dot(tmp2,tmp0);
	PfxFloat d21 = dot(tmp2,tmp1);
	PfxFloat denom = d00 * d11 - d01 * d01;
	if(SCE_PFX_UNLIKELY(denom * denom < SCE_PFX_GJK_EPSILON * SCE_PFX_GJK_EPSILON)) {
		return kPfxGjkResultInvalid;
	}
	PfxFloat v = (d11 * d20 - d01 * d21) / denom;
	PfxFloat w = (d00 * d21 - d01 * d20) / denom;
	pA = vertsP[v1] + v * (vertsP[v2] - vertsP[v1]) + w * (vertsP[v3] - vertsP[v1]);
	pB = vertsQ[v1] + v * (vertsQ[v2] - vertsQ[v1]) + w * (vertsQ[v3] - vertsQ[v1]);
#endif

	PfxFloat lenSqr = lengthSqr(pB-pA);
	if(SCE_PFX_UNLIKELY(lenSqr < SCE_PFX_GJK_EPSILON * SCE_PFX_GJK_EPSILON)) {
		normal = PfxVector3(1.0f,0.0f,0.0f);
		distance = 0.0f;
	}
	else {
	PfxFloat len = sqrtf(lenSqr);
	normal = (pB-pA) / len;
	distance = -len;
	}

	SCE_PFX_POP_MARKER();
	
	return kPfxGjkResultOk;
}

///////////////////////////////////////////////////////////////////////////////
// Gjk

PfxInt32 PfxGjkSolver::collide(PfxFloat &distance, PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
						const PfxTransform3 & transformA,
						const PfxTransform3 & transformB,
						PfxFloat distanceThreshold)
{
	SCE_PFX_PUSH_MARKER("GJK");
	
	SCE_PFX_ASSERT(shapeA&&shapeB);
	
	(void) distanceThreshold;

	int gjkIterationCount = 0;

	m_simplex.reset();

	// Aローカル座標系に変換
	PfxTransform3 transformAB,transformBA;
	PfxMatrix3 matrixAB,matrixBA;
	PfxVector3 offsetAB,offsetBA;
	
	// Bローカル->Aローカル変換
	transformAB = orthoInverse(transformA) * transformB;
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	
	// Aローカル->Bローカル変換
	transformBA = orthoInverse(transformB) * transformA;
	matrixBA = transformBA.getUpper3x3();
	offsetBA = transformBA.getTranslation();

	PfxVector3 separatingAxis(-offsetAB);
	if(lengthSqr(separatingAxis) < 0.000001f) separatingAxis = PfxVector3(1.0f,0.0f,0.0f);
	PfxFloat squaredDistance = SCE_PFX_FLT_MAX;
	PfxFloat delta = 0.0f;
	PfxFloat distance_ = SCE_PFX_FLT_MAX;

	SCE_PFX_PUSH_MARKER("GJK loop");
	for(;;) {
		// サポート頂点の取得
		PfxVector3 pInA,qInB;

		getSupportVertexShapeA(shapeA,(-separatingAxis),pInA);
		getSupportVertexShapeB(shapeB,matrixBA * separatingAxis,qInB);

		PfxVector3 p = pInA;
		PfxVector3 q = offsetAB + matrixAB * qInB;
		PfxVector3 w = p - q;

		delta = dot(separatingAxis,w);

		// 早期終了チェック
		if(SCE_PFX_UNLIKELY(delta > 0.0f)) {
			normal = separatingAxis;
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			distance = distance_;
			return kPfxGjkResultOk;
		}
		
		// 単体に同一頂点が存在しないか？
		if(SCE_PFX_UNLIKELY(m_simplex.inSimplex(w))) {
			break;
		}

		PfxFloat f0 = squaredDistance - delta;
		PfxFloat f1 = squaredDistance * SCE_PFX_GJK_EPSILON;

		if (SCE_PFX_UNLIKELY(f0 <= f1)) {
			break;
		}

		// 頂点を単体に追加
		m_simplex.addVertex(w,p,q);
		
		// 原点と単体の最近接点を求め、分離軸を返す
		if(SCE_PFX_UNLIKELY(!m_simplex.closest(separatingAxis))) {
			// zero triangle error
			normal = separatingAxis;
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			distance = distance_;
			return kPfxGjkResultInvalid;
		}

		squaredDistance = lengthSqr(separatingAxis);

		if(SCE_PFX_UNLIKELY(gjkIterationCount >= SCE_PFX_GJK_ITERATION_MAX || m_simplex.fullSimplex())) {
			break;
		}

		gjkIterationCount++;
	}
	SCE_PFX_POP_MARKER();
	
	PfxVector3 pA(0.0f),pB(0.0f),nA(0.0f);

	// ２つのConvexは交差しているので、接触点を探索する (結果はAローカル系で返される)
	PfxInt32 ret = detectPenetrationDepth(distance_,transformAB,transformBA,pA,pB,nA);
	if(ret != kPfxGjkResultOk) {
		SCE_PFX_POP_MARKER();
		return ret;
	}
	
	distance = distance_;
	
	//マージン考慮
	if(distance_ < 0.0f) {
		// 全てAローカル座標系
		pA += nA * SCE_PFX_GJK_MARGIN;
		pB -= nA * SCE_PFX_GJK_MARGIN;
		distance = dot(nA,pA-pB);
		normal = nA;
		pointA = PfxPoint3(pA);
		pointB = PfxPoint3(pB);
	}
	
	SCE_PFX_POP_MARKER();
	
	return kPfxGjkResultOk;
}

PfxInt32 PfxGjkSolver::closest(PfxFloat &distance, PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
						const PfxTransform3 & transformA,
						const PfxTransform3 & transformB,
						PfxFloat distanceThreshold)
{
	(void) distanceThreshold;

	SCE_PFX_ASSERT(shapeA&&shapeB);
	
	int gjkIterationCount = 0;

	m_simplex.reset();

	// Aローカル座標系に変換
	PfxTransform3 transformAB,transformBA;
	PfxMatrix3 matrixAB,matrixBA;
	PfxVector3 offsetAB,offsetBA;
	
	// Bローカル->Aローカル変換
	transformAB = orthoInverse(transformA) * transformB;
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	
	// Aローカル->Bローカル変換
	transformBA = orthoInverse(transformB) * transformA;
	matrixBA = transformBA.getUpper3x3();
	offsetBA = transformBA.getTranslation();

	PfxVector3 separatingAxis(-offsetAB);
	if(lengthSqr(separatingAxis) < 0.000001f) separatingAxis = PfxVector3(1.0f,0.0f,0.0f);
	PfxFloat squaredDistance = SCE_PFX_FLT_MAX;
	PfxFloat delta = 0.0f;
	PfxFloat distance_ = SCE_PFX_FLT_MAX;

	for(;;) {
		// サポート頂点の取得
		PfxVector3 pInA,qInB;

		getSupportVertexShapeA(shapeA,(-separatingAxis),pInA);
		getSupportVertexShapeB(shapeB,matrixBA * separatingAxis,qInB);

		PfxVector3 p = pInA;
		PfxVector3 q = offsetAB + matrixAB * qInB;
		PfxVector3 w = p - q;

		delta = dot(separatingAxis,w);
		
		// 既に単体に同一頂点が存在している
		if(SCE_PFX_UNLIKELY(m_simplex.inSimplex(w))) {
			break;
		}

		PfxFloat f0 = squaredDistance - delta;
		PfxFloat f1 = squaredDistance * SCE_PFX_GJK_EPSILON;

		if (SCE_PFX_UNLIKELY(f0 <= f1)) {
			break;
		}
		
		// 頂点を単体に追加
		m_simplex.addVertex(w,p,q);
		
		// 原点と単体の最近接点を求め、分離軸を返す
		if(SCE_PFX_UNLIKELY(!m_simplex.closest(separatingAxis))) {
			// zero triangle error
			return kPfxGjkResultInvalid;
		}
		
		// フルシンプレックスのとき（原点は内部にあり）交差している
		if(m_simplex.fullSimplex()) {
			return kPfxGjkResultIntersect;
		}
		
		squaredDistance = lengthSqr(separatingAxis);
		
		// 反復回数が制限に達した。この時点での値を近似値として返す
		if(SCE_PFX_UNLIKELY(gjkIterationCount >= SCE_PFX_GJK_ITERATION_MAX)) {
			break;
		}
		
		gjkIterationCount++;
	}
	
	PfxVector3 pA(0.0f),pB(0.0f),nA(0.0f);
	
	m_simplex.getClosestPoints(pA,pB);
	
	if(squaredDistance < SCE_PFX_GJK_EPSILON * SCE_PFX_GJK_EPSILON) {
		nA = PfxVector3(1.0f,0.0f,0.0f);
	}
	else {
		nA = normalize(separatingAxis);
	}
	
	// 全てAローカル座標系
	distance = dot(nA,pA-pB);
	normal = nA;
	pointA = PfxPoint3(pA);
	pointB = PfxPoint3(pB);
	
	return kPfxGjkResultOk;
}

///////////////////////////////////////////////////////////////////////////////
// Minkowski Portal Refinement Algorithm

#define MPR_EPSILON 0.00001f
#define MPR_MAX_ITERATION 32

void PfxMprSolver:: setup(void *sA,void *sB,PfxGetSupportVertexFunc fsA,PfxGetSupportVertexFunc fsB,PfxGetCenterFunc fcA,PfxGetCenterFunc fcB)
{
	shapeA = sA;
	shapeB = sB;
	getSupportVertexShapeA = fsA;
	getSupportVertexShapeB = fsB;
	getCenterA = fcA;
	getCenterB = fcB;
}

PfxFloat PfxMprSolver::collide( PfxVector3& normal, PfxPoint3 &pointA, PfxPoint3 &pointB,
						const PfxTransform3 & transformA,
						const PfxTransform3 & transformB,
						PfxFloat distanceThreshold)
{
	SCE_PFX_PUSH_MARKER("MPR");
	
	SCE_PFX_PUSH_MARKER("setup");

	(void) distanceThreshold; 
	
	PfxTransform3 transformAB, transformBA;
	PfxMatrix3 matrixAB, matrixBA;
	PfxVector3 offsetAB, offsetBA;

	// Bローカル→Aローカルへの変換
	transformAB = orthoInverse(transformA) * transformB;
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();

	// Aローカル→Bローカルへの変換
	transformBA = orthoInverse(transformB) * transformA;
	matrixBA = transformBA.getUpper3x3();
	offsetBA = transformBA.getTranslation();
	
	// ポータル原点を作成
	PfxVector3 portalOrigin(0.0f);
	{
		PfxVector3 centerA(0.0f);
		PfxVector3 centerB(0.0f);
		
		getCenterA(shapeA,centerA);
		getCenterB(shapeB,centerB);
		
		portalOrigin = 0.5f * (centerA - (offsetAB + matrixAB * centerB));
	}
	SCE_PFX_POP_MARKER();

	SCE_PFX_PUSH_MARKER("create portal");
	PfxVector3 portal1,portal2,portal3;
	PfxVector3 P1,P2,P3,Q1,Q2,Q3;
	PfxVector3 s,pInA,qInB,p,q; // Temporal for support function

	//J 初期ポータルの作成
	//E Create the initial portal
	{
		s = -portalOrigin;
		getSupportVertexShapeA(shapeA,s,pInA);
		getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
		p = pInA;
		q = offsetAB + matrixAB * qInB;
		P1 = pInA;
		Q1 = qInB;
		portal1 = p - q;

		s = cross(portal1,portalOrigin);
		getSupportVertexShapeA(shapeA,s,pInA);
		getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
		p = pInA;
		q = offsetAB + matrixAB * qInB;
		P2 = pInA;
		Q2 = qInB;
		portal2 = p - q;

		s = cross(portal1-portalOrigin,portal2-portalOrigin);
		getSupportVertexShapeA(shapeA,s,pInA);
		getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
		p = pInA;
		q = offsetAB + matrixAB * qInB;
		P3 = pInA;
		Q3 = qInB;
		portal3 = p - q;

		//J 原点がポータルの内側にあるかどうかをチェック。なければ再度ポータルを作り直す
		//E Check if the origin is inside of the portal. If the origin is outside of the portal, create new portal again.
		bool originIsInsidePortal = false;
		int create_count = 0; // Safety net
		do {
			SCE_PFX_PUSH_MARKER("loop");
			PfxVector3 a = portal1-portalOrigin;
			PfxVector3 b = portal2-portalOrigin;
			PfxVector3 c = portal3-portalOrigin;
			PfxVector3 n1 = cross(a,c);
			PfxVector3 n2 = cross(b,a);
			PfxVector3 n3 = cross(c,b);
			PfxFloat distFacet1 = dot(n1,-portalOrigin);
			PfxFloat distFacet2 = dot(n2,-portalOrigin);
			PfxFloat distFacet3 = dot(n3,-portalOrigin);
			
			if( distFacet1 > 0.0f ) {
				SCE_PFX_SWAP(PfxVector3,P1,P3);
				SCE_PFX_SWAP(PfxVector3,Q1,Q3);
				SCE_PFX_SWAP(PfxVector3,portal1,portal3);

				s = n1;
				getSupportVertexShapeA(shapeA,s,pInA);
				getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
				p = pInA;
				q = offsetAB + matrixAB * qInB;
				P2 = pInA;
				Q2 = qInB;
				portal2 = p - q;
			}
			else if( distFacet2 > 0.0f ) {
				SCE_PFX_SWAP(PfxVector3,P1,P2);
				SCE_PFX_SWAP(PfxVector3,Q1,Q2);
				SCE_PFX_SWAP(PfxVector3,portal1,portal2);

				s = n2;
				getSupportVertexShapeA(shapeA,s,pInA);
				getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
				p = pInA;
				q = offsetAB + matrixAB * qInB;
				P3 = pInA;
				Q3 = qInB;
				portal3 = p - q;
			}
			else if( distFacet3 > 0.0f ) {
				SCE_PFX_SWAP(PfxVector3,P2,P3);
				SCE_PFX_SWAP(PfxVector3,Q2,Q3);
				SCE_PFX_SWAP(PfxVector3,portal2,portal3);

				s = n3;
				getSupportVertexShapeA(shapeA,s,pInA);
				getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
				p = pInA;
				q = offsetAB + matrixAB * qInB;
				P1 = pInA;
				Q1 = qInB;
				portal1 = p - q;
			}
			else {
				originIsInsidePortal = true;
			}

			// Safety net
			if((create_count++)>32) {
				SCE_PFX_POP_MARKER();
				SCE_PFX_POP_MARKER();
				SCE_PFX_POP_MARKER();
				return SCE_PFX_FLT_MAX;
			}
		SCE_PFX_POP_MARKER();
		} while(!originIsInsidePortal);
	}
	SCE_PFX_POP_MARKER();
	
	SCE_PFX_PUSH_MARKER("refine portal");
	int mpr_count = 0;
	do {
		SCE_PFX_PUSH_MARKER("loop");

		//J サーフェスに近い新たな点を求める
		//E Find new point close to the surface.
		s = cross(portal2-portal1,portal3-portal1);
		getSupportVertexShapeA(shapeA,s,pInA);
		getSupportVertexShapeB(shapeB,matrixBA * (-s),qInB);
		p = pInA;
		q = offsetAB + matrixAB * qInB;
		PfxVector3 newPortal = p - q;

		//J 原点とサポート平面の位置関係をチェック
		//E Check if the origin is outside of the support plane.
		if(dot(s,-newPortal) > 0.0f) {
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			SCE_PFX_POP_MARKER();
			return SCE_PFX_FLT_MAX;
		}

		//J ポータルが充分サーフェスに近づいたかどうかを判定
		//E Check distance between the portal and the surface.
		if(dot(s,newPortal-portal1) < MPR_EPSILON) {
			SCE_PFX_POP_MARKER();
			break;
		}

		//J カウントチェック
		//E Check counts
		if((mpr_count++) > MPR_MAX_ITERATION) {
			SCE_PFX_POP_MARKER();
			break;
		}

		//J ポータルのリファイン
		//E Portal refinement
		#if 0
		PfxVector3 n1 = cross(newPortal-portalOrigin,portal1-portalOrigin);
		PfxVector3 n2 = cross(newPortal-portalOrigin,portal2-portalOrigin);
		PfxVector3 n3 = cross(newPortal-portalOrigin,portal3-portalOrigin);
		PfxFloat d1 = dot(n1,-portalOrigin);
		PfxFloat d2 = dot(n2,-portalOrigin);
		PfxFloat d3 = dot(n3,-portalOrigin);
		#else
		PfxVector3 tmpvec = cross(newPortal,portalOrigin);
		PfxFloat d1 = dot(portal1-portalOrigin,tmpvec);
		PfxFloat d2 = dot(portal2-portalOrigin,tmpvec);
		PfxFloat d3 = dot(portal3-portalOrigin,tmpvec);
		#endif
		
		if(d2>0.0f&&d3<=0.0f) {
			P1 = pInA;
			Q1 = qInB;
			portal1 = newPortal;
		}
		else if(d1>0.0f&&d2<=0.0f) {
			P3 = pInA;
			Q3 = qInB;
			portal3 = newPortal;
		}
		else if(d3>0.0f&&d1<=0.0f) {
			P2 = pInA;
			Q2 = qInB;
			portal2 = newPortal;
		}
		else {
			//J ここには来ないはず
			//E Don't come here.
		}
		SCE_PFX_POP_MARKER();
	} while(1);
	SCE_PFX_POP_MARKER();

	SCE_PFX_PUSH_MARKER("find contact point");

	//J ポータルと原点の最近接点が衝突点になる
	//E Calculate the closest point as the contact point between the portal and the origin.

	PfxMatrix3 mtx(portal1,portal2,portal3);
	PfxFloat det = determinant(mtx);
	if(SCE_PFX_UNLIKELY(det * det < MPR_EPSILON * MPR_EPSILON)) {
		return SCE_PFX_FLT_MAX;
	}
	PfxVector3 dir = normalize(s);
	PfxVector3 closest = dot(portal1,dir)*dir;
	PfxVector3 uvw = inverse(mtx) * closest;
	PfxVector3 pA = PfxMatrix3(P1,P2,P3) * uvw;
	PfxVector3 pB = PfxMatrix3(Q1,Q2,Q3) * uvw;

	pA -= dir * SCE_PFX_GJK_MARGIN;
	pB += dir * SCE_PFX_GJK_MARGIN;

	PfxVector3 wA = transformA.getTranslation() + transformA.getUpper3x3() * pA;
	PfxVector3 wB = transformB.getTranslation() + transformB.getUpper3x3() * pB;
	
	PfxVector3 wN = -dir;
	PfxFloat dist = dot(wN,wA-wB);
	pointA = PfxPoint3(pA);
	pointB = PfxPoint3(pB);
	normal = wN;

	SCE_PFX_POP_MARKER();
	SCE_PFX_POP_MARKER();

	return dist;
}

} //namespace PhysicsEffects
} //namespace sce
