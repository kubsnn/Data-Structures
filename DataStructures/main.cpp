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


#include <ranges>


#include "deque.h"
#include <deque>


#include "random.h"

struct test {
	test() {
		dbg "CREATED" << nl;
	}
};

auto f = []()->auto {
	int tab[10]{ 1 };
	fill(tab, tab + 10, 5);

	return tab;
};

#include "matrix.h"
using namespace pipeline;

struct test1 {
	array<int, 2> data[2];
};

int main()
{
	matrix<int, 2, 2> a(0);
	a[0][1] = 2;
	a[0][0] = 2;
	a.print();


	matrix<int, 2, 2> b(1);
	b[0][0] = 2;

	a -= b | transpose;
	a.print();
	
}