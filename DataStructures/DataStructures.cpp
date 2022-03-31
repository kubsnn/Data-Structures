// DataStructures.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "Utility.h"
#include "LinkedList.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#define print _STD cout<<
#define sp <<" "
#define nl <<_STD endl

int main()
{
    print sizeof(Node<int>) nl;
    LinkedList<int> list;
    
    for (int i = 0; i < 10000; ++i) {
        for (int j = i; j < 10000; ++j) {
            list.append(i + j);
        }
    }
    print list.size() * sizeof(Node<int>) / 1000 / 1000 nl;
}

