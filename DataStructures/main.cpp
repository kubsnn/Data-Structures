// DataStructures.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Utility.h"
#include "Algorithms.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Vector.h"
#include "Array.h"
#include "Set.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <algorithm>

#define print _STD cout<<
#define sp <<" "
#define nl <<_STD endl
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <list>
//#include <stack>

using _STD vector;

struct pair
{
    int x;
    int y;
    pair() 
        :x{}, y{}
    { }
    pair(int a, int b) 
        :x(a), y(b) 
    { 
        _STD cout << "CONSTRUCTOR" << _STD endl;
    }
    ~pair() {
        _STD cout << "DESTRUCTOR" << _STD endl;
    }
};


//using namespace std;

struct item {
    int x;
    item(){  }
    item(int x):x(x){ _STD cout << "INITIALIZE CONSTRUCTORE" << _STD endl; }
    item(const item& other):x(other.x){ _STD cout << "COPIED" << _STD endl; }
    item(item&& other)noexcept:x(move(other.x)){ _STD cout << "MOVED" << _STD endl; } 
    item& operator=(const item& other) {
        x = other.x;
        return *this;
    }
    item& operator=(item&& other) noexcept {
        x = move(other.x);
        return *this;
    }
    bool operator==(const item& other) {
        return x == other.x;
    }
    bool operator!=(const item& other) {
        return x != other.x;
    }
};

template <class _Container>
void print_v(const _Container& v) {
    for (const auto& e : v) {
        _STD cout << e.x << " ";
    }
    _STD cout << _STD endl;
}

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

int main()
{

    _STD ios_base::sync_with_stdio(false);

    srand(time(NULL));

    HashTable<int, int> h;
    h.insert(3, 5);
    h.insert(6, 3);
    h.insert(4, 2);

    for (auto& [key, val] : h) {
        _STD cout << key << " : " << val << _STD endl;
    }

    Vector<item> v;
    v.emplace_back(3);
    v.emplace_back(2);
    v.emplace_back(5);
    v.emplace_back(3);
    v.emplace_back(7);


    Set<item> s;
    s.insert(v.begin(), v.end());

    LinkedList<item> f;
    f.append(s.begin(), s.end());

    Vector<item> s2;
    s2.append(f.begin(), f.end());
    print_v(s2);
    print_v(f);
    print_v(v);

} 

