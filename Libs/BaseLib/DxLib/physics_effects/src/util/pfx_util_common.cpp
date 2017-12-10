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

#include "../../include/physics_effects/util/pfx_util_common.h"
#include "../../include/physics_effects/base_level/base/pfx_common.h"

namespace sce {
namespace PhysicsEffects {

#ifdef _WIN32
	#include <malloc.h>
	SCE_PFX_FUNC_ALLOC		palloc_func		= _aligned_malloc;
	SCE_PFX_FUNC_REALLOC	prealloc_func	= _aligned_realloc;
	SCE_PFX_FUNC_FREE		pfree_func		= _aligned_free;
#else
	SCE_PFX_FUNC_ALLOC		palloc_func		= memalign;
	SCE_PFX_FUNC_REALLOC	prealloc_func	= reallocalign;
	SCE_PFX_FUNC_FREE		pfree_func		= free;
#endif

void pfxSetUtilityAllocator(SCE_PFX_FUNC_ALLOC func_alloc, SCE_PFX_FUNC_REALLOC func_realloc, SCE_PFX_FUNC_FREE func_free)
{
	SCE_PFX_ASSERT(func_alloc&&func_realloc&&func_free);
	palloc_func		= func_alloc;
	prealloc_func	= func_realloc;
	pfree_func		= func_free;
}

} //namespace PhysicsEffects
} //namespace sce
