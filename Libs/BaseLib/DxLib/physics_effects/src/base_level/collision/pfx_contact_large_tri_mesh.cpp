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
#include "pfx_contact_tri_mesh_sphere.h"
#include "pfx_contact_tri_mesh_box.h"
#include "pfx_contact_tri_mesh_capsule.h"
#include "pfx_contact_tri_mesh_cylinder.h"
#include "pfx_contact_tri_mesh_convex.h"
#include "pfx_contact_large_tri_mesh.h"
#include "pfx_intersect_common.h"
#include "pfx_mesh_common.h"


namespace sce {
namespace PhysicsEffects {


PfxInt32 pfxContactLargeTriMesh(
				PfxContactCache &contacts,
				const PfxLargeTriMesh *lmeshA,
				const PfxTransform3 &transformA,
				const PfxShape &shapeB,
				const PfxTransform3 &transformB,
				PfxFloat distanceThreshold)
{
	PfxTransform3 transformAB,transformBA;
	PfxMatrix3 matrixAB;
	PfxVector3 offsetAB;
	
	// Bローカル→Aローカルへの変換
	transformAB = orthoInverse(transformA) * transformB;
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	
	// Aローカル→Bローカルへの変換
	transformBA = orthoInverse(transformAB);
	
	// -----------------------------------------------------
	// LargeTriMeshに含まれるTriMeshのAABBと凸体のAABBを判定し、
	// 交差するものを個別に衝突判定する。※LargeMesh座標系
	
	PfxVector3 shapeHalf(0.0f);
	PfxVector3 shapeCenter = offsetAB;
	
	const PfxConvexMesh *convexB = NULL;

	if(shapeB.getType() == kPfxShapeConvexMesh) {
	convexB = shapeB.getConvexMesh();
	}

	switch(shapeB.getType()) {
		case kPfxShapeSphere:
		shapeHalf = PfxVector3(shapeB.getSphere().m_radius);
		break;
		
		case kPfxShapeCapsule:
		{
			PfxCapsule capsule = shapeB.getCapsule();
			shapeHalf = absPerElem(matrixAB) * PfxVector3(capsule.m_halfLen+capsule.m_radius,capsule.m_radius,capsule.m_radius);
		}
		break;
		
		case kPfxShapeCylinder:
		{
			PfxCylinder cylinder = shapeB.getCylinder();
			shapeHalf = absPerElem(matrixAB) * PfxVector3(cylinder.m_halfLen,cylinder.m_radius,cylinder.m_radius);
		}
		break;
		
		case kPfxShapeBox:
		shapeHalf = absPerElem(matrixAB) * shapeB.getBox().m_half;
		break;
		
		case kPfxShapeConvexMesh:
		shapeHalf = absPerElem(matrixAB) * convexB->m_half;
		break;
		
		default:
		break;
	}

	// -----------------------------------------------------
	// アイランドとの衝突判定

	PfxVecInt3 aabbMinL,aabbMaxL;
	lmeshA->getLocalPosition((shapeCenter-shapeHalf),(shapeCenter+shapeHalf),aabbMinL,aabbMaxL);
	
	PfxUInt32 numIslands = lmeshA->m_numIslands;
	
	{
	for(PfxUInt32 i=0;i<numIslands;i++) {
		// AABBチェック
		PfxAabb16 aabbB = lmeshA->m_aabbList[i];
		if(aabbMaxL.getX() < pfxGetXMin(aabbB) || aabbMinL.getX() > pfxGetXMax(aabbB)) continue;
		if(aabbMaxL.getY() < pfxGetYMin(aabbB) || aabbMinL.getY() > pfxGetYMax(aabbB)) continue;
		if(aabbMaxL.getZ() < pfxGetZMin(aabbB) || aabbMinL.getZ() > pfxGetZMax(aabbB)) continue;
		
		void *island=NULL;
		
		if(lmeshA->m_type & 0x01) {
			island = ((PfxQuantizedTriMesh*)lmeshA->m_islands) + i;
		}
		else {
			island = ((PfxExpandedTriMesh*)lmeshA->m_islands) + i;
		}

			// 衝突判定
			PfxContactCache localContacts;
			if(lmeshA->m_type & 0x01) {
				switch(shapeB.getType()) {
					case kPfxShapeSphere:
					pfxContactTriMeshSphere(localContacts,lmeshA,(PfxQuantizedTriMesh*)island,shapeB.getSphere(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeCapsule:
					pfxContactTriMeshCapsule(localContacts,lmeshA,(PfxQuantizedTriMesh*)island,shapeB.getCapsule(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeBox:
					pfxContactTriMeshBox(localContacts,lmeshA,(PfxQuantizedTriMesh*)island,shapeB.getBox(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeCylinder:
					pfxContactTriMeshCylinder(localContacts,lmeshA,(PfxQuantizedTriMesh*)island,shapeB.getCylinder(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeConvexMesh:
					pfxContactTriMeshConvex(localContacts,lmeshA,(PfxQuantizedTriMesh*)island,*convexB,transformAB,distanceThreshold);
					break;
					
					default:
					break;
				}
			}
			else {
				switch(shapeB.getType()) {
					case kPfxShapeSphere:
					pfxContactTriMeshSphere(localContacts,lmeshA,(PfxExpandedTriMesh*)island,shapeB.getSphere(),transformAB,distanceThreshold);
					break;

					case kPfxShapeCapsule:
					pfxContactTriMeshCapsule(localContacts,lmeshA,(PfxExpandedTriMesh*)island,shapeB.getCapsule(),transformAB,distanceThreshold);
					break;

					case kPfxShapeBox:
					pfxContactTriMeshBox(localContacts,lmeshA,(PfxExpandedTriMesh*)island,shapeB.getBox(),transformAB,distanceThreshold);
					break;

					case kPfxShapeCylinder:
					pfxContactTriMeshCylinder(localContacts,lmeshA,(PfxExpandedTriMesh*)island,shapeB.getCylinder(),transformAB,distanceThreshold);
					break;

					case kPfxShapeConvexMesh:
					pfxContactTriMeshConvex(localContacts,lmeshA,(PfxExpandedTriMesh*)island,*convexB,transformAB,distanceThreshold);
					break;

					default:
					break;
				}
			}

			
			// 衝突点を追加
			// 結果はAローカルなので、変換する
			for(int j=0;j<localContacts.getNumContacts();j++) {
				PfxSubData subData = localContacts.getSubData(j);
				subData.setIslandId(i);
				contacts.addContactPoint(
					localContacts.getDistance(j),
					transformA.getUpper3x3() * localContacts.getNormal(j),
					localContacts.getLocalPointA(j),
					transformBA * localContacts.getLocalPointB(j),
					subData);
			}
		}
	}


	return contacts.getNumContacts();
}


PfxInt32 pfxContactLargeTriMeshBvh(
				PfxContactCache &contacts,
				const PfxLargeTriMesh *lmeshA,
				const PfxTransform3 &transformA,
				const PfxShape &shapeB,
				const PfxTransform3 &transformB,
				PfxFloat distanceThreshold)
{
	PfxTransform3 transformAB,transformBA;
	PfxMatrix3 matrixAB;
	PfxVector3 offsetAB;
	
	// Bローカル→Aローカルへの変換
	transformAB = orthoInverse(transformA) * transformB;
	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	
	// Aローカル→Bローカルへの変換
	transformBA = orthoInverse(transformAB);
	
	// -----------------------------------------------------
	// LargeTriMeshに含まれるTriMeshのAABBと凸体のAABBを判定し、
	// 交差するものを個別に衝突判定する。※LargeMesh座標系
	
	PfxVector3 shapeHalf(0.0f);
	PfxVector3 shapeCenter = offsetAB;
	
	switch(shapeB.getType()) {
	case kPfxShapeSphere:
		{
			shapeHalf = PfxVector3(shapeB.getSphere().m_radius);
		}
		break;
		
	case kPfxShapeCapsule:
		{
			PfxCapsule capsule = shapeB.getCapsule();
			shapeHalf = absPerElem(matrixAB) * PfxVector3(capsule.m_halfLen+capsule.m_radius,capsule.m_radius,capsule.m_radius);
		}
		break;
		
	case kPfxShapeCylinder:
		{
			PfxCylinder cylinder = shapeB.getCylinder();
			shapeHalf = absPerElem(matrixAB) * PfxVector3(cylinder.m_halfLen,cylinder.m_radius,cylinder.m_radius);
		}
		break;
		
	case kPfxShapeBox:
		{
			shapeHalf = absPerElem(matrixAB) * shapeB.getBox().m_half;
		}
		break;

	case kPfxShapeConvexMesh:
		{
			shapeHalf = absPerElem(matrixAB) * shapeB.getConvexMesh()->m_half;
		}
		break;
		
	default:break;
	}

	// -----------------------------------------------------
	// アイランドとの衝突判定

	PfxVecInt3 aabbMinL,aabbMaxL;
	lmeshA->getLocalPosition((shapeCenter-shapeHalf),(shapeCenter+shapeHalf),aabbMinL,aabbMaxL);

	PfxAabb16 aabbA;
	
	pfxSetXMin(aabbA,aabbMinL.getX());
	pfxSetXMax(aabbA,aabbMaxL.getX());
	pfxSetYMin(aabbA,aabbMinL.getY());
	pfxSetYMax(aabbA,aabbMaxL.getY());
	pfxSetZMin(aabbA,aabbMinL.getZ());
	pfxSetZMax(aabbA,aabbMaxL.getZ());
	
	{
		PfxBvhStack<PfxAabb16> bvhStack;
		
		bvhStack.push(lmeshA->m_bvhNodes[lmeshA->m_bvhRootId]);
		
		while(!bvhStack.isEmpty()) {
			PfxAabb16 aabbB = bvhStack.pop();
			
			if(pfxGetXMax(aabbA) < pfxGetXMin(aabbB) || pfxGetXMin(aabbA) > pfxGetXMax(aabbB)) continue;
			if(pfxGetYMax(aabbA) < pfxGetYMin(aabbB) || pfxGetYMin(aabbA) > pfxGetYMax(aabbB)) continue;
			if(pfxGetZMax(aabbA) < pfxGetZMin(aabbB) || pfxGetZMin(aabbA) > pfxGetZMax(aabbB)) continue;
			
			PfxUInt32 flag = aabbB.get32(3) & 0xc0000000;
			PfxUInt32 packedId = aabbB.get32(3) & 0x3fffffff;
			PfxUInt32 nodeId = (packedId&0x3ff00000)>>20;
			PfxUInt32 leftId = (packedId&0x000ffc00)>>10;
			PfxUInt32 rightId = packedId&0x000003ff;

			if(flag != 0) {
				bvhStack.push(lmeshA->m_bvhNodes[rightId]);
				bvhStack.push(lmeshA->m_bvhNodes[leftId]);
				continue;
			}
			
			PfxUInt32 i = nodeId;
			
			// 衝突判定
			PfxContactCache localContacts;
			
			if(lmeshA->m_type & 0x01) {
				switch(shapeB.getType()) {
					case kPfxShapeSphere:
					pfxContactTriMeshSphere(localContacts,lmeshA,(PfxQuantizedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getSphere(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeCapsule:
					pfxContactTriMeshCapsule(localContacts,lmeshA,(PfxQuantizedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getCapsule(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeBox:
					pfxContactTriMeshBox(localContacts,lmeshA,(PfxQuantizedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getBox(),transformAB,distanceThreshold);
					break;
					
					case kPfxShapeCylinder:
					pfxContactTriMeshCylinder(localContacts,lmeshA,(PfxQuantizedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getCylinder(),transformAB,distanceThreshold);
					break;

					case kPfxShapeConvexMesh:
					pfxContactTriMeshConvex(localContacts,lmeshA,(PfxQuantizedTriMeshBvh*)lmeshA->m_islands + i,*shapeB.getConvexMesh(),transformAB,distanceThreshold);
					break;
	
					default:
					break;
				}
			}
			else {
				switch(shapeB.getType()) {
					case kPfxShapeSphere:
					pfxContactTriMeshSphere(localContacts,lmeshA,(PfxExpandedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getSphere(),transformAB,distanceThreshold);
					break;

					case kPfxShapeCapsule:
					pfxContactTriMeshCapsule(localContacts,lmeshA,(PfxExpandedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getCapsule(),transformAB,distanceThreshold);
					break;

					case kPfxShapeBox:
					pfxContactTriMeshBox(localContacts,lmeshA,(PfxExpandedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getBox(),transformAB,distanceThreshold);
					break;

					case kPfxShapeCylinder:
					pfxContactTriMeshCylinder(localContacts,lmeshA,(PfxExpandedTriMeshBvh*)lmeshA->m_islands + i,shapeB.getCylinder(),transformAB,distanceThreshold);
					break;

					case kPfxShapeConvexMesh:
					pfxContactTriMeshConvex(localContacts,lmeshA,(PfxExpandedTriMeshBvh*)lmeshA->m_islands + i,*shapeB.getConvexMesh(),transformAB,distanceThreshold);
					break;

					default:
					break;
				}
			}
			
			// 衝突点を追加
			for(int j=0;j<localContacts.getNumContacts();j++) {
				PfxSubData subData = localContacts.getSubData(j);
				subData.setIslandId(i);
				contacts.addContactPoint(
					localContacts.getDistance(j),
					transformA.getUpper3x3() * localContacts.getNormal(j),
					localContacts.getLocalPointA(j),
					transformBA * localContacts.getLocalPointB(j),
					subData);
			}
		}
	}

	return contacts.getNumContacts();
}


} //namespace PhysicsEffects
} //namespace sce
