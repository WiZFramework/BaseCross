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

struct PfxSubData {
	enum {
		NONE = 0,
		MESH_INFO
	};

	union {
		struct {
			struct {
				PfxUInt32 userData;
				PfxUInt16 islandId;
				PfxUInt16 s;
				PfxUInt16 t;
				PfxUInt8  facetId;
				SCE_PFX_PADDING(1,1)
			} m_facetLocal;
			PfxUInt8  m_type;
			SCE_PFX_PADDING(2,3)
		};
		PfxUInt32 param[3];
	};

	PfxSubData()
	{
		param[0] = param[1] = param[2] = 0;
	}

	void setIslandId(PfxUInt16 i) {m_facetLocal.islandId = i;}
	void setFacetId(PfxUInt8 i) {m_facetLocal.facetId = i;}
	void setFacetLocalS(PfxFloat s) {m_facetLocal.s = (PfxUInt16)(SCE_PFX_CLAMP(s,0.0f,1.0f) * 65535.0f);}
	void setFacetLocalT(PfxFloat t) {m_facetLocal.t = (PfxUInt16)(SCE_PFX_CLAMP(t,0.0f,1.0f) * 65535.0f);}
	void setUserData(PfxUInt32 data) {m_facetLocal.userData = data;}

	PfxUInt16 getIslandId() {return m_facetLocal.islandId;}
	PfxUInt8 getFacetId() {return m_facetLocal.facetId;}
	PfxFloat getFacetLocalS() {return m_facetLocal.s / 65535.0f;}
	PfxFloat getFacetLocalT() {return m_facetLocal.t / 65535.0f;}
	PfxUInt32 getUserData() {return m_facetLocal.userData;}
};

} //namespace PhysicsEffects
} //namespace sce

