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

///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"

// PfxPoolQueue

template <class T>
inline PfxPoolQueue<T>::PfxPoolQueue(PfxHeapManager *pool,PfxUInt32 maxData)
	 : m_pool(pool),m_numData(0),m_maxData(maxData),m_head(0),m_tail(0)
{
	m_data = (T*)m_pool->allocate(sizeof(T)*m_maxData);
}

template <class T>
inline PfxPoolQueue<T>::~PfxPoolQueue()
{
	m_pool->deallocate(m_data);
}

template <class T>
inline PfxUInt32 PfxPoolQueue<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);
	PfxUInt32 id = m_tail;
	m_data[id] = data;
	m_tail = (m_tail+1)%m_maxData;
	m_numData++;
	return id;
}

template <class T>
inline void PfxPoolQueue<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_head = (m_head+1)%m_maxData;
	m_numData--;
}

template <class T>
inline T& PfxPoolQueue<T>::front()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

template <class T>
inline const T& PfxPoolQueue<T>::front() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

///////////////////////////////////////////////////////////////////////////////
// PfxPoolPriorityQueue

template <class T>
inline PfxPoolPriorityQueue<T>::PfxPoolPriorityQueue(PfxHeapManager *pool,PfxUInt32 maxData)
 : m_pool(pool),m_numData(1),m_maxData(maxData+1)
{
	m_data = (T*)m_pool->allocate(sizeof(T)*m_maxData);
}

template <class T>
inline PfxPoolPriorityQueue<T>::~PfxPoolPriorityQueue()
{
	m_pool->deallocate(m_data);
}

template <class T>
void PfxPoolPriorityQueue<T>::upHeap(PfxUInt32 i)
{
	SCE_PFX_ASSERT(i>0 && i<m_numData);
	PfxUInt32 parent = i / 2;
	while(parent > 0) {
		if(m_data[i] < m_data[parent])
			break;
		
		SCE_PFX_SWAP(T,m_data[i],m_data[parent]);
		i = parent;
		parent = i / 2;
	}
}

template <class T>
void PfxPoolPriorityQueue<T>::downHeap(PfxUInt32 i)
{
	PfxUInt32 child = i * 2;
	while(child < m_numData) {
		if((child+1) < m_numData && m_data[child] < m_data[child+1])
			child++;
		
		if(m_data[i] > m_data[child])
			break;
		
		SCE_PFX_SWAP(T,m_data[i],m_data[child]);
		i = child;
		child = i * 2;
	}
}

template <class T>
inline PfxUInt32 PfxPoolPriorityQueue<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);

	PfxUInt32 id = 	m_numData++;
	m_data[id] = data;
	upHeap(id);
	return id;
}

template <class T>
inline void PfxPoolPriorityQueue<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>1);
	PfxUInt32 id = --m_numData;
	m_data[1] = m_data[id];
	downHeap(1);
}

template <class T>
inline T& PfxPoolPriorityQueue<T>::front()
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

template <class T>
inline const T& PfxPoolPriorityQueue<T>::front() const
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxPoolStack

template <class T>
inline PfxPoolStack<T>::PfxPoolStack(PfxHeapManager *pool,PfxUInt32 maxData)
	: m_pool(pool),m_numData(0),m_maxData(maxData)
{
	m_data = (T*)m_pool->allocate(sizeof(T)*m_maxData);
}

template <class T>
inline PfxPoolStack<T>::~PfxPoolStack()
{
	m_pool->deallocate(m_data);
}

template <class T>
inline PfxUInt32 PfxPoolStack<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);
	PfxUInt32 id = m_numData++;
	m_data[id] = data;
	return id;
}

template <class T>
inline void PfxPoolStack<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_numData--;
}

template <class T>
inline T& PfxPoolStack<T>::top()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

template <class T>
inline const T& PfxPoolStack<T>::top() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxPoolArray

template <class T>
inline PfxPoolArray<T>::PfxPoolArray(PfxHeapManager *pool,PfxUInt32 maxData)
	: m_pool(pool),m_numData(0),m_maxData(maxData),m_dataLength(0),m_poolId(pool,maxData)
{
	m_data = (T*)m_pool->allocate(sizeof(T)*m_maxData,128);
	m_poolTable = (PfxUInt32*)m_pool->allocate(sizeof(PfxUInt32)*((m_maxData+31)>>5));
	clear();
}

template <class T>
inline PfxPoolArray<T>::~PfxPoolArray()
{
	m_pool->deallocate(m_poolTable);
	m_pool->deallocate(m_data);
}

template <class T>
inline T& PfxPoolArray<T>::operator[](PfxUInt32 i)
{
	SCE_PFX_ASSERT(i < m_maxData);
#ifdef SCE_PFX_POOL_ARRAY_ASSERT_REMOVED_DATA
	SCE_PFX_ASSERT((m_poolTable[i>>5]&(1<<(i&31)))>0);
#endif
	return m_data[i];
}

template <class T>
inline const T& PfxPoolArray<T>::operator[](PfxUInt32 i) const
{
	SCE_PFX_ASSERT(i < m_maxData);
#ifdef SCE_PFX_POOL_ARRAY_ASSERT_REMOVED_DATA
	SCE_PFX_ASSERT((m_poolTable[i>>5]&(1<<(i&31)))>0);
#endif
	return m_data[i];
}

template <class T>
inline void PfxPoolArray<T>::assign(PfxUInt32 num,const T &initData)
{
	clear();
	
	SCE_PFX_ALWAYS_ASSERT(num <= m_maxData);
	
	for(PfxUInt32 i=0;i<num;i++) {
		push(initData);
	}
}
template <class T>
inline void PfxPoolArray<T>::assign(PfxUInt32 num)
{
	clear();

	SCE_PFX_ALWAYS_ASSERT(num <= m_maxData);

	for(PfxUInt32 i=0;i<num;i++) {
		SCE_PFX_ALWAYS_ASSERT(!m_poolId.empty());
		PfxUInt32 id = m_poolId.top();
		m_poolId.pop();
		m_poolTable[id>>5] |= (1<<(id&31));
		if(id == m_dataLength) m_dataLength++;
		m_numData++;
	}
	
}

template <class T>
inline PfxUInt32 PfxPoolArray<T>::push(const T& data)
{
	SCE_PFX_ALWAYS_ASSERT(!m_poolId.empty());
	PfxUInt32 id = m_poolId.top();
	m_poolId.pop();
	m_data[id] = data;
	m_poolTable[id>>5] |= (1<<(id&31));
	if(id == m_dataLength) m_dataLength++;
	m_numData++;
	return id;
}

template <class T>
inline bool PfxPoolArray<T>::remove(PfxUInt32 i)
{
	if(i>=m_dataLength || (m_poolTable[i>>5]&(1<<(i&31)))==0) {
		return false;
	}
	
	m_poolId.push(i);
	m_poolTable[i>>5] &= ~(1<<(i&31));
	m_numData--;
	
	return true;
}

template <class T>
inline bool PfxPoolArray<T>::isRemoved(PfxUInt32 i) const
{
	return (m_poolTable[i>>5]&(1<<(i&31))) == 0;
}

template <class T>
inline void PfxPoolArray<T>::clear()
{
	m_poolId.clear();
	m_numData = m_dataLength = 0;
	for(PfxUInt32 i=0;i<m_maxData;i++) {
		PfxUInt32 id = m_maxData-1-i;
		m_poolId.push(id);
	}
	memset(m_poolTable,0,sizeof(PfxUInt32)*((m_maxData+31)>>5));
}

