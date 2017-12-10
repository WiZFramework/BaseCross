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

// PfxStaticQueue

template <class T,int SIZE>
inline PfxStaticQueue<T,SIZE>::PfxStaticQueue()
	 : m_numData(0),m_head(0),m_tail(0)
{
}

template <class T,int SIZE>
inline PfxStaticQueue<T,SIZE>::~PfxStaticQueue()
{
}

template <class T,int SIZE>
inline PfxUInt32 PfxStaticQueue<T,SIZE>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<SIZE);
	PfxUInt32 id = m_tail;
	m_data[id] = data;
	m_tail = (m_tail+1)%SIZE;
	m_numData++;
	return id;
}

template <class T,int SIZE>
inline void PfxStaticQueue<T,SIZE>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_head = (m_head+1)%SIZE;
	m_numData--;
}

template <class T,int SIZE>
inline T& PfxStaticQueue<T,SIZE>::front()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

template <class T,int SIZE>
inline const T& PfxStaticQueue<T,SIZE>::front() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

///////////////////////////////////////////////////////////////////////////////
// PfxStaticPriorityQueue

template <class T,int SIZE>
inline PfxStaticPriorityQueue<T,SIZE>::PfxStaticPriorityQueue()
 : m_numData(1)
{
}

template <class T,int SIZE>
inline PfxStaticPriorityQueue<T,SIZE>::~PfxStaticPriorityQueue()
{
}

template <class T,int SIZE>
void PfxStaticPriorityQueue<T,SIZE>::upHeap(PfxUInt32 i)
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

template <class T,int SIZE>
void PfxStaticPriorityQueue<T,SIZE>::downHeap(PfxUInt32 i)
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

template <class T,int SIZE>
inline PfxUInt32 PfxStaticPriorityQueue<T,SIZE>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<SIZE);

	PfxUInt32 id = 	m_numData++;
	m_data[id] = data;
	upHeap(id);
	return id;
}

template <class T,int SIZE>
inline void PfxStaticPriorityQueue<T,SIZE>::pop()
{
	SCE_PFX_ASSERT(m_numData>1);
	PfxUInt32 id = --m_numData;
	m_data[1] = m_data[id];
	downHeap(1);
}

template <class T,int SIZE>
inline T& PfxStaticPriorityQueue<T,SIZE>::front()
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

template <class T,int SIZE>
inline const T& PfxStaticPriorityQueue<T,SIZE>::front() const
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxStaticStack

template <class T,int SIZE>
inline PfxStaticStack<T,SIZE>::PfxStaticStack()
	: m_numData(0)
{
}

template <class T,int SIZE>
inline PfxStaticStack<T,SIZE>::~PfxStaticStack()
{
}

template <class T,int SIZE>
inline PfxUInt32 PfxStaticStack<T,SIZE>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<SIZE);
	PfxUInt32 id = m_numData++;
	m_data[id] = data;
	return id;
}

template <class T,int SIZE>
inline void PfxStaticStack<T,SIZE>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_numData--;
}

template <class T,int SIZE>
inline T& PfxStaticStack<T,SIZE>::top()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

template <class T,int SIZE>
inline const T& PfxStaticStack<T,SIZE>::top() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxStaticArray

template <class T,int SIZE>
inline PfxStaticArray<T,SIZE>::PfxStaticArray()
	: m_numData(0),m_dataLength(0),m_poolId()
{
	clear();
}

template <class T,int SIZE>
inline PfxStaticArray<T,SIZE>::~PfxStaticArray()
{
}

template <class T,int SIZE>
inline T& PfxStaticArray<T,SIZE>::operator[](PfxUInt32 i)
{
	SCE_PFX_ASSERT(i < SIZE);
#ifdef SCE_PFX_STATIC_ARRAY_ASSERT_REMOVED_DATA
	SCE_PFX_ASSERT((m_poolTable[i>>5]&(1<<(i&31)))>0);
#endif
	return m_data[i];
}

template <class T,int SIZE>
inline const T& PfxStaticArray<T,SIZE>::operator[](PfxUInt32 i) const
{
	SCE_PFX_ASSERT(i < SIZE);
#ifdef SCE_PFX_STATIC_ARRAY_ASSERT_REMOVED_DATA
	SCE_PFX_ASSERT((m_poolTable[i>>5]&(1<<(i&31)))>0);
#endif
	return m_data[i];
}

template <class T,int SIZE>
inline void PfxStaticArray<T,SIZE>::assign(PfxUInt32 num,const T &initData)
{
	clear();
	
	SCE_PFX_ALWAYS_ASSERT(num <= SIZE);
	
	for(PfxUInt32 i=0;i<num;i++) {
		push(initData);
	}
}
template <class T,int SIZE>
inline void PfxStaticArray<T,SIZE>::assign(PfxUInt32 num)
{
	clear();

	SCE_PFX_ALWAYS_ASSERT(num <= SIZE);

	for(PfxUInt32 i=0;i<num;i++) {
		SCE_PFX_ALWAYS_ASSERT(!m_poolId.empty());
		PfxUInt32 id = m_poolId.top();
		m_poolId.pop();
		m_poolTable[id>>5] |= (1<<(id&31));
		if(id == m_dataLength) m_dataLength++;
		m_numData++;
	}
	
}

template <class T,int SIZE>
inline PfxUInt32 PfxStaticArray<T,SIZE>::push(const T& data)
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

template <class T,int SIZE>
inline bool PfxStaticArray<T,SIZE>::remove(PfxUInt32 i)
{
	if(i>=m_dataLength || (m_poolTable[i>>5]&(1<<(i&31)))==0) {
		return false;
	}
	
	m_poolId.push(i);
	m_poolTable[i>>5] &= ~(1<<(i&31));
	m_numData--;
	
	return true;
}

template <class T,int SIZE>
inline bool PfxStaticArray<T,SIZE>::isRemoved(PfxUInt32 i) const
{
	return (m_poolTable[i>>5]&(1<<(i&31))) == 0;
}

template <class T,int SIZE>
inline void PfxStaticArray<T,SIZE>::clear()
{
	m_poolId.clear();
	m_numData = m_dataLength = 0;
	for(PfxUInt32 i=0;i<SIZE;i++) {
		PfxUInt32 id = SIZE-1-i;
		m_poolId.push(id);
	}
	memset(m_poolTable,0,sizeof(PfxUInt32)*((SIZE+31)>>5));
}

///////////////////////////////////////////////////////////////////////////////
// PfxStaticMap

template <class KEY,class T,int SIZE>
inline PfxUInt32 PfxStaticMap<KEY,T,SIZE>::getHashId(const KEY& key)
{
	const unsigned char *p = (const unsigned char *)&key;
	PfxUInt32 id = 0;
	for(int i=0;i<sizeof(KEY);i++) {
		id *= 0x39532BA9;
		id += p[i];
	}
	return id % (SIZE==0?1:SIZE);
}

template <class KEY,class T,int SIZE>
inline PfxStaticMap<KEY,T,SIZE>::PfxStaticMap() : m_numData(0)
{
	clear();
}

template <class KEY,class T,int SIZE>
inline PfxStaticMap<KEY,T,SIZE>::~PfxStaticMap()
{
}

template <class KEY,class T,int SIZE>
inline bool PfxStaticMap<KEY,T,SIZE>::insert(const KEY& key, const T& data)
{
	SCE_PFX_ASSERT(m_numData<SIZE);
	
	PfxUInt32 id = getHashId(key);

	if(!m_headers[id]) {
		int pos = m_numData++;
		m_data[pos] = data;
		PfxMapNode &newNode = m_nodes[pos];
		newNode.key = key;
		newNode.data = m_data + pos;
		newNode.prev = NULL;
		newNode.next = NULL;
		m_headers[id] = &newNode;
		return true;
	}
	
	for(PfxMapNode *iterator=m_headers[id];iterator;iterator=iterator->next) {
		if(*(iterator->data) == data) {
			return false;
		}
	}
	
	int pos = m_numData++;
	m_data[pos] = data;
	PfxMapNode &newNode = m_nodes[pos];
	newNode.key = key;
	newNode.data = m_data + pos;
	newNode.prev = NULL;
	newNode.next = m_headers[id];
	m_headers[id]->prev = &newNode;
	m_headers[id] = &newNode;
	return true;
}

template <class KEY,class T,int SIZE>
inline bool PfxStaticMap<KEY,T,SIZE>::find(const KEY& key, const T& data) const
{
	PfxUInt32 id = getHashId(key);
	
	for(PfxMapNode *iterator=m_headers[id];iterator!=NULL;iterator=iterator->next) {
		if(iterator->key == key) {
			data = *(iterator->data);
			return true;
		}
	}
	
	return false;
}

template <class KEY,class T,int SIZE>
inline bool PfxStaticMap<KEY,T,SIZE>::find(const KEY& key, T& data)
{
	PfxUInt32 id = getHashId(key);
	
	for(PfxMapNode *iterator=m_headers[id];iterator!=NULL;iterator=iterator->next) {
		if(iterator->key == key) {
			data = *(iterator->data);
			return true;
		}
	}
	
	return false;
}

template <class KEY,class T,int SIZE>
inline bool PfxStaticMap<KEY,T,SIZE>::erase(const KEY& key)
{
	PfxUInt32 id = getHashId(key);
	
	for(PfxMapNode *iterator=m_headers[id];iterator!=NULL;iterator=iterator->next) {
		if(iterator->key == key) {
			// ƒm[ƒhíœ
			if(iterator->prev) {
				iterator->prev->next = iterator->next;
			}
			else {
				m_headers[id] = iterator->next;
			}
			
			if(iterator->next) {
				iterator->next->prev = iterator->prev;
			}
			
			// ƒm[ƒhˆÚ“®
			int pos = m_numData-1;
			PfxMapNode *lastNode = &m_nodes[pos];
			if(iterator != lastNode) {
				if(lastNode->prev) {
					lastNode->prev->next = iterator;
				}
				else {
					PfxUInt32 lastId =getHashId(lastNode->key);
					m_headers[lastId] = iterator;
				}
				
				if(lastNode->next) {
					lastNode->next->prev = iterator;
				}
				
				T *data = iterator->data;
				*(data) = m_data[pos];
				*iterator = *lastNode;
				iterator->data = data;
			}
			
			m_numData--;
			
			return true;
		}
	}

	return false;
}

template <class KEY,class T,int SIZE>
inline void PfxStaticMap<KEY,T,SIZE>::clear()
{
	m_numData = 0;
	memset(m_nodes,0,sizeof(PfxMapNode)*SIZE);
	memset(m_headers,0,sizeof(PfxMapNode*)*SIZE);
	memset(m_data,0,sizeof(T)*SIZE);
}

