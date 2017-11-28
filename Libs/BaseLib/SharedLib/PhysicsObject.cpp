/*!
@file PhysicsObject.cpp
@brief 物理計算オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	class PsAllocator {
	public:
		virtual void *allocate(size_t bytes) = 0;
		virtual void deallocate(void *p) = 0;
	};

	class DefaultAllocator : public PsAllocator
	{
	public:
		void *allocate(size_t bytes)
		{
			return malloc(bytes);
		}

		void deallocate(void *p)
		{
			free(p);
		}
	} allocator;

	typedef bool (*PsBroadPhaseCallback)(uint32_t rigidBodyIdA, uint32_t rigidBodyIdB, void *userData);

#define Key(a) ((a).key)
	template <class SortData>
	void PsMergeTwoBuffers(SortData* d1, unsigned int n1, SortData* d2, unsigned int n2, SortData *buff)
	{
		unsigned int i = 0, j = 0;

		while (i<n1&&j<n2) {
			if (Key(d1[i]) < Key(d2[j])) {
				buff[i + j] = d1[i++];
			}
			else {
				buff[i + j] = d2[j++];
			}
		}

		if (i<n1) {
			while (i<n1) {
				buff[i + j] = d1[i++];
			}
		}
		else if (j<n2) {
			while (j<n2) {
				buff[i + j] = d2[j++];
			}
		}

		for (unsigned int k = 0; k<(n1 + n2); k++) {
			d1[k] = buff[k];
		}
	}

	template <class SortData>
	void PsSort(SortData *d, SortData *buff, int n)
	{
		int n1 = n >> 1;
		int n2 = n - n1;
		if (n1>1) PsSort(d, buff, n1);
		if (n2>1) PsSort(d + n1, buff, n2);
		PsMergeTwoBuffers(d, n1, d + n1, n2, buff);
	}

	inline
	bsm::Mat3x3 PsCalcInertiaBox(const bsm::Vec3 &halfExtent, float mass)
	{
		bsm::Vec3 sqrSz = halfExtent * 2.0f;
		sqrSz = bsm::mulPerElem(sqrSz, sqrSz);
		bsm::Mat3x3 inertia;
		inertia.identity();
		inertia[0][0] = (mass*(sqrSz[1] + sqrSz[2])) / 12.0f;
		inertia[1][1] = (mass*(sqrSz[0] + sqrSz[2])) / 12.0f;
		inertia[2][2] = (mass*(sqrSz[0] + sqrSz[1])) / 12.0f;
		return inertia;
	}

	bool PsCreateConvexMesh(PsConvexMesh *convexMesh,
		const float *vertices, uint32_t numVertices,
		const uint16_t *indices, uint32_t numIndices,
		const bsm::Vec3 &scale)
	{
		assert(convexMesh);
		assert(vertices);
		assert(indices);
		assert(dot(scale, scale)>0.0f);

		if (numVertices > PS_CONVEX_MESH_MAX_VERTICES || numIndices > PS_CONVEX_MESH_MAX_FACETS * 3) {
			return false;
		}

		memset(convexMesh, 0, sizeof(PsConvexMesh));

		for (uint32_t i = 0; i<numVertices; i++) {
			convexMesh->m_vertices[i][0] = vertices[i * 3];
			convexMesh->m_vertices[i][1] = vertices[i * 3 + 1];
			convexMesh->m_vertices[i][2] = vertices[i * 3 + 2];
			convexMesh->m_vertices[i] = bsm::mulPerElem(scale, convexMesh->m_vertices[i]);
		}
		convexMesh->m_numVertices = numVertices;

		uint32_t nf = 0;
		for (uint32_t i = 0; i<numIndices / 3; i++) {
			bsm::Vec3 p[3] = {
				convexMesh->m_vertices[indices[i * 3]],
				convexMesh->m_vertices[indices[i * 3 + 1]],
				convexMesh->m_vertices[indices[i * 3 + 2]]
			};

			bsm::Vec3 normal = bsm::cross(p[1] - p[0], p[2] - p[0]);
			float areaSqr = bsm::lengthSqr(normal);
			if (areaSqr > PS_EPSILON * PS_EPSILON) {
				convexMesh->m_facets[nf].vertId[0] = (uint8_t)indices[i * 3];
				convexMesh->m_facets[nf].vertId[1] = (uint8_t)indices[i * 3 + 1];
				convexMesh->m_facets[nf].vertId[2] = (uint8_t)indices[i * 3 + 2];
				convexMesh->m_facets[nf].normal = normal / sqrtf(areaSqr);
				nf++;
			}
		}
		convexMesh->m_numFacets = nf;

		uint8_t edgeIdTable[PS_CONVEX_MESH_MAX_VERTICES*PS_CONVEX_MESH_MAX_VERTICES / 2];
		memset(edgeIdTable, 0xff, sizeof(edgeIdTable));

		uint32_t ne = 0;
		for (uint32_t i = 0; i<convexMesh->m_numFacets; i++) {
			PsFacet &facet = convexMesh->m_facets[i];
			for (uint32_t e = 0; e<3; e++) {
				uint32_t vertId0 = PS_MIN(facet.vertId[e % 3], facet.vertId[(e + 1) % 3]);
				uint32_t vertId1 = PS_MAX(facet.vertId[e % 3], facet.vertId[(e + 1) % 3]);
				uint32_t tableId = vertId1*(vertId1 - 1) / 2 + vertId0;
				if (edgeIdTable[tableId] == 0xff) {
					convexMesh->m_edges[ne].facetId[0] = i;
					convexMesh->m_edges[ne].facetId[1] = i;
					convexMesh->m_edges[ne].vertId[0] = (uint8_t)vertId0;
					convexMesh->m_edges[ne].vertId[1] = (uint8_t)vertId1;
					convexMesh->m_edges[ne].type = PsEdgeType::EdgeTypeConvex; // 凸エッジで初期化
					facet.edgeId[e] = ne;
					edgeIdTable[tableId] = ne;
					ne++;
					if (ne > PS_CONVEX_MESH_MAX_EDGES) {
						return false;
					}
				}
				else {
					assert(edgeIdTable[tableId] < PS_CONVEX_MESH_MAX_EDGES);
					PsEdge &edge = convexMesh->m_edges[edgeIdTable[tableId]];
					PsFacet &facetB = convexMesh->m_facets[edge.facetId[0]];

					assert(edge.facetId[0] == edge.facetId[1]);

					bsm::Vec3 s = convexMesh->m_vertices[facet.vertId[(e + 2) % 3]];
					bsm::Vec3 q = convexMesh->m_vertices[facetB.vertId[0]];
					float d = dot(s - q, facetB.normal);

					if (d < -PS_EPSILON) {
						edge.type = PsEdgeType::EdgeTypeConvex;
					}
					else if (d > PS_EPSILON) {
						// 本来ここに来てはいけない
						edge.type = PsEdgeType::EdgeTypeConcave;
					}
					else {
						edge.type = PsEdgeType::EdgeTypeFlat;
					}
					edge.facetId[1] = i;
				}
			}
		}
		convexMesh->m_numEdges = ne;

		return true;
	}

	void PsApplyExternalForce(
		PsState &state,
		const PsRigidBody &body,
		const bsm::Vec3 &externalForce,
		const bsm::Vec3 &externalTorque,
		float timeStep)
	{
		if (state.m_motionType == PsMotionType::MotionTypeStatic) {
			return;
		}

		bsm::Mat3x3 orientation(state.m_orientation);
		bsm::Mat3x3 worldInertia = transpose(orientation) * (body.m_inertia * orientation);
		bsm::Mat3x3 worldInertiaInv = transpose(orientation) * (inverse(body.m_inertia) * orientation);
		bsm::Vec3 angularMomentum = worldInertia * state.m_angularVelocity;

		state.m_linearVelocity += externalForce / body.m_mass * timeStep;
		angularMomentum += externalTorque * timeStep;
		state.m_angularVelocity = worldInertiaInv * angularMomentum;

		float linVelSqr = lengthSqr(state.m_linearVelocity);
		if (linVelSqr > (PS_MAX_LINEAR_VELOCITY*PS_MAX_LINEAR_VELOCITY)) {
			state.m_linearVelocity = (state.m_linearVelocity / sqrtf(linVelSqr)) * PS_MAX_LINEAR_VELOCITY;
		}

		float angVelSqr = lengthSqr(state.m_angularVelocity);
		if (angVelSqr > (PS_MAX_ANGULAR_VELOCITY*PS_MAX_ANGULAR_VELOCITY)) {
			state.m_angularVelocity = (state.m_angularVelocity / sqrtf(angVelSqr)) * PS_MAX_ANGULAR_VELOCITY;
		}
	}

	inline
	bool epxIntersectAABB(const bsm::Vec3 &centerA, const bsm::Vec3 &halfA, const bsm::Vec3 &centerB, const bsm::Vec3 &halfB)
	{
		if (fabs(centerA[0] - centerB[0]) > halfA[0] + halfB[0]) return false;
		if (fabs(centerA[1] - centerB[1]) > halfA[1] + halfB[1]) return false;
		if (fabs(centerA[2] - centerB[2]) > halfA[2] + halfB[2]) return false;
		return true;
	}

	void PsBroadPhase(
		const PsState *states,
		const PsCollidable *collidables,
		uint32_t numRigidBodies,
		const PsPair *oldPairs,
		const uint32_t numOldPairs,
		PsPair *newPairs,
		uint32_t &numNewPairs,
		const uint32_t maxPairs,
		PsAllocator *allocator,
		void *userData,
		PsBroadPhaseCallback callback)
	{
		assert(states);
		assert(collidables);
		assert(oldPairs);
		assert(newPairs);
		assert(allocator);

		numNewPairs = 0;

		for (uint32_t i = 0; i<numRigidBodies; i++) {
			for (uint32_t j = i + 1; j<numRigidBodies; j++) {
				const PsState &stateA = states[i];
				const PsCollidable &collidableA = collidables[i];
				const PsState &stateB = states[j];
				const PsCollidable &collidableB = collidables[j];

				if (callback && !callback(i, j, userData)) {
					continue;
				}

				bsm::Mat3x3 orientationA(stateA.m_orientation);
				bsm::Vec3 centerA = stateA.m_position + orientationA * collidableA.m_center;
				bsm::Vec3 halfA = absPerElem(orientationA) * (collidableA.m_half + bsm::Vec3(PS_AABB_EXPAND));// AABBサイズを若干拡張

				bsm::Mat3x3 orientationB(stateB.m_orientation);
				bsm::Vec3 centerB = stateB.m_position + orientationB * collidableB.m_center;
				bsm::Vec3 halfB = absPerElem(orientationB) * (collidableB.m_half + bsm::Vec3(PS_AABB_EXPAND));// AABBサイズを若干拡張

				if (epxIntersectAABB(centerA, halfA, centerB, halfB) && numNewPairs < maxPairs) {
					PsPair &newPair = newPairs[numNewPairs++];

					newPair.rigidBodyA = i<j ? i : j;
					newPair.rigidBodyB = i<j ? j : i;
					newPair.contact = NULL;
				}
			}
		}

		{
			PsPair *sortBuff = (PsPair*)allocator->allocate(sizeof(PsPair)*numNewPairs);
			PsSort<PsPair>(newPairs, sortBuff, numNewPairs);
			allocator->deallocate(sortBuff);
		}

		PsPair *outNewPairs = (PsPair*)allocator->allocate(sizeof(PsPair)*numNewPairs);
		PsPair *outKeepPairs = (PsPair*)allocator->allocate(sizeof(PsPair)*numOldPairs);
		assert(outNewPairs);
		assert(outKeepPairs);

		uint32_t nNew = 0;
		uint32_t nKeep = 0;

		uint32_t oldId = 0, newId = 0;

		while (oldId<numOldPairs&&newId<numNewPairs) {
			if (newPairs[newId].key > oldPairs[oldId].key) {
				// remove
				allocator->deallocate(oldPairs[oldId].contact);
				oldId++;
			}
			else if (newPairs[newId].key == oldPairs[oldId].key) {
				// keep
				assert(nKeep <= numOldPairs);
				outKeepPairs[nKeep] = oldPairs[oldId];
				nKeep++;
				oldId++;
				newId++;
			}
			else {
				// new
				assert(nNew <= numNewPairs);
				outNewPairs[nNew] = newPairs[newId];
				nNew++;
				newId++;
			}
		};

		if (newId<numNewPairs) {
			// all new
			for (; newId<numNewPairs; newId++, nNew++) {
				assert(nNew <= numNewPairs);
				outNewPairs[nNew] = newPairs[newId];
			}
		}
		else if (oldId<numOldPairs) {
			// all remove
			for (; oldId<numOldPairs; oldId++) {
				allocator->deallocate(oldPairs[oldId].contact);
			}
		}

		for (uint32_t i = 0; i<nNew; i++) {
			outNewPairs[i].contact = (PsContact*)allocator->allocate(sizeof(PsContact));
			outNewPairs[i].contact->reset();
		}

		for (uint32_t i = 0; i<nKeep; i++) {
			outKeepPairs[i].contact->refresh(
				states[outKeepPairs[i].rigidBodyA].m_position,
				states[outKeepPairs[i].rigidBodyA].m_orientation,
				states[outKeepPairs[i].rigidBodyB].m_position,
				states[outKeepPairs[i].rigidBodyB].m_orientation);
		}

		numNewPairs = 0;
		for (uint32_t i = 0; i<nKeep; i++) {
			outKeepPairs[i].type = PsPairType::PairTypeKeep;
			newPairs[numNewPairs++] = outKeepPairs[i];
		}
		for (uint32_t i = 0; i<nNew; i++) {
			outNewPairs[i].type = PsPairType::PairTypeNew;
			newPairs[numNewPairs++] = outNewPairs[i];
		}

		allocator->deallocate(outKeepPairs);
		allocator->deallocate(outNewPairs);

		{
			PsPair *sortBuff = (PsPair*)allocator->allocate(sizeof(PsPair)*numNewPairs);
			PsSort<PsPair>(newPairs, sortBuff, numNewPairs);
			allocator->deallocate(sortBuff);
		}
	}

	enum class PsSatType {
		SatTypePointAFacetB,
		SatTypePointBFacetA,
		SatTypeEdgeEdge,
	};

#define PS_CHECK_MINMAX(axis,AMin,AMax,BMin,BMax,type) \
{\
	satCount++;\
	float d1 = AMin - BMax;\
	float d2 = BMin - AMax;\
	if(d1 >= 0.0f || d2 >= 0.0f) {\
		return false;\
	}\
	if(distanceMin < d1) {\
		distanceMin = d1;\
		axisMin = axis;\
		satType = type;\
		axisFlip = false;\
	}\
	if(distanceMin < d2) {\
		distanceMin = d2;\
		axisMin = -axis;\
		satType = type;\
		axisFlip = true;\
	}\
}


	inline const bsm::Mat4x4 orthoInverse(const bsm::Mat4x4 & tfrm)
	{
		bsm::Flt3 inv0, inv1, inv2;
		inv0 = bsm::Flt3(tfrm.getMajor0().x, tfrm.getMajor1().x, tfrm.getMajor2().x);
		inv1 = bsm::Flt3(tfrm.getMajor0().y, tfrm.getMajor1().y, tfrm.getMajor2().y);
		inv2 = bsm::Flt3(tfrm.getMajor0().z, tfrm.getMajor1().z, tfrm.getMajor2().z);
		return bsm::Mat4x4(
			inv0,
			inv1,
			inv2,
			Flt3((-((inv0 * tfrm.getMajor3().x) + ((inv1 * tfrm.getMajor3().y) + (inv2 * tfrm.getMajor3().z)))))
		);
	}

	void PsGetProjection(
		float &pmin, float &pmax,
		const PsConvexMesh *convexMesh,
		const bsm::Vec3 &axis)
	{
		assert(convexMesh);

		float pmin_ = PS_FLT_MAX;
		float pmax_ = -PS_FLT_MAX;

		for (uint32_t i = 0; i<convexMesh->m_numVertices; i++) {
			float prj = dot(axis, convexMesh->m_vertices[i]);
			pmin_ = PS_MIN(pmin_, prj);
			pmax_ = PS_MAX(pmax_, prj);
		}

		pmin = pmin_;
		pmax = pmax_;
	}

	void PsGetClosestTwoSegments(
		const bsm::Vec3 &segmentPointA0, const bsm::Vec3 &segmentPointA1,
		const bsm::Vec3 &segmentPointB0, const bsm::Vec3 &segmentPointB1,
		bsm::Vec3 &closestPointA, bsm::Vec3 &closestPointB)
	{
		bsm::Vec3 v1 = segmentPointA1 - segmentPointA0;
		bsm::Vec3 v2 = segmentPointB1 - segmentPointB0;
		bsm::Vec3 r = segmentPointA0 - segmentPointB0;

		float a = dot(v1, v1);
		float b = dot(v1, v2);
		float c = dot(v2, v2);
		float d = dot(v1, r);
		float e = dot(v2, r);
		float det = -a*c + b*b;
		float s = 0.0f, t = 0.0f;

		if (det*det > PS_EPSILON) {
			s = (c*d - b*e) / det;
		}

		s = PS_CLAMP(s, 0.0f, 1.0f);

		t = (e + s*b) / c;

		t = PS_CLAMP(t, 0.0f, 1.0f);

		s = (-d + t*b) / a;
		s = PS_CLAMP(s, 0.0f, 1.0f);

		closestPointA = segmentPointA0 + s * v1;
		closestPointB = segmentPointB0 + t * v2;
	}

	inline
	void PsGetClosestPointLine(
			const bsm::Vec3 &point,
			const bsm::Vec3 &linePoint,
			const bsm::Vec3 &lineDirection,
			bsm::Vec3 &closestPoint)
	{
		float s = dot(point - linePoint, lineDirection) / dot(lineDirection, lineDirection);
		closestPoint = linePoint + s * lineDirection;
	}


	void PsGetClosestPointTriangle(
		const bsm::Vec3 &point,
		const bsm::Vec3 &trianglePoint0,
		const bsm::Vec3 &trianglePoint1,
		const bsm::Vec3 &trianglePoint2,
		const bsm::Vec3 &triangleNormal,
		bsm::Vec3 &closestPoint)
	{
		bsm::Vec3 proj = point - dot(triangleNormal, point - trianglePoint0) * triangleNormal;

		bsm::Vec3 edgeP01 = trianglePoint1 - trianglePoint0;
		bsm::Vec3 edgeP01_normal = cross(edgeP01, triangleNormal);

		float voronoiEdgeP01_check1 = dot(proj - trianglePoint0, edgeP01_normal);
		float voronoiEdgeP01_check2 = dot(proj - trianglePoint0, edgeP01);
		float voronoiEdgeP01_check3 = dot(proj - trianglePoint1, -edgeP01);

		if (voronoiEdgeP01_check1 > 0.0f && voronoiEdgeP01_check2 > 0.0f && voronoiEdgeP01_check3 > 0.0f) {
			PsGetClosestPointLine(trianglePoint0, edgeP01, proj, closestPoint);
			return;
		}

		bsm::Vec3 edgeP12 = trianglePoint2 - trianglePoint1;
		bsm::Vec3 edgeP12_normal = cross(edgeP12, triangleNormal);

		float voronoiEdgeP12_check1 = dot(proj - trianglePoint1, edgeP12_normal);
		float voronoiEdgeP12_check2 = dot(proj - trianglePoint1, edgeP12);
		float voronoiEdgeP12_check3 = dot(proj - trianglePoint2, -edgeP12);

		if (voronoiEdgeP12_check1 > 0.0f && voronoiEdgeP12_check2 > 0.0f && voronoiEdgeP12_check3 > 0.0f) {
			PsGetClosestPointLine(trianglePoint1, edgeP12, proj, closestPoint);
			return;
		}

		bsm::Vec3 edgeP20 = trianglePoint0 - trianglePoint2;
		bsm::Vec3 edgeP20_normal = cross(edgeP20, triangleNormal);

		float voronoiEdgeP20_check1 = dot(proj - trianglePoint2, edgeP20_normal);
		float voronoiEdgeP20_check2 = dot(proj - trianglePoint2, edgeP20);
		float voronoiEdgeP20_check3 = dot(proj - trianglePoint0, -edgeP20);

		if (voronoiEdgeP20_check1 > 0.0f && voronoiEdgeP20_check2 > 0.0f && voronoiEdgeP20_check3 > 0.0f) {
			PsGetClosestPointLine(trianglePoint2, edgeP20, proj, closestPoint);
			return;
		}

		if (voronoiEdgeP01_check1 <= 0.0f && voronoiEdgeP12_check1 <= 0.0f && voronoiEdgeP20_check1 <= 0.0f) {
			closestPoint = proj;
			return;
		}

		if (voronoiEdgeP01_check2 <= 0.0f && voronoiEdgeP20_check3 <= 0.0f) {
			closestPoint = trianglePoint0;
			return;
		}

		if (voronoiEdgeP01_check3 <= 0.0f && voronoiEdgeP12_check2 <= 0.0f) {
			closestPoint = trianglePoint1;
			return;
		}

		if (voronoiEdgeP20_check2 <= 0.0f && voronoiEdgeP12_check3 <= 0.0f) {
			closestPoint = trianglePoint2;
			return;
		}
	}


	bool PsConvexConvexContact_local(
		const PsConvexMesh &convexA, const bsm::Mat4x4 &transformA,
		const PsConvexMesh &convexB, const bsm::Mat4x4 &transformB,
		bsm::Vec3 &normal,
		float &penetrationDepth,
		bsm::Vec3 &contactPointA,
		bsm::Vec3 &contactPointB)
	{
		bsm::Mat4x4 transformAB, transformBA;
		bsm::Mat3x3 matrixAB, matrixBA;
		bsm::Vec3 offsetAB, offsetBA;

		transformAB = transformB * orthoInverse(transformA);
		matrixAB = transformAB.getUpper3x3();
		offsetAB = transformAB.getTranslation();

		transformBA = orthoInverse(transformAB);
		matrixBA = transformBA.getUpper3x3();
		offsetBA = transformBA.getTranslation();

		float distanceMin = -PS_FLT_MAX;
		bsm::Vec3 axisMin(0.0f);
		PsSatType satType = PsSatType::SatTypeEdgeEdge;
		bool axisFlip;

		int satCount = 0;

		for (uint32_t f = 0; f<convexA.m_numFacets; f++) {
			const PsFacet &facet = convexA.m_facets[f];
			const bsm::Vec3 separatingAxis = facet.normal;

			float minA, maxA;
			PsGetProjection(minA, maxA, &convexA, separatingAxis);

			float minB, maxB;
			PsGetProjection(minB, maxB, &convexB, matrixBA * facet.normal);
			float offset = dot(offsetAB, separatingAxis);
			minB += offset;
			maxB += offset;

			PS_CHECK_MINMAX(separatingAxis, minA, maxA, minB, maxB, PsSatType::SatTypePointBFacetA);
		}

		for (uint32_t f = 0; f<convexB.m_numFacets; f++) {
			const PsFacet &facet = convexB.m_facets[f];
			const bsm::Vec3 separatingAxis = matrixAB * facet.normal;

			float minA, maxA;
			PsGetProjection(minA, maxA, &convexA, separatingAxis);

			float minB, maxB;
			PsGetProjection(minB, maxB, &convexB, facet.normal);
			float offset = dot(offsetAB, separatingAxis);
			minB += offset;
			maxB += offset;

			PS_CHECK_MINMAX(separatingAxis, minA, maxA, minB, maxB, PsSatType::SatTypePointAFacetB);
		}

		for (uint32_t eA = 0; eA<convexA.m_numEdges; eA++) {
			const PsEdge &edgeA = convexA.m_edges[eA];
			if (edgeA.type != PsEdgeType::EdgeTypeConvex) continue;

			const bsm::Vec3 edgeVecA = convexA.m_vertices[edgeA.vertId[1]] - convexA.m_vertices[edgeA.vertId[0]];

			for (uint32_t eB = 0; eB<convexB.m_numEdges; eB++) {
				const PsEdge &edgeB = convexB.m_edges[eB];
				if (edgeB.type != PsEdgeType::EdgeTypeConvex) continue;

				const bsm::Vec3 edgeVecB = matrixAB * (convexB.m_vertices[edgeB.vertId[1]] - convexB.m_vertices[edgeB.vertId[0]]);

				bsm::Vec3 separatingAxis = cross(edgeVecA, edgeVecB);
				if (lengthSqr(separatingAxis) < PS_EPSILON*PS_EPSILON) continue;

				separatingAxis = normalize(separatingAxis);

				float minA, maxA;
				PsGetProjection(minA, maxA, &convexA, separatingAxis);

				float minB, maxB;
				PsGetProjection(minB, maxB, &convexB, matrixBA * separatingAxis);
				float offset = dot(offsetAB, separatingAxis);
				minB += offset;
				maxB += offset;

				PS_CHECK_MINMAX(separatingAxis, minA, maxA, minB, maxB, PsSatType::SatTypeEdgeEdge);
			}
		}
		int collCount = 0;

		float closestMinSqr = PS_FLT_MAX;
		bsm::Vec3 closestPointA, closestPointB;
		bsm::Vec3 separation = 1.1f * fabs(distanceMin) * axisMin;

		for (uint32_t fA = 0; fA<convexA.m_numFacets; fA++) {
			const PsFacet &facetA = convexA.m_facets[fA];

			float checkA = dot(facetA.normal, -axisMin);
			if (satType == PsSatType::SatTypePointBFacetA && checkA < 0.99f && axisFlip) {
				continue;
			}

			if (checkA < 0.0f) {
				continue;
			}

			for (uint32_t fB = 0; fB<convexB.m_numFacets; fB++) {
				const PsFacet &facetB = convexB.m_facets[fB];

				float checkB = dot(facetB.normal, matrixBA * axisMin);
				if (satType == PsSatType::SatTypePointAFacetB && checkB < 0.99f && !axisFlip) {
					continue;
				}

				if (checkB < 0.0f) {
					continue;
				}

				collCount++;

				// 面Ａと面Ｂの最近接点を求める
				bsm::Vec3 triangleA[3] = {
					separation + convexA.m_vertices[facetA.vertId[0]],
					separation + convexA.m_vertices[facetA.vertId[1]],
					separation + convexA.m_vertices[facetA.vertId[2]],
				};

				bsm::Vec3 triangleB[3] = {
					offsetAB + matrixAB * convexB.m_vertices[facetB.vertId[0]],
					offsetAB + matrixAB * convexB.m_vertices[facetB.vertId[1]],
					offsetAB + matrixAB * convexB.m_vertices[facetB.vertId[2]],
				};

				for (int i = 0; i<3; i++) {
					if (convexA.m_edges[facetA.edgeId[i]].type != PsEdgeType::EdgeTypeConvex) continue;

					for (int j = 0; j<3; j++) {
						if (convexB.m_edges[facetB.edgeId[j]].type != PsEdgeType::EdgeTypeConvex) continue;

						bsm::Vec3 sA, sB;
						PsGetClosestTwoSegments(
							triangleA[i], triangleA[(i + 1) % 3],
							triangleB[j], triangleB[(j + 1) % 3],
							sA, sB);

						float dSqr = lengthSqr(sA - sB);
						if (dSqr < closestMinSqr) {
							closestMinSqr = dSqr;
							closestPointA = sA;
							closestPointB = sB;
						}
					}
				}

				for (int i = 0; i<3; i++) {
					bsm::Vec3 s;
					PsGetClosestPointTriangle(triangleA[i], triangleB[0], triangleB[1], triangleB[2], matrixAB * facetB.normal, s);
					float dSqr = lengthSqr(triangleA[i] - s);
					if (dSqr < closestMinSqr) {
						closestMinSqr = dSqr;
						closestPointA = triangleA[i];
						closestPointB = s;
					}
				}

				for (int i = 0; i<3; i++) {
					bsm::Vec3 s;
					PsGetClosestPointTriangle(triangleB[i], triangleA[0], triangleA[1], triangleA[2], facetA.normal, s);
					float dSqr = lengthSqr(triangleB[i] - s);
					if (dSqr < closestMinSqr) {
						closestMinSqr = dSqr;
						closestPointA = s;
						closestPointB = triangleB[i];
					}
				}
			}
		}

		normal = transformA.getUpper3x3() * axisMin;
		penetrationDepth = distanceMin;
		contactPointA = closestPointA - separation;
		contactPointB = offsetBA + matrixBA * closestPointB;

		return true;
	}



	bool PsConvexConvexContact(
		const PsConvexMesh &convexA, const bsm::Mat4x4 &transformA,
		const PsConvexMesh &convexB, const bsm::Mat4x4 &transformB,
		bsm::Vec3 &normal,
		float &penetrationDepth,
		bsm::Vec3 &contactPointA,
		bsm::Vec3 &contactPointB)
	{

		bool ret;
		if (convexA.m_numFacets >= convexB.m_numFacets) {
			ret = PsConvexConvexContact_local(
				convexA, transformA,
				convexB, transformB,
				normal, penetrationDepth, contactPointA, contactPointB);
		}
		else {
			ret = PsConvexConvexContact_local(
				convexB, transformB,
				convexA, transformA,
				normal, penetrationDepth, contactPointB, contactPointA);
			normal = -normal;
		}

		return ret;
	}




	void PsDetectCollision(
		const PsState *states,
		const PsCollidable *collidables,
		uint32_t numRigidBodies,
		const PsPair *pairs,
		uint32_t numPairs)
	{
		assert(states);
		assert(collidables);
		assert(pairs);

		for (uint32_t i = 0; i<numPairs; i++) {
			const PsPair &pair = pairs[i];

			assert(pair.contact);

			const PsState &stateA = states[pair.rigidBodyA];
			const PsState &stateB = states[pair.rigidBodyB];
			const PsCollidable &collA = collidables[pair.rigidBodyA];
			const PsCollidable &collB = collidables[pair.rigidBodyB];

			bsm::Mat4x4 transformA(stateA.m_orientation, stateA.m_position);
			bsm::Mat4x4 transformB(stateB.m_orientation, stateB.m_position);

			for (uint32_t j = 0; j<collA.m_numShapes; j++) {
				const PsShape &shapeA = collA.m_shapes[j];
				bsm::Mat4x4 offsetTransformA(shapeA.m_offsetOrientation, shapeA.m_offsetPosition);
				bsm::Mat4x4 worldTransformA = offsetTransformA * transformA;

				for (uint32_t k = 0; k<collB.m_numShapes; k++) {
					const PsShape &shapeB = collB.m_shapes[k];
					bsm::Mat4x4 offsetTransformB(shapeB.m_offsetOrientation, shapeB.m_offsetPosition);
					bsm::Mat4x4 worldTransformB = offsetTransformB * transformB;

					bsm::Vec3 contactPointA;
					bsm::Vec3 contactPointB;
					bsm::Vec3 normal;
					float penetrationDepth;

					if (PsConvexConvexContact(
						shapeA.m_geometry, worldTransformA,
						shapeB.m_geometry, worldTransformB,
						normal, penetrationDepth,
						contactPointA, contactPointB) && penetrationDepth < 0.0f) {

						pair.contact->addContact(
							penetrationDepth, normal,
							offsetTransformA.getTranslation() + offsetTransformA.getUpper3x3() * contactPointA,
							offsetTransformB.getTranslation() + offsetTransformB.getUpper3x3() * contactPointB);
					}
				}
			}
		}
	}

	struct PsSolverBody {
		bsm::Vec3 deltaLinearVelocity;
		bsm::Vec3 deltaAngularVelocity;
		bsm::Quat    orientation;
		bsm::Mat3x3 inertiaInv;
		float   massInv;
	};

	inline void PsCalcTangentVector(const bsm::Vec3 &normal, bsm::Vec3 &tangent1, bsm::Vec3 &tangent2)
	{
		bsm::Vec3 vec(1.0f, 0.0f, 0.0f);
		bsm::Vec3 n(normal);
		n[0] = 0.0f;
		if (lengthSqr(n) < PS_EPSILON) {
			vec = bsm::Vec3(0.0f, 1.0f, 0.0f);
		}
		tangent1 = normalize(cross(normal, vec));
		tangent2 = normalize(cross(tangent1, normal));
	}


	void PsSolveConstraints(
		PsState *states,
		const PsRigidBody *bodies,
		uint32_t numRigidBodies,
		const PsPair *pairs,
		uint32_t numPairs,
		PsBallJoint *joints,
		uint32_t numJoints,
		uint32_t iteration,
		float bias,
		float slop,
		float timeStep,
		PsAllocator *allocator)
	{
		assert(states);
		assert(bodies);
		assert(pairs);

		PsSolverBody *solverBodies = (PsSolverBody*)allocator->allocate(sizeof(PsSolverBody)*numRigidBodies);
		assert(solverBodies);

		for (uint32_t i = 0; i<numRigidBodies; i++) {
			PsState &state = states[i];
			const PsRigidBody &body = bodies[i];
			PsSolverBody &solverBody = solverBodies[i];

			solverBody.orientation = state.m_orientation;
			solverBody.deltaLinearVelocity = bsm::Vec3(0.0f);
			solverBody.deltaAngularVelocity = bsm::Vec3(0.0f);

			if (state.m_motionType == PsMotionType::MotionTypeStatic) {
				solverBody.massInv = 0.0f;
				solverBody.inertiaInv = bsm::Mat3x3(0.0f);
			}
			else {
				solverBody.massInv = 1.0f / body.m_mass;
				bsm::Mat3x3 m(solverBody.orientation);
				solverBody.inertiaInv = transpose(m) * (inverse(body.m_inertia) * m);
			}
		}

		for (uint32_t i = 0; i<numJoints; i++) {
			PsBallJoint &joint = joints[i];

			PsState &stateA = states[joint.rigidBodyA];
			const PsRigidBody &bodyA = bodies[joint.rigidBodyA];
			PsSolverBody &solverBodyA = solverBodies[joint.rigidBodyA];

			PsState &stateB = states[joint.rigidBodyB];
			const PsRigidBody &bodyB = bodies[joint.rigidBodyB];
			PsSolverBody &solverBodyB = solverBodies[joint.rigidBodyB];

			bsm::Vec3 rA = rotate(solverBodyA.orientation, joint.anchorA);
			bsm::Vec3 rB = rotate(solverBodyB.orientation, joint.anchorB);

			bsm::Vec3 positionA = stateA.m_position + rA;
			bsm::Vec3 positionB = stateB.m_position + rB;
			bsm::Vec3 direction = positionA - positionB;
			float distanceSqr = lengthSqr(direction);

			if (distanceSqr < PS_EPSILON * PS_EPSILON) {
				joint.constraint.jacDiagInv = 0.0f;
				joint.constraint.rhs = 0.0f;
				joint.constraint.lowerLimit = -PS_FLT_MAX;
				joint.constraint.upperLimit = PS_FLT_MAX;
				joint.constraint.axis = bsm::Vec3(1.0f, 0.0f, 0.0f);
				continue;
			}

			float distance = sqrtf(distanceSqr);
			direction /= distance;

			bsm::Vec3 velocityA = stateA.m_linearVelocity + cross(stateA.m_angularVelocity, rA);
			bsm::Vec3 velocityB = stateB.m_linearVelocity + cross(stateB.m_angularVelocity, rB);
			bsm::Vec3 relativeVelocity = velocityA - velocityB;

			bsm::Mat3x3 K;
			K.scale(bsm::Vec3(solverBodyA.massInv + solverBodyB.massInv));
			K -= crossMatrix(rA) * solverBodyA.inertiaInv * crossMatrix(rA);
			K -= crossMatrix(rB) * solverBodyB.inertiaInv * crossMatrix(rB);


			float denom = dot(K * direction, direction);
			joint.constraint.jacDiagInv = 1.0f / denom;
			joint.constraint.rhs = -dot(relativeVelocity, direction); // velocity error
			joint.constraint.rhs -= joint.bias * distance / timeStep; // position error
			joint.constraint.rhs *= joint.constraint.jacDiagInv;
			joint.constraint.lowerLimit = -PS_FLT_MAX;
			joint.constraint.upperLimit = PS_FLT_MAX;
			joint.constraint.axis = direction;

			joint.constraint.accumImpulse = 0.0f;
		}

		for (uint32_t i = 0; i<numPairs; i++) {
			const PsPair &pair = pairs[i];

			PsState &stateA = states[pair.rigidBodyA];
			const PsRigidBody &bodyA = bodies[pair.rigidBodyA];
			PsSolverBody &solverBodyA = solverBodies[pair.rigidBodyA];

			PsState &stateB = states[pair.rigidBodyB];
			const PsRigidBody &bodyB = bodies[pair.rigidBodyB];
			PsSolverBody &solverBodyB = solverBodies[pair.rigidBodyB];

			assert(pair.contact);

			pair.contact->m_friction = sqrtf(bodyA.m_friction * bodyB.m_friction);

			for (uint32_t j = 0; j<pair.contact->m_numContacts; j++) {
				PsContactPoint &cp = pair.contact->m_contactPoints[j];

				bsm::Vec3 rA = rotate(solverBodyA.orientation, cp.pointA);
				bsm::Vec3 rB = rotate(solverBodyB.orientation, cp.pointB);

				bsm::Mat3x3 K;
				K.scale(bsm::Vec3(solverBodyA.massInv + solverBodyB.massInv));
				K -= crossMatrix(rA) * solverBodyA.inertiaInv * crossMatrix(rA);
				K -= crossMatrix(rB) * solverBodyB.inertiaInv * crossMatrix(rB);

				bsm::Vec3 velocityA = stateA.m_linearVelocity + cross(stateA.m_angularVelocity, rA);
				bsm::Vec3 velocityB = stateB.m_linearVelocity + cross(stateB.m_angularVelocity, rB);
				bsm::Vec3 relativeVelocity = velocityA - velocityB;

				bsm::Vec3 tangent1, tangent2;

				PsCalcTangentVector(cp.normal, tangent1, tangent2);

				float restitution = pair.type == PsPairType::PairTypeNew ? 0.5f*(bodyA.m_restitution + bodyB.m_restitution) : 0.0f;

				{
					bsm::Vec3 axis = cp.normal;
					float denom = dot(K * axis, axis);
					cp.constraints[0].jacDiagInv = 1.0f / denom;
					cp.constraints[0].rhs = -(1.0f + restitution) * dot(relativeVelocity, axis); // velocity error
					cp.constraints[0].rhs -= (bias * PS_MIN(0.0f, cp.distance + slop)) / timeStep; // position error
					cp.constraints[0].rhs *= cp.constraints[0].jacDiagInv;
					cp.constraints[0].lowerLimit = 0.0f;
					cp.constraints[0].upperLimit = PS_FLT_MAX;
					cp.constraints[0].axis = axis;
				}

				{
					bsm::Vec3 axis = tangent1;
					float denom = dot(K * axis, axis);
					cp.constraints[1].jacDiagInv = 1.0f / denom;
					cp.constraints[1].rhs = -dot(relativeVelocity, axis);
					cp.constraints[1].rhs *= cp.constraints[1].jacDiagInv;
					cp.constraints[1].lowerLimit = 0.0f;
					cp.constraints[1].upperLimit = 0.0f;
					cp.constraints[1].axis = axis;
				}

				{
					bsm::Vec3 axis = tangent2;
					float denom = dot(K * axis, axis);
					cp.constraints[2].jacDiagInv = 1.0f / denom;
					cp.constraints[2].rhs = -dot(relativeVelocity, axis);
					cp.constraints[2].rhs *= cp.constraints[2].jacDiagInv;
					cp.constraints[2].lowerLimit = 0.0f;
					cp.constraints[2].upperLimit = 0.0f;
					cp.constraints[2].axis = axis;
				}
			}
		}

		for (uint32_t i = 0; i<numPairs; i++) {
			const PsPair &pair = pairs[i];

			PsSolverBody &solverBodyA = solverBodies[pair.rigidBodyA];
			PsSolverBody &solverBodyB = solverBodies[pair.rigidBodyB];

			for (uint32_t j = 0; j<pair.contact->m_numContacts; j++) {
				PsContactPoint &cp = pair.contact->m_contactPoints[j];
				bsm::Vec3 rA = rotate(solverBodyA.orientation, cp.pointA);
				bsm::Vec3 rB = rotate(solverBodyB.orientation, cp.pointB);

				for (uint32_t k = 0; k<3; k++) {
					float deltaImpulse = cp.constraints[k].accumImpulse;
					solverBodyA.deltaLinearVelocity += deltaImpulse * solverBodyA.massInv * cp.constraints[k].axis;
					solverBodyA.deltaAngularVelocity += deltaImpulse * solverBodyA.inertiaInv * cross(rA, cp.constraints[k].axis);
					solverBodyB.deltaLinearVelocity -= deltaImpulse * solverBodyB.massInv * cp.constraints[k].axis;
					solverBodyB.deltaAngularVelocity -= deltaImpulse * solverBodyB.inertiaInv * cross(rB, cp.constraints[k].axis);
				}
			}
		}

		for (uint32_t itr = 0; itr<iteration; itr++) {
			for (uint32_t i = 0; i<numJoints; i++) {
				PsBallJoint &joint = joints[i];

				PsSolverBody &solverBodyA = solverBodies[joint.rigidBodyA];
				PsSolverBody &solverBodyB = solverBodies[joint.rigidBodyB];

				bsm::Vec3 rA = rotate(solverBodyA.orientation, joint.anchorA);
				bsm::Vec3 rB = rotate(solverBodyB.orientation, joint.anchorB);

				PsConstraint &constraint = joint.constraint;
				float deltaImpulse = constraint.rhs;
				bsm::Vec3 deltaVelocityA = solverBodyA.deltaLinearVelocity + cross(solverBodyA.deltaAngularVelocity, rA);
				bsm::Vec3 deltaVelocityB = solverBodyB.deltaLinearVelocity + cross(solverBodyB.deltaAngularVelocity, rB);
				deltaImpulse -= constraint.jacDiagInv * dot(constraint.axis, deltaVelocityA - deltaVelocityB);
				float oldImpulse = constraint.accumImpulse;
				constraint.accumImpulse = PS_CLAMP(oldImpulse + deltaImpulse, constraint.lowerLimit, constraint.upperLimit);
				deltaImpulse = constraint.accumImpulse - oldImpulse;
				solverBodyA.deltaLinearVelocity += deltaImpulse * solverBodyA.massInv * constraint.axis;
				solverBodyA.deltaAngularVelocity += deltaImpulse * solverBodyA.inertiaInv * cross(rA, constraint.axis);
				solverBodyB.deltaLinearVelocity -= deltaImpulse * solverBodyB.massInv * constraint.axis;
				solverBodyB.deltaAngularVelocity -= deltaImpulse * solverBodyB.inertiaInv * cross(rB, constraint.axis);
			}

			for (uint32_t i = 0; i<numPairs; i++) {
				const PsPair &pair = pairs[i];

				PsSolverBody &solverBodyA = solverBodies[pair.rigidBodyA];
				PsSolverBody &solverBodyB = solverBodies[pair.rigidBodyB];

				for (uint32_t j = 0; j<pair.contact->m_numContacts; j++) {
					PsContactPoint &cp = pair.contact->m_contactPoints[j];
					bsm::Vec3 rA = rotate(solverBodyA.orientation, cp.pointA);
					bsm::Vec3 rB = rotate(solverBodyB.orientation, cp.pointB);

					{
						PsConstraint &constraint = cp.constraints[0];
						float deltaImpulse = constraint.rhs;
						bsm::Vec3 deltaVelocityA = solverBodyA.deltaLinearVelocity + cross(solverBodyA.deltaAngularVelocity, rA);
						bsm::Vec3 deltaVelocityB = solverBodyB.deltaLinearVelocity + cross(solverBodyB.deltaAngularVelocity, rB);
						deltaImpulse -= constraint.jacDiagInv * dot(constraint.axis, deltaVelocityA - deltaVelocityB);
						float oldImpulse = constraint.accumImpulse;
						constraint.accumImpulse = PS_CLAMP(oldImpulse + deltaImpulse, constraint.lowerLimit, constraint.upperLimit);
						deltaImpulse = constraint.accumImpulse - oldImpulse;
						solverBodyA.deltaLinearVelocity += deltaImpulse * solverBodyA.massInv * constraint.axis;
						solverBodyA.deltaAngularVelocity += deltaImpulse * solverBodyA.inertiaInv * cross(rA, constraint.axis);
						solverBodyB.deltaLinearVelocity -= deltaImpulse * solverBodyB.massInv * constraint.axis;
						solverBodyB.deltaAngularVelocity -= deltaImpulse * solverBodyB.inertiaInv * cross(rB, constraint.axis);
					}

					float maxFriction = pair.contact->m_friction * fabs(cp.constraints[0].accumImpulse);
					cp.constraints[1].lowerLimit = -maxFriction;
					cp.constraints[1].upperLimit = maxFriction;
					cp.constraints[2].lowerLimit = -maxFriction;
					cp.constraints[2].upperLimit = maxFriction;

					{
						PsConstraint &constraint = cp.constraints[1];
						float deltaImpulse = constraint.rhs;
						bsm::Vec3 deltaVelocityA = solverBodyA.deltaLinearVelocity + cross(solverBodyA.deltaAngularVelocity, rA);
						bsm::Vec3 deltaVelocityB = solverBodyB.deltaLinearVelocity + cross(solverBodyB.deltaAngularVelocity, rB);
						deltaImpulse -= constraint.jacDiagInv * dot(constraint.axis, deltaVelocityA - deltaVelocityB);
						float oldImpulse = constraint.accumImpulse;
						constraint.accumImpulse = PS_CLAMP(oldImpulse + deltaImpulse, constraint.lowerLimit, constraint.upperLimit);
						deltaImpulse = constraint.accumImpulse - oldImpulse;
						solverBodyA.deltaLinearVelocity += deltaImpulse * solverBodyA.massInv * constraint.axis;
						solverBodyA.deltaAngularVelocity += deltaImpulse * solverBodyA.inertiaInv * cross(rA, constraint.axis);
						solverBodyB.deltaLinearVelocity -= deltaImpulse * solverBodyB.massInv * constraint.axis;
						solverBodyB.deltaAngularVelocity -= deltaImpulse * solverBodyB.inertiaInv * cross(rB, constraint.axis);
					}
					{
						PsConstraint &constraint = cp.constraints[2];
						float deltaImpulse = constraint.rhs;
						bsm::Vec3 deltaVelocityA = solverBodyA.deltaLinearVelocity + cross(solverBodyA.deltaAngularVelocity, rA);
						bsm::Vec3 deltaVelocityB = solverBodyB.deltaLinearVelocity + cross(solverBodyB.deltaAngularVelocity, rB);
						deltaImpulse -= constraint.jacDiagInv * dot(constraint.axis, deltaVelocityA - deltaVelocityB);
						float oldImpulse = constraint.accumImpulse;
						constraint.accumImpulse = PS_CLAMP(oldImpulse + deltaImpulse, constraint.lowerLimit, constraint.upperLimit);
						deltaImpulse = constraint.accumImpulse - oldImpulse;
						solverBodyA.deltaLinearVelocity += deltaImpulse * solverBodyA.massInv * constraint.axis;
						solverBodyA.deltaAngularVelocity += deltaImpulse * solverBodyA.inertiaInv * cross(rA, constraint.axis);
						solverBodyB.deltaLinearVelocity -= deltaImpulse * solverBodyB.massInv * constraint.axis;
						solverBodyB.deltaAngularVelocity -= deltaImpulse * solverBodyB.inertiaInv * cross(rB, constraint.axis);
					}
				}
			}
		}

		for (uint32_t i = 0; i<numRigidBodies; i++) {
			states[i].m_linearVelocity += solverBodies[i].deltaLinearVelocity;
			states[i].m_angularVelocity += solverBodies[i].deltaAngularVelocity;
		}

		allocator->deallocate(solverBodies);
	}

	void PsIntegrate(PsState *states, uint32_t numRigidBodies, float timeStep)
	{
		for (uint32_t i = 0; i<numRigidBodies; i++) {
			PsState &state = states[i];
			bsm::Quat Dest;
			Dest.x = state.m_angularVelocity.x;
			Dest.y = state.m_angularVelocity.y;
			Dest.z = state.m_angularVelocity.z;
			Dest.w = 0;
			bsm::Quat dAng = state.m_orientation * Dest * 0.5f;
			state.m_position += state.m_linearVelocity * timeStep;
			state.m_orientation = normalize(state.m_orientation + dAng * timeStep);
		}
	}

	inline
	float calcArea4Points(const bsm::Vec3 &p0, const bsm::Vec3 &p1, const bsm::Vec3 &p2, const bsm::Vec3 &p3)
	{
		float areaSqrA = lengthSqr(cross(p0 - p1, p2 - p3));
		float areaSqrB = lengthSqr(cross(p0 - p2, p1 - p3));
		float areaSqrC = lengthSqr(cross(p0 - p3, p1 - p2));
		return PS_MAX(PS_MAX(areaSqrA, areaSqrB), areaSqrC);
	}

	int PsContact::findNearestContactPoint(const bsm::Vec3 &newPointA, const bsm::Vec3 &newPointB, const bsm::Vec3 &newNormal)
	{
		int nearestIdx = -1;

		float minDiff = PS_CONTACT_SAME_POINT;
		for (uint32_t i = 0; i<m_numContacts; i++) {
			float diffA = lengthSqr(m_contactPoints[i].pointA - newPointA);
			float diffB = lengthSqr(m_contactPoints[i].pointB - newPointB);
			if (diffA < minDiff && diffB < minDiff && dot(newNormal, m_contactPoints[i].normal) > 0.99f) {
				minDiff = PS_MAX(diffA, diffB);
				nearestIdx = i;
			}
		}

		return nearestIdx;
	}

	int PsContact::sort4ContactPoints(const bsm::Vec3 &newPoint, float newDistance)
	{
		int maxPenetrationIndex = -1;
		float maxPenetration = newDistance;

		for (uint32_t i = 0; i<m_numContacts; i++) {
			if (m_contactPoints[i].distance < maxPenetration) {
				maxPenetrationIndex = i;
				maxPenetration = m_contactPoints[i].distance;
			}
		}

		float res[4] = { 0.0f };

		bsm::Vec3 newp(newPoint);
		bsm::Vec3 p[4] = {
			m_contactPoints[0].pointA,
			m_contactPoints[1].pointA,
			m_contactPoints[2].pointA,
			m_contactPoints[3].pointA,
		};

		if (maxPenetrationIndex != 0) {
			res[0] = calcArea4Points(newp, p[1], p[2], p[3]);
		}

		if (maxPenetrationIndex != 1) {
			res[1] = calcArea4Points(newp, p[0], p[2], p[3]);
		}

		if (maxPenetrationIndex != 2) {
			res[2] = calcArea4Points(newp, p[0], p[1], p[3]);
		}

		if (maxPenetrationIndex != 3) {
			res[3] = calcArea4Points(newp, p[0], p[1], p[2]);
		}

		int maxIndex = 0;
		float maxVal = res[0];

		if (res[1] > maxVal) {
			maxIndex = 1;
			maxVal = res[1];
		}

		if (res[2] > maxVal) {
			maxIndex = 2;
			maxVal = res[2];
		}

		if (res[3] > maxVal) {
			maxIndex = 3;
			maxVal = res[3];
		}

		return maxIndex;
	}

	void PsContact::reset()
	{
		m_numContacts = 0;
		for (int i = 0; i<PS_NUM_CONTACTS; i++) {
			m_contactPoints[i].reset();
		}
	}

	void PsContact::removeContactPoint(int i)
	{
		m_contactPoints[i] = m_contactPoints[m_numContacts - 1];
		m_numContacts--;
	}

	void PsContact::refresh(const bsm::Vec3 &pA, const bsm::Quat &qA, const bsm::Vec3 &pB, const bsm::Quat &qB)
	{
		for (int i = 0; i<(int)m_numContacts; i++) {
			bsm::Vec3 normal = m_contactPoints[i].normal;
			bsm::Vec3 cpA = pA + rotate(qA, m_contactPoints[i].pointA);
			bsm::Vec3 cpB = pB + rotate(qB, m_contactPoints[i].pointB);

			float distance = dot(normal, cpA - cpB);
			if (distance > PS_CONTACT_THRESHOLD_NORMAL) {
				removeContactPoint(i);
				i--;
				continue;
			}
			m_contactPoints[i].distance = distance;

			cpA = cpA - m_contactPoints[i].distance * normal;
			float distanceAB = lengthSqr(cpA - cpB);
			if (distanceAB > PS_CONTACT_THRESHOLD_TANGENT) {
				removeContactPoint(i);
				i--;
				continue;
			}
		}
	}

	void PsContact::merge(const PsContact &contact)
	{
		for (uint32_t i = 0; i<contact.m_numContacts; i++) {
			PsContactPoint &cp = m_contactPoints[i];

			int id = findNearestContactPoint(cp.pointA, cp.pointB, cp.normal);

			if (id >= 0) {
				if (fabs(dot(cp.normal, m_contactPoints[id].normal)) > 0.99f) {
					m_contactPoints[id].distance = cp.distance;
					m_contactPoints[id].pointA = cp.pointA;
					m_contactPoints[id].pointB = cp.pointB;
					m_contactPoints[id].normal = cp.normal;
				}
				else {
					m_contactPoints[id] = cp;
				}
			}
			else if (m_numContacts < PS_NUM_CONTACTS) {
				m_contactPoints[m_numContacts++] = cp;
			}
			else {
				id = sort4ContactPoints(cp.pointA, cp.distance);
				m_contactPoints[id] = cp;
			}
		}
	}

	void PsContact::addContact(
		float penetrationDepth,
		const bsm::Vec3 &normal,
		const bsm::Vec3 &contactPointA,
		const bsm::Vec3 &contactPointB)
	{
		int id = findNearestContactPoint(contactPointA, contactPointB, normal);

		if (id < 0 && m_numContacts < PS_NUM_CONTACTS) {
			id = m_numContacts++;
			m_contactPoints[id].reset();
		}
		else if (id < 0) {
			id = sort4ContactPoints(contactPointA, penetrationDepth);
			m_contactPoints[id].reset();
		}

		m_contactPoints[id].distance = penetrationDepth;
		m_contactPoints[id].pointA = contactPointA;
		m_contactPoints[id].pointB = contactPointB;
		m_contactPoints[id].normal = normal;
	}



	//--------------------------------------------------------------------------------------
	///	物理計算マネージャ
	//--------------------------------------------------------------------------------------
	PhysicsManager::PhysicsManager(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		m_BoxMeshRes(nullptr),
		m_PsTranformIndex(0)
	{}
	PhysicsManager::~PhysicsManager() {}

	uint32_t PhysicsManager::AddSingleShape(PsShapeType ShapeType, PsMotionType MotionType, const bsm::Vec3& Scale, const bsm::Quat& Qt, const bsm::Vec3& Pos) {
		uint32_t id = numRigidBodies++;

		bsm::Vec3 PsScale = Scale * 0.5f;

		states[id].reset();
		states[id].m_position = Pos;
		states[id].m_orientation = Qt;
		states[id].m_motionType = MotionType;
		bodies[id].reset();
		bodies[id].m_mass = 1.0f;
		switch (ShapeType) {
			case PsShapeType::ShapeTypeBox:
				bodies[id].m_inertia = PsCalcInertiaBox(PsScale, 1.0f);
			break;
		}
		collidables[id].reset();

		PsShape shape;
		shape.reset();
		shape.m_Scale = PsScale;
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		vector<float> new_vertices;
		switch (ShapeType) {
			case PsShapeType::ShapeTypeBox:
			{
				MeshUtill::CreateCube(2.0f, vertices, indices);
				//メッシュの作成（変更できない）
				if (!m_BoxMeshRes) {
					vector<VertexPositionColor> col_vertices;
					for (auto& v : vertices) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						col_vertices.push_back(vertex);
					}
					m_BoxMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
				}
				for (auto& v : vertices) {
					new_vertices.push_back(v.position.x);
					new_vertices.push_back(v.position.y);
					new_vertices.push_back(v.position.z);
				}
				shape.m_PsShapeType = PsShapeType::ShapeTypeBox;
			}
			break;
		}
		PsCreateConvexMesh(&shape.m_geometry, &new_vertices.front(), vertices.size(), &indices.front(), indices.size(), PsScale);
		auto shapeid = collidables[id].addShape(shape);
		collidables[id].finish();

		PsTranform trans(id, shapeid);
		m_PsTranformMap[m_PsTranformIndex] = trans;
		uint32_t retindex = m_PsTranformIndex;
		m_PsTranformIndex++;

		return retindex;
	}

	void PhysicsManager::GetShapeQuatPos(uint32_t TransIndex, bsm::Quat& RetQt, bsm::Vec3& RetPos) {
		if (TransIndex >= m_PsTranformIndex) {
			throw BaseException(
				L"指定されたインデックスが範囲外です",
				L"if (TransIndex >= m_PsTranformIndex)",
				L"PhysicsManager::GetShapePosition()"
			);

		}
		bsm::Mat4x4 World, Local;
		auto& Trans = m_PsTranformMap[TransIndex];
		auto& state = physicsGetState(Trans.stateIndex);
		const bsm::Vec3 Pos(state.m_position);
		const bsm::Quat Qt(state.m_orientation);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Qt,				//回転角度
			Pos				//位置
		);

		auto& shape = collidables[Trans.stateIndex].m_shapes[Trans.shapeIndex];
		const bsm::Vec3 LocalPos(shape.m_offsetPosition);
		const bsm::Quat LocalQt(shape.m_offsetOrientation);
		Local.affineTransformation(
			shape.m_Scale,			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);

		World = Local * World;
		RetQt = World.quatInMatrix();
		RetPos =  World.transInMatrix();
	}


	void PhysicsManager::OnCreate() {

	}

	void PhysicsManager::OnUpdate() {

		pairSwap = 1 - pairSwap;

		for (uint32_t i = 0; i<numRigidBodies; i++) {
			bsm::Vec3 externalForce = gravity * bodies[i].m_mass;
			bsm::Vec3 externalTorque(0.0f);
			PsApplyExternalForce(states[i], bodies[i], externalForce, externalTorque, timeStep);
		}

		PsBroadPhase(
			states, collidables, numRigidBodies,
			pairs[1 - pairSwap], numPairs[1 - pairSwap],
			pairs[pairSwap], numPairs[pairSwap],
			maxPairs, &allocator, NULL, NULL);

		PsDetectCollision(
			states, collidables, numRigidBodies,
			pairs[pairSwap], numPairs[pairSwap]);

		PsSolveConstraints(
			states, bodies, numRigidBodies,
			pairs[pairSwap], numPairs[pairSwap],
			joints, numJoints,
			iteration, contactBias, contactSlop, timeStep, &allocator);

		PsIntegrate(states, numRigidBodies, timeStep);


	}

	void PhysicsManager::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		bsm::Mat4x4 World, ViewMat, ProjMat;
		World = world;
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//エミッシブ
		sb.Emissive = Col4(0, 0, 0, 0);
		//デフィーズはすべて通す
		sb.Diffuse = Col4(1, 1, 1, 1);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, res->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(res->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//ブレンドステート
		//透明処理しない
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ラスタライザステート(ワイアフレーム)
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		pD3D11DeviceContext->DrawIndexed(res->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();

	}


	void PhysicsManager::OnDraw() {
		for (int i = 0; i < physicsGetNumRigidbodies(); i++) {
			//行列の定義
			bsm::Mat4x4 World, Local;
			auto& state = physicsGetState(i);
			const bsm::Vec3 Pos(state.m_position.x, state.m_position.y, state.m_position.z);
			const bsm::Quat Qt(state.m_orientation.x, state.m_orientation.y, state.m_orientation.z, state.m_orientation.w);
			//ワールド行列の決定
			World.affineTransformation(
				bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
				bsm::Vec3(0, 0, 0),		//回転の中心（重心）
				Qt,				//回転角度
				Pos				//位置
			);

			for (auto j = 0; j < collidables[i].m_numShapes; j++) {
				auto& shape = collidables[i].m_shapes[j];

				const bsm::Vec3 LocalPos(shape.m_offsetPosition);
				const bsm::Quat LocalQt(shape.m_offsetOrientation);
				Local.affineTransformation(
					shape.m_Scale,			//スケーリング
					bsm::Vec3(0, 0, 0),		//回転の中心（重心）
					LocalQt,				//回転角度
					LocalPos				//位置
				);

				bsm::Mat4x4 DrawWorld = Local * World;
				switch (shape.m_PsShapeType) {
				case PsShapeType::ShapeTypeBox:
					DrawShapeWireFrame(m_BoxMeshRes, DrawWorld);
					break;
				}
			}
		}

	}


}
//end basecross
