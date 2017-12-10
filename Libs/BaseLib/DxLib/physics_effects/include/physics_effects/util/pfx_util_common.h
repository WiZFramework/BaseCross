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

#include <string.h>

typedef void* (*SCE_PFX_FUNC_ALLOC)(size_t align, size_t size);
typedef void* (*SCE_PFX_FUNC_REALLOC)(void* ptr, size_t align, size_t size);
typedef void  (*SCE_PFX_FUNC_FREE)(void* ptr);

namespace sce
{
namespace PhysicsEffects
{

extern SCE_PFX_FUNC_ALLOC	palloc_func;
extern SCE_PFX_FUNC_REALLOC	prealloc_func;
extern SCE_PFX_FUNC_FREE	pfree_func;

/**
 * set the custom allocators for PhysicsEffects::Util
 * 
 * @param func_alloc a pointer to the func that alocate some memories with using a proper boundary
 * @param func_realloc
 *                   a pointer to the func that re-alocate the memory with using a proper boundary
 * @param func_free  a pointer to the func that free the memory
 */
void pfxSetUtilityAllocator(SCE_PFX_FUNC_ALLOC func_alloc, SCE_PFX_FUNC_REALLOC func_realloc, SCE_PFX_FUNC_FREE func_free);

}
}

#ifdef _WIN32
	#define SCE_PFX_UTIL_ALLOC(align,size) 			sce::PhysicsEffects::palloc_func(size,align)
	#define SCE_PFX_UTIL_REALLOC(ptr,align,size)	sce::PhysicsEffects::prealloc_func(ptr,size,align)
	#define SCE_PFX_UTIL_FREE(ptr) 					if(ptr){sce::PhysicsEffects::pfree_func(ptr);ptr=NULL;}
#else
	#define SCE_PFX_UTIL_ALLOC(align,size) 			sce::PhysicsEffects::palloc_func(align,size)
	#define SCE_PFX_UTIL_REALLOC(ptr,align,size) 	sce::PhysicsEffects::prealloc_func(ptr,size,align)
	#define SCE_PFX_UTIL_FREE(ptr) 					if(ptr){sce::PhysicsEffects::pfree_func(ptr);ptr=NULL;}
#endif

