#pragma once

#include <initializer_list>
#include <utility>
//
#include <lux_shared/slice.hpp>
#include <lux_shared/memory.hpp>

template<typename T>
class DynArr : public Slice<T> {
    public:
    DynArr();
    DynArr(std::initializer_list<T> init);
    DynArr(SizeT cap);
    template<typename ThatT>
    DynArr(Slice<ThatT> const& that);
    DynArr(DynArr<T> const& that);
    DynArr(DynArr<T>&& that);
    ~DynArr();

    using Slice<T>::operator==;
    template<typename ThatT>
    DynArr<T>& operator=(Slice<ThatT> const& that);
    DynArr<T>& operator=(DynArr<T> const& that);
    DynArr<T>& operator=(DynArr<T>&& that);
    T& operator[](SizeT idx);
    T const& operator[](SizeT idx) const;

    template<typename ...Args>
    T& emplace(Args&& ...args);
    T& push(T&& val);
    T& last();
    void erase(SizeT idx);
    void erase(SizeT idx, SizeT len);
    void pop();

    void resize(SizeT new_sz);
    void clear();
    void shrink_to_fit();
    void reserve_at_least(SizeT new_cap);
    void reserve_exactly(SizeT new_cap);
    void dealloc_all();
    SizeT  cap;
    SizeT& len = Slice<T>::len;
    T*&    beg = Slice<T>::beg;
};

template<typename T>
DynArr<T>::DynArr() {
    len = 0;
    cap = 0;
}

template<typename T>
DynArr<T>::DynArr(std::initializer_list<T> init) {
    len = 0;
    cap = 0;
    reserve_exactly(init.size());
    for(auto&& x : init) {
        emplace(x);
    }
}

template<typename T>
DynArr<T>::DynArr(SizeT _cap) {
    len = 0;
    cap = 0;
    reserve_exactly(_cap);
    while(len < _cap) {
        new (beg + len) T();
        len++;
    }
}

template<typename T>
template<typename ThatT>
DynArr<T>::DynArr(Slice<ThatT> const& that) {
    len = 0;
    cap = 0;
    *this = that;
}

template<typename T>
DynArr<T>::DynArr(DynArr<T> const& that) :
    Slice<T>() {
    cap = 0;
    len = 0;
    reserve_exactly(that.len);
    while(len < that.len) {
        emplace(that.beg[len]);
    }
}

template<typename T>
DynArr<T>::DynArr(DynArr<T>&& that) {
    cap = 0;
    len = 0;
    *this = move(that);
}

template<typename T>
DynArr<T>::~DynArr() {
    dealloc_all();
}

template<typename T>
template<typename ThatT>
DynArr<T>& DynArr<T>::operator=(Slice<ThatT> const& that) {
    if(cap < that.len) {
        reserve_exactly(that.len);
    }
    for(Uns i = 0; i < that.len; i++) {
        new (beg + i) T(that[i]);
    }
    len = that.len;
    return *this;
}

template<typename T>
DynArr<T>& DynArr<T>::operator=(DynArr<T> const& that) {
    return *this = (Slice<T>)that;
}

template<typename T>
DynArr<T>& DynArr<T>::operator=(DynArr<T>&& that) {
    dealloc_all();
    beg = that.beg;
    len = that.len;
    cap = that.cap;
    that.len = 0;
    that.cap = 0;
    return *this;
}

template<typename T>
T& DynArr<T>::operator[](SizeT idx) {
    LUX_ASSERT(idx < len);
    return beg[idx];
}

template<typename T>
T const& DynArr<T>::operator[](SizeT idx) const {
    LUX_ASSERT(idx < len);
    return beg[idx];
}

template<typename T>
template<typename ...Args>
T& DynArr<T>::emplace(Args&& ...args) {
    if(len >= cap) {
        reserve_at_least(len + 1);
    }
    new (beg + len) T(args...);
    len++;
    return last();
}

template<typename T>
T& DynArr<T>::push(T&& val) {
    if(len >= cap) {
        reserve_at_least(len + 1);
    }
    new (beg + len) T(move(val));
    len++;
    return last();
}

template<typename T>
T& DynArr<T>::last() {
    LUX_ASSERT(len > 0);
    return beg[len - 1];
}

template<typename T>
void DynArr<T>::erase(SizeT idx) {
    LUX_ASSERT(len != 0);
    LUX_ASSERT(idx < len);
    beg[idx].~T();
    len--;
    for(Uns i = idx; i < len; i++) {
        new (beg + i) T(move(beg[i + 1]));
        beg[i + 1].~T();
    }
}

template<typename T>
void DynArr<T>::erase(SizeT idx, SizeT count) {
    if(idx + count > len)
    LUX_LOG("%zu %zu", idx+count, len);
    LUX_ASSERT(idx + count <= len);
    for(Uns i = idx; i < idx + count; ++i) {
        beg[i].~T();
    }
    for(Uns i = idx + count; i < len; ++i) {
        new (beg + i - count) T(move(beg[i]));
        beg[i].~T();
    }
    len -= count;
}

template<typename T>
void DynArr<T>::pop() {
    LUX_ASSERT(len > 0);
    erase(len - 1);
}

template<typename T>
void DynArr<T>::resize(SizeT new_sz) {
    reserve_at_least(new_sz);
    while(len < new_sz) {
        new (beg + len) T();
        len++;
    }
    while(len > new_sz) {
        len--;
        beg[len].~T();
    }
}

template<typename T>
void DynArr<T>::clear() {
    while(len > 0) {
        len--;
        beg[len].~T();
    }
}

template<typename T>
void DynArr<T>::shrink_to_fit() {
    reserve_exactly(len);
}

template<typename T>
void DynArr<T>::reserve_at_least(SizeT new_cap) {
    SizeT temp = cap == 0 ? 1 : cap;
    while(temp < new_cap) {
        temp *= 2;
    }
    reserve_exactly(temp);
}

template<typename T>
void DynArr<T>::reserve_exactly(SizeT new_cap)
{
    if(cap == new_cap) return;
    if(new_cap == 0) {
        dealloc_all();
    } else if(cap == 0) {
        beg = lux_alloc<T>(new_cap);
        cap = new_cap;
    } else {
        T* new_beg = lux_alloc<T>(new_cap);
        for(Uns i = 0; i < len; i++) {
            new (new_beg + i) T(move(beg[i]));
            beg[i].~T();
        }
        lux_free<T>(beg);
        beg = new_beg;
        cap = new_cap;
    }
}

template<typename T>
void DynArr<T>::dealloc_all() {
    if(cap == 0) return;
    clear();
    lux_free<T>(beg);
    cap = 0;
}

template<typename T>
void swap(DynArr<T>& a, DynArr<T>& b) noexcept {
    swap(a.cap, b.cap);
    swap(a.len, b.len);
    swap(a.beg, b.beg);
}
