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

using namespace pipeline;

#include "deque.h"
#include <deque>


#include "random.h"

 
int main()
{
    random r;
    auto data = array<int, 1>()
        | cycle
        | transform([&r](int x) { return r.frand(); })
        | take(10) | to_file("test.txt");

	for (auto&& x : data) {
		dbg x <<  nl;
	}
	return 0;
}

