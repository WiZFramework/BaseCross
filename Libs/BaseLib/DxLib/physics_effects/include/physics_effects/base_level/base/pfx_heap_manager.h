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

#include "pfx_common.h"

//J プールされたメモリを管理するスタックのサイズ
//E Size of a stack which used to manage pool memory
#define SCE_PFX_HEAP_STACK_SIZE 64

#define SCE_PFX_MIN_ALLOC_SIZE 16

#define SCE_PFX_ALLOC_BYTES_ALIGN16(bytes) SCE_PFX_MAX(16,SCE_PFX_BYTES_ALIGN16(bytes))
#define SCE_PFX_ALLOC_BYTES_ALIGN128(bytes) SCE_PFX_MAX(128,SCE_PFX_BYTES_ALIGN128(bytes))

///////////////////////////////////////////////////////////////////////////////
// PfxHeapManager

//J ＜補足＞
//J メモリはスタックで管理されています。取得した順と逆に開放する必要があります。
//J メモリを一気に開放したい場合はclear()を呼び出してください。
//J 最小割り当てサイズはSCE_PFX_MIN_ALLOC_SIZEで定義されます。

//E <Notes>
//E Memory is managed as a stack, so deallocate() needs to be called in reverse order.
//E Use clear() to deallocate all allocated memory at once.
//E SCE_PFX_MIN_ALLOC_SIZE defines the smallest amount of buffer.

namespace sce {
namespace PhysicsEffects {

class PfxHeapManager
{
private:
	PfxUInt8 *m_heap;
	PfxUInt8 *m_poolStack[SCE_PFX_HEAP_STACK_SIZE];
	PfxInt32 m_heapBytes;
	PfxInt32 m_curStack;
	PfxInt32 m_rest;
	
public:
	enum {ALIGN16=16,ALIGN128=128};

	PfxHeapManager(PfxUInt8 *buf,PfxInt32 bytes)
	{
		m_heap = buf;
		m_heapBytes = bytes;
		clear();
	}
	
	~PfxHeapManager()
	{
	}
	
	PfxInt32 getAllocated()
	{
		return (PfxInt32)(m_poolStack[m_curStack]-m_heap);
	}
	
	PfxInt32 getRest()
	{
		return m_heapBytes-getAllocated();
	}

	void *allocate(size_t bytes,PfxInt32 alignment = ALIGN16)
	{
	SCE_PFX_ALWAYS_ASSERT(m_curStack<SCE_PFX_HEAP_STACK_SIZE-1);

	bytes = SCE_PFX_MAX(bytes,SCE_PFX_MIN_ALLOC_SIZE);

	uintptr_t p = (uintptr_t)m_poolStack[m_curStack];

	if(alignment == ALIGN128) {
		p = (p+127) & ~127;
		bytes = (bytes+127) & ~127;
	}
	else {
		p = (p+15) & ~15;
		bytes = (bytes+15) & ~15;
	}

	SCE_PFX_ALWAYS_ASSERT_MSG(bytes <= (m_heapBytes-(p-(uintptr_t)m_heap)),"Memory overflow");

	m_poolStack[++m_curStack] = (PfxUInt8 *)(p + bytes);

	m_rest = getRest();

	return (void*)p;
	}

	void deallocate(void *p)
	{
#if 0
		m_curStack--;
		PfxInt32 addr = (PfxInt32)m_poolStack[m_curStack];
		SCE_PFX_ALWAYS_ASSERT_MSG(addr == (PfxInt32)p || ((addr+127) & 0xffffff80) == (PfxInt32)p,"Stack overflow");
#else
		(void) p;
		m_curStack--;
#endif
	}
	
	void clear()
	{
		m_poolStack[0] = m_heap;
		m_curStack = 0;
		m_rest = 0;
	}

	void printStack()
	{
		SCE_PFX_PRINTF("memStack %d/%d\n",m_curStack,SCE_PFX_HEAP_STACK_SIZE);
	}
};

} //namespace PhysicsEffects
} //namespace sce

