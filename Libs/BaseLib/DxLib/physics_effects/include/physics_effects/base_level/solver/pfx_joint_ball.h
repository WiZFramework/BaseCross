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

#include "pfx_joint.h"
#include "pfx_solver_body.h"

namespace sce {
namespace PhysicsEffects {

struct PfxBallJointInitParam {
	PfxVector3 anchorPoint;
	
	PfxBallJointInitParam()
	{
		anchorPoint = PfxVector3(0.0f);
	}
};

PfxInt32 pfxInitializeBallJoint(
	PfxJoint &joint,
	const PfxRigidState &stateA,
	const PfxRigidState &stateB,
	const PfxBallJointInitParam &param);

void pfxSetupBallJoint(
	PfxJoint &joint,
	const PfxRigidState &stateA,
	const PfxRigidState &stateB,
	PfxSolverBody &solverBodyA,
	PfxSolverBody &solverBodyB,
	PfxFloat timeStep);

void pfxWarmStartBallJoint(
	PfxJoint &joint,
	PfxSolverBody &solverBodyA,
	PfxSolverBody &solverBodyB);

void pfxSolveBallJoint(
	PfxJoint &joint,
	PfxSolverBody &solverBodyA,
	PfxSolverBody &solverBodyB);

} //namespace PhysicsEffects
} //namespace sce

