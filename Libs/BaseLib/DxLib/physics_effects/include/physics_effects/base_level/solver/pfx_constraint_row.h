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

namespace sce {
namespace PhysicsEffects {

#include "../base/pfx_common.h"

//E Don't change following order of parameters
struct SCE_PFX_ALIGNED(16) PfxConstraintRow {
	PfxFloat m_normal[3];
	PfxFloat m_rhs;
	PfxFloat m_jacDiagInv;
	PfxFloat m_lowerLimit;
	PfxFloat m_upperLimit;
	PfxFloat m_accumImpulse;
};

} //namespace PhysicsEffects
} //namespace sce

