// DataStructures.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Utility.h"
#include "LinkedList.h"
#include "HashTable.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#define print _STD cout<<
#define sp <<" "
#define nl <<_STD endl

#include <unordered_map>

#define map _STD unordered_map

int main()
{
    //srand(time(NULL));
    HashTable<int, HashTable<int, int>> table;
    clock_t begin = clock();

    for (int i = 0; i < 1000; ++i) {

        for (int j = 0; j < 500; ++j) {
            HashTable<int, int> t;
            for (int k = 0; k < 700; ++k) {
                t.insert(k, i + k + j);
            }
            table.insert(j, move(t));
        }

        for (int j = 0; j < 500; ++j) {
            table.remove(j);
        }

        if (i % 100 == 0) print i nl;
    }
    clock_t end = clock();

    print end - begin << "ms" nl;
}

