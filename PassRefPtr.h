/*
 *  Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_PassRefPtr_h
#define WTF_PassRefPtr_h

template<typename T> class RefPtr;
template<typename T> class PassRefPtr;
template<typename T> PassRefPtr<T> adoptRef(T*);

inline void adopted(const void*) { }



#define ALWAYS_INLINE inline


template<typename T> ALWAYS_INLINE void refIfNotNull(T* ptr)
{
    if (ptr != 0)
        ptr->ref();
}

template<typename T> ALWAYS_INLINE void derefIfNotNull(T* ptr)
{
    if (ptr != 0)
        ptr->deref();
}


template<typename T> 
class PassRefPtr 
{
public:
    PassRefPtr() : m_ptr(0) { }
    PassRefPtr(T* ptr) : m_ptr(ptr) { refIfNotNull(ptr); }
    // It somewhat breaks the type system to allow transfer of ownership out of
    // a const PassRefPtr. However, it makes it much easier to work with PassRefPtr
    // temporaries, and we don't have a need to use real const PassRefPtrs anyway.
    PassRefPtr(const PassRefPtr& o) : m_ptr(o.leakRef()) { }
    template<typename U> PassRefPtr(const PassRefPtr<U>& o) : m_ptr(o.leakRef()) { }

    ALWAYS_INLINE ~PassRefPtr() { derefIfNotNull(m_ptr); }

    template<typename U> PassRefPtr(const RefPtr<U>&);
    
    T* get() const { return m_ptr; }

    T* leakRef() const;

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }

    bool operator!() const { return !m_ptr; }

    // This conversion operator allows implicit conversion to bool but not to other integer types.
    typedef T* (PassRefPtr::*UnspecifiedBoolType);
    operator UnspecifiedBoolType() const { return m_ptr ? &PassRefPtr::m_ptr : 0; }

    PassRefPtr& operator=(const PassRefPtr&) { return *this; }

    friend PassRefPtr adoptRef<T>(T*);

private:
    // adopting constructor
    PassRefPtr(T* ptr, bool) : m_ptr(ptr) { }

    mutable T* m_ptr;
};

template<typename T> template<typename U> ALWAYS_INLINE PassRefPtr<T>::PassRefPtr(const RefPtr<U>& o)
    : m_ptr(o.get())
{
    T* ptr = m_ptr;
    refIfNotNull(ptr);
}

template<typename T> ALWAYS_INLINE T* PassRefPtr<T>::leakRef() const
{
    T* ptr = m_ptr;
    m_ptr = 0;
    return ptr;
}

template<typename T, typename U> ALWAYS_INLINE bool operator==(const PassRefPtr<T>& a, const PassRefPtr<U>& b) 
{ 
    return a.get() == b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator==(const PassRefPtr<T>& a, const RefPtr<U>& b) 
{ 
    return a.get() == b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator==(const RefPtr<T>& a, const PassRefPtr<U>& b) 
{ 
    return a.get() == b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator==(const PassRefPtr<T>& a, U* b) 
{ 
    return a.get() == b; 
}

template<typename T, typename U> ALWAYS_INLINE bool operator==(T* a, const PassRefPtr<U>& b) 
{
    return a == b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator!=(const PassRefPtr<T>& a, const PassRefPtr<U>& b) 
{ 
    return a.get() != b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator!=(const PassRefPtr<T>& a, const RefPtr<U>& b) 
{ 
    return a.get() != b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator!=(const RefPtr<T>& a, const PassRefPtr<U>& b) 
{ 
    return a.get() != b.get(); 
}

template<typename T, typename U> ALWAYS_INLINE bool operator!=(const PassRefPtr<T>& a, U* b)
{
    return a.get() != b; 
}

template<typename T, typename U> ALWAYS_INLINE bool operator!=(T* a, const PassRefPtr<U>& b) 
{ 
    return a != b.get(); 
}

template<typename T> ALWAYS_INLINE PassRefPtr<T> adoptRef(T* p)
{
    adopted(p);
    return PassRefPtr<T>(p, true);
}

template<typename T, typename U> ALWAYS_INLINE PassRefPtr<T> static_pointer_cast(const PassRefPtr<U>& p) 
{ 
    return adoptRef(static_cast<T*>(p.leakRef())); 
}

template<typename T, typename U> inline PassRefPtr<T> const_pointer_cast(const PassRefPtr<U>& p) 
{ 
    return adoptRef(const_cast<T*>(p.leakRef())); 
}

template<typename T> ALWAYS_INLINE T* getPtr(const PassRefPtr<T>& p)
{
    return p.get();
}


#endif // WTF_PassRefPtr_h
