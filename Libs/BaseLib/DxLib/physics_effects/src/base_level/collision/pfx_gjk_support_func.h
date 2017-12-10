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

#include "../../../include/physics_effects/base_level/base/pfx_common.h"

// Support func + Margin
void pfxGetSupportVertexTriangle(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexTriangleWithThickness(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexConvex(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexBox(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexCapsule(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexSphere(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexCylinder(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);

// Support func + No Margin
void pfxGetSupportVertexTriangleNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexTriangleWithThicknessNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexConvexNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexBoxNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexCapsuleNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexSphereNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);
void pfxGetSupportVertexCylinderNoMargin(void *shape,const PfxVector3 &seperatingAxis,PfxVector3 &supportVertex);

// Center
void pfxGetCenterTriangle(void *shape,PfxVector3 &center);
void pfxGetCenterTriangleWithThickness(void *shape,PfxVector3 &center);
void pfxGetCenterConvex(void *shape,PfxVector3 &center);
void pfxGetCenterBox(void *shape,PfxVector3 &center);
void pfxGetCenterCapsule(void *shape,PfxVector3 &center);
void pfxGetCenterSphere(void *shape,PfxVector3 &center);
void pfxGetCenterCylinder(void *shape,PfxVector3 &center);

} //namespace PhysicsEffects
} //namespace sce

