// DataStructures.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Utility.h"
#include "Algorithms.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Vector.h"
#include "Array.h"

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
    item(){ _STD cout << "CONSTRUCTOR" << _STD endl; }
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

};

template <class _Container>
void print_v(const _Container& v) {
    for (const auto& e : v) {
        _STD cout << e << " ";
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

int main()
{

    srand(time(NULL));

    auto v = range<int, Vector>(12, -3);
    
    print_v(v);
    
   // sort(v.begin(), v.end(), greater{});


    
} 

