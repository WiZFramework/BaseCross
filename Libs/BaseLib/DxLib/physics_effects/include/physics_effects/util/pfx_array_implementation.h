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

///////////////////////////////////////////////////////////////////////////////
// PfxArray

namespace sce {
namespace PhysicsEffects {
template <class T>
inline T& PfxArray<T>::operator[](PfxUInt32 i)
{
	SCE_PFX_ASSERT (i < m_maxData);
	return m_data[i];
}

template <class T>
inline const T& PfxArray<T>::operator[](PfxUInt32 i) const
{
	SCE_PFX_ASSERT (i < m_maxData);
	return m_data[i];
}

template <class T>
inline const PfxArray<T>& PfxArray<T>::operator=(const PfxArray &array)
{
	clear();
	if(array.size() > m_maxData) {
		m_maxData = array.size();
		m_data = (T*)SCE_PFX_UTIL_REALLOC(m_data,16,sizeof(T)*m_maxData);
		SCE_PFX_ASSERT(m_data);
	}
	for(PfxUInt32 i=0;i<array.size();i++) {
		push(array[i]);
	}
	return *this;
}

template <class T>
inline void PfxArray<T>::assign(PfxUInt32 num,const T &initData)
{
	clear();
	if(num > m_maxData) {
		m_maxData = num;
		m_data = (T*)SCE_PFX_UTIL_REALLOC(m_data,16,sizeof(T)*m_maxData);
		SCE_PFX_ASSERT(m_data);
	}
	for(PfxUInt32 i=0;i<num;i++) {
		push(initData);
	}
}
template <class T>
inline void PfxArray<T>::assign(PfxUInt32 num)
{
	clear();
	SCE_PFX_ASSERT(num<=m_maxData);

	m_numData = num;
}

template <class T>
inline PfxUInt32 PfxArray<T>::push(const T& data)
{
	if(m_numData == m_maxData) {
		m_maxData<<=1;
		m_data = (T*)SCE_PFX_UTIL_REALLOC(m_data,16,sizeof(T)*m_maxData);
		SCE_PFX_ASSERT(m_data);
	}
	
	PfxUInt32 id = m_numData++;
	m_data[id] = data;
	return id;
}

template <class T>
inline bool PfxArray<T>::remove(PfxUInt32 i)
{
	if(i>=m_numData) {
		return false;
	}
	
	m_numData--;
	m_data[i] = m_data[m_numData];
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// PfxQueue

template <class T>
inline PfxUInt32 PfxQueue<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);
	PfxUInt32 id = m_tail;
	m_data[id] = data;
	m_tail = (m_tail+1)%m_maxData;
	m_numData++;
	return id;
}

template <class T>
inline void PfxQueue<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_head = (m_head+1)%m_maxData;
	m_numData--;
}

template <class T>
inline T& PfxQueue<T>::front()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

template <class T>
inline const T& PfxQueue<T>::front() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_head];
}

///////////////////////////////////////////////////////////////////////////////
// PfxPriorityQueue

template <class T>
void PfxPriorityQueue<T>::upHeap(PfxUInt32 i)
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
void PfxPriorityQueue<T>::downHeap(PfxUInt32 i)
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
inline PfxUInt32 PfxPriorityQueue<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);

	PfxUInt32 id = 	m_numData++;
	m_data[id] = data;
	upHeap(id);
	return id;
}

template <class T>
inline void PfxPriorityQueue<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>1);
	PfxUInt32 id = --m_numData;
	m_data[1] = m_data[id];
	downHeap(1);
}

template <class T>
inline T& PfxPriorityQueue<T>::front()
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

template <class T>
inline const T& PfxPriorityQueue<T>::front() const
{
	SCE_PFX_ASSERT(m_numData>1);
	return m_data[1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxStack

template <class T>
inline PfxUInt32 PfxStack<T>::push(const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);
	PfxUInt32 id = m_numData++;
	m_data[id] = data;
	return id;
}

template <class T>
inline void PfxStack<T>::pop()
{
	SCE_PFX_ASSERT(m_numData>0);
	m_numData--;
}

template <class T>
inline T& PfxStack<T>::top()
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

template <class T>
inline const T& PfxStack<T>::top() const
{
	SCE_PFX_ASSERT(m_numData>0);
	return m_data[m_numData-1];
}

///////////////////////////////////////////////////////////////////////////////
// PfxMap

template <class KEY, class T>
inline PfxUInt32 PfxMap<KEY,T>::getHashId(const KEY& key)
{
	const unsigned char *p = (const unsigned char *)&key;
	PfxUInt32 id = 0;
	for(int i=0;i<sizeof(KEY);i++) {
		id *= 0x39532BA9;
		id += p[i];
	}
	return id % (m_maxData==0?1:m_maxData);
}

template <class KEY, class T>
PfxMap<KEY,T>::PfxMap() : m_numData(0),m_maxData(100)
{
	m_nodes = (PfxMapNode*)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMapNode)*m_maxData);
	m_headers = (PfxMapNode**)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMapNode*)*m_maxData);
	m_data = (T*)SCE_PFX_UTIL_ALLOC(16,sizeof(T)*m_maxData);
	
	clear();
}

template <class KEY, class T>
PfxMap<KEY,T>::PfxMap(PfxUInt32 maxData) : m_numData(0),m_maxData(maxData==0?1:maxData)
{
	m_nodes = (PfxMapNode*)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMapNode)*m_maxData);
	m_headers = (PfxMapNode**)SCE_PFX_UTIL_ALLOC(16,sizeof(PfxMapNode*)*m_maxData);
	m_data = (T*)SCE_PFX_UTIL_ALLOC(16,sizeof(T)*m_maxData);
	
	clear();
}

template <class KEY, class T>
PfxMap<KEY,T>::~PfxMap()
{
	SCE_PFX_UTIL_FREE(m_data);
	SCE_PFX_UTIL_FREE(m_headers);
	SCE_PFX_UTIL_FREE(m_nodes);
}

template <class KEY, class T>
inline bool PfxMap<KEY,T>::insert(const KEY& key, const T& data)
{
	SCE_PFX_ASSERT(m_numData<m_maxData);
	
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

template <class KEY, class T>
inline bool PfxMap<KEY,T>::find(const KEY& key, const T& data) const
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

template <class KEY, class T>
inline bool PfxMap<KEY,T>::find(const KEY& key, T& data)
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

template <class KEY, class T>
inline bool PfxMap<KEY,T>::erase(const KEY& key)
{
	PfxUInt32 id = getHashId(key);
	
	for(PfxMapNode *iterator=m_headers[id];iterator!=NULL;iterator=iterator->next) {
		if(iterator->key == key) {
			//J ƒm[ƒhíœ
			//E Remove nodes
			if(iterator->prev) {
				iterator->prev->next = iterator->next;
			}
			else {
				m_headers[id] = iterator->next;
			}
			
			if(iterator->next) {
				iterator->next->prev = iterator->prev;
			}
			
			//J ƒm[ƒhˆÚ“®
			//E Move nodes
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

template <class KEY, class T>
inline void PfxMap<KEY,T>::clear()
{
	m_numData = 0;
	memset(m_nodes,0,sizeof(PfxMapNode)*m_maxData);
	memset(m_headers,0,sizeof(PfxMapNode*)*m_maxData);
	memset(m_data,0,sizeof(T)*m_maxData);
}

} //namespace PhysicsEffects
} //namespace sce

