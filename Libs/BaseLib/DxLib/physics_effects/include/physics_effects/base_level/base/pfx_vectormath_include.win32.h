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

// vectormath include
#include "../../../../../vectormath/vectormath_aos.h"
#include "../../../../../vectormath/floatInVec.h"
#define SCE_VECTORMATH_AOS_VECTOR_ARG
#define SCE_VECTORMATH_AOS_MATRIX_ARG

namespace sce {
namespace PhysicsEffects {
typedef Vectormath::Aos::Point3     PfxPoint3;
typedef Vectormath::Aos::Vector3    PfxVector3;
typedef Vectormath::Aos::Vector4    PfxVector4;
typedef Vectormath::Aos::Quat       PfxQuat;
typedef Vectormath::Aos::Matrix3    PfxMatrix3;
typedef Vectormath::Aos::Matrix4    PfxMatrix4;
typedef Vectormath::Aos::Transform3 PfxTransform3;
typedef Vectormath::floatInVec		PfxFloatInVec;
typedef Vectormath::boolInVec		PfxBoolInVec;
} //namespace PhysicsEffects
} //namespace sce

