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

namespace sce {
namespace PhysicsEffects {

struct PfxDecodedFacet
{
	PfxFloat m_thickness;
	SCE_PFX_PADDING(1,12)
	PfxVector3 m_normal;
};

struct PfxDecodedTriMesh
{
	PfxVector3 m_verts[SCE_PFX_NUMMESHVERTICES];

	PfxUInt32 m_decodedVertex[(SCE_PFX_NUMMESHVERTICES+31)/32];
	
	PfxBool isDecodedVertex(PfxUInt32 i)
	{
		return (m_decodedVertex[i>>5] & (1<<(i&0x1f))) != 0;
	}
	
	PfxDecodedTriMesh()
	{
		for(int i=0;i<(SCE_PFX_NUMMESHVERTICES+31)/32;i++) {
			m_decodedVertex[i] = 0;
		}
	}
};

} // namespace PhysicsEffects
} // namespace sce

