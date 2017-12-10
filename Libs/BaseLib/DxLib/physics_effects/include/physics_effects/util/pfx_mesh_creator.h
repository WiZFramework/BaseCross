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

#include "../base_level/collision/pfx_large_tri_mesh.h"

namespace sce {
namespace PhysicsEffects {

//J フラグに指定する値
//E Specify these values to a flag parameter
#define SCE_PFX_MESH_FLAG_NORMAL_FLIP		0x01
#define SCE_PFX_MESH_FLAG_16BIT_INDEX		0x02
#define SCE_PFX_MESH_FLAG_32BIT_INDEX		0x04
#define SCE_PFX_MESH_FLAG_AUTO_ELIMINATION	0x08
#define SCE_PFX_MESH_FLAG_AUTO_THICKNESS	0x10
#define SCE_PFX_MESH_FLAG_USE_BVH			0x20
#define SCE_PFX_MESH_FLAG_USE_QUANTIZED		0x40
#define SCE_PFX_MESH_FLAG_OUTPUT_INFO		0x80

///////////////////////////////////////////////////////////////////////////////
// Convex Mesh

struct PfxCreateConvexMeshParam {
	PfxUInt32	flag;
	PfxFloat*	verts;
	PfxUInt32	numVerts;
	void*		triangles;
	PfxUInt32	numTriangles;
	PfxUInt32	vertexStrideBytes;
	PfxUInt32	triangleStrideBytes;
	
	PfxCreateConvexMeshParam()
	{
		flag = SCE_PFX_MESH_FLAG_16BIT_INDEX|SCE_PFX_MESH_FLAG_AUTO_ELIMINATION;
		verts = NULL;
		triangles = NULL;
		numVerts = 0;
		numTriangles = 0;
		vertexStrideBytes = sizeof(PfxFloat)*3;
		triangleStrideBytes = sizeof(PfxUInt16)*3;
	}
};

PfxInt32 pfxCreateConvexMesh(PfxConvexMesh &convex,const PfxCreateConvexMeshParam &param);

void pfxReleaseConvexMesh(PfxConvexMesh &cmesh);

///////////////////////////////////////////////////////////////////////////////
// Large Mesh

struct PfxCreateLargeTriMeshParam {
	PfxUInt32 flag;
	PfxFloat *verts;
	PfxUInt32 numVerts;
	void *triangles;
	PfxUInt32 *userData;
	PfxUInt32 numTriangles;
	PfxUInt32 vertexStrideBytes;
	PfxUInt32 triangleStrideBytes;
	PfxUInt32 numFacetsLimit;
	PfxFloat islandsRatio;
	PfxFloat defaultThickness;
	
	PfxCreateLargeTriMeshParam()
	{
		flag = SCE_PFX_MESH_FLAG_16BIT_INDEX|SCE_PFX_MESH_FLAG_AUTO_ELIMINATION|SCE_PFX_MESH_FLAG_OUTPUT_INFO;
		verts = NULL;
		triangles = NULL;
		userData = 0;
		numVerts = 0;
		numTriangles = 0;
		vertexStrideBytes = sizeof(PfxFloat)*3;
		triangleStrideBytes = sizeof(PfxUInt16)*3;
		numFacetsLimit = 15;
		islandsRatio = 0.2f;
		defaultThickness = 0.025f;
	}
};

PfxInt32 pfxCreateLargeTriMesh(PfxLargeTriMesh &lmesh,const PfxCreateLargeTriMeshParam &param);

void pfxReleaseLargeTriMesh(PfxLargeTriMesh &lmesh);

} //namespace PhysicsEffects
} //namespace sce

