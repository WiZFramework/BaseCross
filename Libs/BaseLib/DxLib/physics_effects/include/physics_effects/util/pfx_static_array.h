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

#define SCE_PFX_STATIC_MAX 100

namespace sce{
namespace PhysicsEffects{

///////////////////////////////////////////////////////////////////////////////
// PfxStaticQueue

template <class T,int SIZE = SCE_PFX_STATIC_MAX>
class PfxStaticQueue
{
private:
	PfxUInt32 m_numData;
	PfxUInt32 m_head;
	PfxUInt32 m_tail;
	T SCE_PFX_ALIGNED(16) m_data[SIZE];
	
public:
	
	PfxStaticQueue();
	~PfxStaticQueue();
	
	PfxUInt32 size() const {return m_numData;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& front();
	
	inline const T& front() const;
	
	inline bool empty() const {return m_numData==0;}
	
	inline void clear() {m_numData=0;m_head=0;m_tail=0;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxStaticPriorityQueue

template <class T,int SIZE = SCE_PFX_STATIC_MAX>
class PfxStaticPriorityQueue
{
private:
	PfxUInt32 m_numData;
	T SCE_PFX_ALIGNED(16) m_data[SIZE];
		
	void upHeap(PfxUInt32 i);
	void downHeap(PfxUInt32 i);
	
public:
	PfxStaticPriorityQueue();
	~PfxStaticPriorityQueue();
	
	PfxUInt32 size() const {return m_numData-1;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& front();
	
	inline const T& front() const;
	
	inline bool empty() const {return m_numData==1;}
	
	inline void clear() {m_numData=1;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxStaticStack

template <class T,int SIZE = SCE_PFX_STATIC_MAX>
class PfxStaticStack
{
private:
	PfxUInt32 m_numData;
	SCE_PFX_PADDING(1,12)
	T SCE_PFX_ALIGNED(16) m_data[SIZE];

public:

	inline PfxStaticStack();
	inline ~PfxStaticStack();
	
	PfxUInt32 size() const {return m_numData;}
	
	inline PfxUInt32 push(const T& data);
	
	inline void pop();
	
	inline T& top();
	inline const T& top() const;
	
	inline bool empty() const {return m_numData==0;}
	
	inline void clear() {m_numData = 0;}
};

///////////////////////////////////////////////////////////////////////////////
// PfxStaticArray

template <class T,int SIZE = SCE_PFX_STATIC_MAX>
class PfxStaticArray
{
private:
	PfxUInt32 m_numData;
	PfxUInt32 m_dataLength;
	T SCE_PFX_ALIGNED(16) m_data[SIZE];
	PfxUInt32 m_poolTable[sizeof(PfxUInt32)*((SIZE+31)>>5)];
	
	PfxStaticStack<PfxUInt32,SIZE> m_poolId;
	
public:
	inline PfxStaticArray();
	inline ~PfxStaticArray();
	
	PfxUInt32 size() const {return m_numData;}
	PfxUInt32 capacity() const {return SIZE;}
	
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

///////////////////////////////////////////////////////////////////////////////
// PfxStaticMap

template <class KEY,class T,int SIZE = SCE_PFX_STATIC_MAX>
class PfxStaticMap
{
private:
	PfxUInt32 m_numData;
	
	struct PfxMapNode {
		KEY key;
		T *data;
		PfxMapNode *prev;
		PfxMapNode *next;
	} m_nodes[SIZE],*m_headers[SIZE];
	T m_data[SIZE];
	
	inline PfxUInt32 getHashId(const KEY& key);
	
public:
	inline PfxStaticMap();
	inline ~PfxStaticMap();
	
	PfxUInt32 size() const {return m_numData;}
	
	PfxUInt32 capacity() const {return SIZE;}
	
	inline bool insert(const KEY& key, const T& data);
	
	inline bool find(const KEY& key,const T& data) const;
	
	inline bool find(const KEY& key,T& data);
	
	inline bool erase(const KEY& key);
	
	inline bool empty() const {return m_numData==0;}
	
	inline void clear();

	inline T* ptr(){return m_data;}
};

#include "pfx_static_array_implementation.h"

} // namespace PhysicsEffects
} // namespace sce

