// DataStructures.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Utility.h"
#include "Algorithms.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "vector.h"
#include "array.h"
#include "Set.h"
#include "forward_list.h"
#include "dynarray.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>


//#define print _STD cout<<
//#define sp <<" "
//#define nl <<_STD endl
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//#include <list>
//#include <stack>

#define dbg _STD cout <<
#define nl _STD endl;

#define TIME_START { auto begin = high_resolution_clock::now(); for (int _i = 0; _i < 100; ++_i) {
#define TIME_END } auto end = high_resolution_clock::now(); dbg duration_cast<microseconds>(end - begin).count() / 100 << "us" << nl; }

//using namespace std;

struct item {
    int x;
    item(){  }
    item(int x) :x(x) { }//_STD cout << "INITIALIZE CONSTRUCTORE" << _STD endl;

    item(const item& other) :x(other.x) { }//_STD cout << "COPIED" << _STD endl; }
    item(item&& other)noexcept :x(move(other.x)) { }// cout << "MOVED" << _STD endl; }
    item& operator=(const item& other) {
        x = other.x;
        return *this;
    }
    item& operator=(item&& other) noexcept {
        x = move(other.x);
        return *this;
    }
    bool operator==(const item& other) const {
        return x == other.x;
    }
    bool operator!=(const item& other) const {
        return x != other.x;
    }
};

template<>
struct less<item>
{
    bool operator()(const item& left, const item& right)
    {
        return left.x < right.x;
    }
};

template<>
struct Hash<item>
{
    size_t operator()(const item& _Item) 
    {
        return Hash<int>()(_Item.x);
    }
};


struct empty{};
template<class T>
struct tmp {
    using val = T;
    using ptr = T*;
    val f;
    ptr a;
};
#include <map>
#include <unordered_set>



template <class T>
struct my_alloc : _STD allocator<T>
{
	using base = _STD allocator<T>;
    T* allocate(size_t size) {
        //print "ALLOCATE" nl;
        return base::allocate(size);
    }
    void deallocate(T* p, size_t size) {
        //print "DEALLOCATE" nl;
        base::deallocate(p, size);
    }
};

#include "rbtree.h"
#include "map.h"
#include "memory.h"


unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned rands()
{
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    return lfsr = (lfsr >> 1) | (bit << 15);
}
#include <thread>

#ifdef _M_IX86                      // Processor: x86

inline uint64_t clockCycleCount()
{
    uint64_t c;
    __asm {
        cpuid       // serialize processor
        rdtsc       // read time stamp counter
        mov dword ptr[c + 0], eax
        mov dword ptr[c + 4], edx
    }
    return c; 
}

#elif defined(_M_X64)               // Processor: x64

extern "C" unsigned __int64 __rdtsc();
#pragma intrinsic(__rdtsc)
inline uint64_t clockCycleCount()
{
    return __rdtsc();
}

#endif

#include <ranges>

using namespace pipeline;

#include "deque.h"
#include <deque>


int main()
{
    auto rng = range<int, deque>(100);
	
    const auto cpy = rng;
	
    dbg cpy.front() << " " << cpy.back();
}

