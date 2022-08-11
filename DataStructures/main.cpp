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


#define log(x) _STD cout << x << _STD endl



struct emptyt{};

#include <chrono>
using namespace std::chrono;

int add_xor(const int N, int B) {
    int sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += i ^ B;
    }
    return sum;
}

int iff(const int N, int B) {
    int sum = 0;
    int a = B << 1;
    for (int i = 0; i < N; ++i) {
        if (i > B) {
            if (i < a) sum += i;
        }
    }
    return sum;
}

template <class Func>
void checktime(Func func, const int N) {
    auto start = high_resolution_clock::now();
    func(N, N / 3);
    auto end = high_resolution_clock::now();
    int diff = duration_cast<microseconds>(end - start).count();
    _STD cout << diff << " us" << _STD endl;
}

#include "rbtree.h"
#include "map.h"


template <class container>
container test_create(size_t n) {

    auto begin = high_resolution_clock::now();
    container data;
    for (int i = 0; i < n; ++i) {
        data.insert(i, i + 1);
    }

    auto end = high_resolution_clock::now();
    dbg _CHRONO duration_cast<_CHRONO milliseconds>(end - begin).count() << nl;
    return data;
}

template <class container>
void test_access(container& data) {
    int sum = 0;
    auto begin = high_resolution_clock::now();

    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }

    auto end = high_resolution_clock::now();
    dbg _CHRONO duration_cast<_CHRONO milliseconds>(end - begin).count() << nl;
    dbg "sum: " << sum << nl;
}


template <class container>
void test_iterate(container& data) {
    int sum = 0;
    auto begin = high_resolution_clock::now();

   // for (auto& [k, v] : data) {
   //    sum += v;
   // }

    auto end = high_resolution_clock::now();
    dbg _CHRONO duration_cast<_CHRONO milliseconds>(end - begin).count() << nl;
    dbg "sum: " << sum << nl;
}

#include "memory.h"

struct f {
    f() {
   //     dbg "CREATED" << nl;
    }
    f(int d, int e) : x(d), y(e) {
 //       dbg "CREATED" << nl;
    }
    f(const f& other) 
        : x(other.x), y(other.y) {
   //     dbg "COPIED" << nl;
    }
    f(f&& other) noexcept
        : x(other.x), y(other.y) {
  //      dbg "MOVED" << nl;
    }
    f& operator=(const f& other) {
        x = other.x;
        y = other.y;
   //     dbg "=COPIED" << nl;
        return *this;
    }
    f& operator=(f&& other) noexcept {
        x = other.x;
        y = other.y;
  //      dbg "=MOVED" << nl;
        return *this;
    }
    int x = 0;
    int y = 0;
};

_STD vector<f>::const_iterator get_at(_STD vector<f>& v, int index) {
    auto it = v.begin();
    for (int i = 0; i < index; ++i) {
        ++it;
    }
    return it;
}

void test_allocs()
{
    TIME_START
        int n = 8;
        auto data = allocator<f>::allocate(n);
        for (int i = 0; n < 200000; ++i) {
            size_t new_size = n * 2;
            auto buff = allocator<f>::allocate(new_size);
            _Move_in_place(data, buff, n);
            allocator<f>::deallocate(data);
            data = buff;
            n = new_size;
        }
    TIME_END
    auto merge = [](f** buffers, int size, int count) {
        f* new_buff = allocator<f>::allocate(size * count);
        for (int i = 0; i < count; ++i) {
            _Move_in_place(buffers[i], new_buff + i * size, size);
            allocator<f>::deallocate(buffers[i]);
        }
        buffers[0] = new_buff;
    };
    TIME_START
       int n = 8;
       int buffer_count = 1;
       f** buffers = new f* [10000];

       buffers[0] = allocator<f>::allocate(n);
       for (int i = 0; n * buffer_count < 200000; ++i) {
           for (int j = 1; j < buffer_count; ++j) {
               buffers[j] = allocator<f>::allocate(n);
           }
           merge(buffers, n, buffer_count);
           n <<= 1;
           buffer_count = n / 8;
       }
    TIME_END
}



struct greater_t
{
     bool operator()(int& _Left, int& _Right) const
    {
        //dbg "!";
        return _Left < _Right;
    }
};

#include "cycle.h"
#include "enumerate.h"
greater_t greaterr;


template <class T>
void ff(T arg) {
    dbg is_rvalue<T> << nl;
}

#include <ranges>

int main()
{
   // std::_Get_unwrapped
  //  std::vector
    auto data = range<array<int, 16>>(2);

    pointer_iterator<int> it = data.begin();

    auto tmp = it + 2;

    for (auto&& [i, e] : data | cycle | enumerate) {
        dbg i << " " << e << nl;
        e++;
    }
  //  cycle(data);

   // cycle c(range<int, vector>(10));

   // for (auto e : cycle(data)) {
   //     dbg e << " ";
   // }


    //{
    //    auto begin = high_resolution_clock::now();
    //    vector<vector<int>> v;
    //    for (int i = 0; i < 50; ++i) {
    //        for (int j = 0; j < 1000; ++j) {
    //            vector<int> u;
    //           // u.reserve(10000);
    //            for (int k = 0; k < 10000; ++k) {
    //                u.append(i + j + k);
    //            }
    //            v.append(u);
    //        }
    //        for (int j = 0; j < 1000; ++j) {
    //            v.remove_at(0);
    //        }
    //       // v.clear();
    //    }
    //    auto end = high_resolution_clock::now();
    //    dbg "my vector: ";
    //    dbg _CHRONO duration_cast<_CHRONO milliseconds>(end - begin).count() << "ms" << nl;
    //}
    //{
    //    auto begin = high_resolution_clock::now();
    //    _STD vector<_STD vector<int>> v;
    //    for (int i = 0; i < 50; ++i) {
    //        for (int j = 0; j < 1000; ++j) {
    //            _STD vector<int> u;
    //           // u.reserve(10000);
    //            for (int k = 0; k < 10000; ++k) {
    //                u.push_back(i + j + k);
    //            }
    //            v.push_back(u);
    //        }
    //        for (int j = 0; j < 1000; ++j) {
    //            v.erase(v.begin());
    //        }
    //      //  v.clear();
    //    }
    //    auto end = high_resolution_clock::now();
    //    dbg "std vector: ";
    //    dbg _CHRONO duration_cast<_CHRONO milliseconds>(end - begin).count() << "ms" << nl;
    //}
}

