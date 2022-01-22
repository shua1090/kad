#include <iostream>
#include "RoutingTable.hpp"

#include <array>
#include <bitset>
#include <random>
#include <ctime>
#include <vector>
using namespace Kademlia;

std::random_device dev;
Node randomNode(){
    std::default_random_engine rng(dev());
    std::uniform_int_distribution<int> dist(0, 65535);
    int k = dist(rng);
    std::bitset<16> b(k);
    return {b, "null", 5};
}

int main(){
    std::vector<Node> vec;
    for (int i = 0; i < 50; i++){
        vec.push_back(randomNode());
    }
    KademliaSearchTree tree;
    for (int i = 0; i < 50; i++){
        tree.insertNode(vec[i]);
    }
    tree.print();

    return 0;
}