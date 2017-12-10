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

#include "../../physics_effects/base_level/base/pfx_common.h"

namespace sce{
namespace PhysicsEffects{

///////////////////////////////////////////////////////////////////////////////
// PfxPoolQueue

template <class T>
class PfxPoolQueue
{
private:
	PfxHeapManager *m_pool;
	PfxUInt32 m_numData;
	PfxUInt32 m_maxData;
	PfxUInt32 m_head;
	PfxUInt32 m_tail;
	T SCE_PFX_ALIGNED(16) *m_data;
	SCE_PFX_PADDING(1,12)
	
	PfxPoolQueue() {}
	
public:
	
	PfxPoolQueue(PfxHeapManager *pool,PfxUInt32 maxData);
	~PfxPoolQueue();
	
	PfxUInt32 size() const {return m_numData;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& front();
	
	inline const T& front() const;
	
	inline bool empty() const {return m_numData==0;}
	
	inline void clear() {m_numData=0;m_head=0;m_tail=0;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxPoolPriorityQueue

template <class T>
class PfxPoolPriorityQueue
{
private:
	PfxHeapManager *m_pool;
	PfxUInt32 m_numData;
	PfxUInt32 m_maxData;
	T SCE_PFX_ALIGNED(16) *m_data;
	SCE_PFX_PADDING(1,12)
	
	PfxPoolPriorityQueue() {}
	
	void upHeap(PfxUInt32 i);
	void downHeap(PfxUInt32 i);
	
public:
	PfxPoolPriorityQueue(PfxHeapManager *pool,PfxUInt32 maxData);
	~PfxPoolPriorityQueue();
	
	PfxUInt32 size() const {return m_numData-1;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& front();
	
	inline const T& front() const;
	
	inline bool empty() const {return m_numData==1;}
	
	inline void clear() {m_numData=1;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxPoolStack

template <class T>
class PfxPoolStack
{
private:
	PfxHeapManager *m_pool;
	PfxUInt32 m_numData;
	PfxUInt32 m_maxData;

	SCE_PFX_PADDING(1,4)

	T SCE_PFX_ALIGNED(16) *m_data;

	SCE_PFX_PADDING(2,12)

	PfxPoolStack() {}
	
public:

	inline PfxPoolStack(PfxHeapManager *pool,PfxUInt32 maxData);
	inline ~PfxPoolStack();
	
	PfxUInt32 size() const {return m_numData;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& top();
	inline const T& top() const;
	
	inline bool empty() const {return m_numData==0;}
	
	inline void clear() {m_numData = 0;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxPoolArray

template <class T>
class PfxPoolArray
{
private:
	PfxHeapManager *m_pool;
	PfxUInt32 m_numData;
	const PfxUInt32 m_maxData;
	PfxUInt32 m_dataLength;
	T SCE_PFX_ALIGNED(16) *m_data;
	
	PfxUInt32 *m_poolTable;

	SCE_PFX_PADDING(1,8)

	PfxPoolStack<PfxUInt32> m_poolId;
	
public:
	inline PfxPoolArray(PfxHeapManager *pool,PfxUInt32 maxData);
	inline ~PfxPoolArray();
	
	PfxUInt32 size() const {return m_numData;}
	PfxUInt32 capacity() const {return m_maxData;}
	
	inline PfxUInt32 length() const {return m_dataLength;}
	
	inline T& operator[](PfxUInt32 i);
	
	inline const T& operator[](PfxUInt32 i) const;
	
	inline void assign(PfxUInt32 num,const T &initData);

	inline void assign(PfxUInt32 num);
	
	inline PfxUInt32 push(const T& data);
	
	inline bool remove(PfxUInt32 i);

	inline bool isRemoved(PfxUInt32 i) const;

	inline bool empty() const {return m_numData==0;}
	
	inline void clear();

	inline T* ptr() {return m_data;}
};

#include "pfx_pool_array_implementation.h"

} // namespace PhysicsEffects
} // namespace sce

