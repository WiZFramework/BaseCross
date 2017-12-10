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

#include "../../../include/physics_effects/base_level/collision/pfx_large_tri_mesh.h"
#include "../../../include/physics_effects/base_level/collision/pfx_cylinder.h"
#include "pfx_contact_cache.h"

namespace sce {
namespace PhysicsEffects {

PfxInt32 pfxContactTriMeshCylinder(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMesh *meshA,
	const PfxCylinder &cylinderB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold = SCE_PFX_FLT_MAX );

PfxInt32 pfxContactTriMeshCylinder(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMesh *meshA,
	const PfxCylinder &cylinderB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold = SCE_PFX_FLT_MAX );

PfxInt32 pfxContactTriMeshCylinder(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxQuantizedTriMeshBvh *meshA,
	const PfxCylinder &cylinderB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold = SCE_PFX_FLT_MAX );

PfxInt32 pfxContactTriMeshCylinder(
	PfxContactCache &contacts,
	const PfxLargeTriMesh *largeMeshA,const PfxExpandedTriMeshBvh *meshA,
	const PfxCylinder &cylinderB,
	const PfxTransform3 &transformB,
	PfxFloat distanceThreshold = SCE_PFX_FLT_MAX );

} //namespace PhysicsEffects
} //namespace sce

