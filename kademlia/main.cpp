#include <iostream>
#include "RoutingTable.hpp"

#include <array>

int main(){

    Kademlia::LimitedList<int> list(20);
    list.add(1);
    list.add(2);
    list.add(3);
    for (int i = 0; i < 15; i++) {
        list.add(i);
    }
    std::cout << list << std::endl;
    Kademlia::LimitedList<int> list2(10);
    list2 = list;
    std::cout << list2 << std::endl;

    return 0;
}