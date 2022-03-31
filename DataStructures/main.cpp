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

int main()
{
    srand(time(NULL));
    
    HashTable<_STD string, HashTable<int, int>> table;

    HashTable<int, int> t1;
    t1.insert(10, 12);
    t1.insert(8, 11);

    HashTable<int, int> t2;
    t2.insert(101, 121);
    t2.insert(81, 111);

    table.insert("lol", t1);
    table.insert("liga", t2);

    print table["lol"][8] nl;
}

