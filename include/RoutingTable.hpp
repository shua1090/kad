#ifndef KADEMLIA_RT_ROUTINGTABLE_HPP
#define KADEMLIA_RT_ROUTINGTABLE_HPP

#include <bitset>
#include <utility>
#include <vector>
#include <array>
#include <ostream>
#include <iostream>

namespace Kademlia {

    class Node {
        std::bitset<16> sb;
        std::string ip_address;
        short port = 0;
    public:
        Node()=default;
        Node(std::bitset<16> sb, std::string ip_address, short port){
            this->sb = sb;
            this->ip_address = std::move(ip_address);
            this->port = port;
        }

        Node& operator=(const Node& other)= default;
        ~Node()=default;

        friend std::ostream& operator<<(std::ostream& os, const Node& node){
            os << node.sb;
            return os;
        }

        std::string get_bit_string() const {
            return sb.to_string();
        }

        explicit operator std::string() const{
            return ip_address + ":" + std::to_string(port);
        }

    };

    template <class T>
    class LimitedList {
    private:
        int currentSizeInUse = 0;
        int capacity = 0;
        T * arr{};

    public:
        explicit LimitedList(int i){
            this->arr = new T[i];
            this->capacity = i;
        }

        LimitedList() = default;

        void add(T item) {
            if (currentSizeInUse < capacity) {
                arr[currentSizeInUse] = item;
                currentSizeInUse++;
            } else {
                throw std::out_of_range("List is full");
            }
        }

        T get(int index) {
            if (index < currentSizeInUse && index >= 0) {
                return arr[index];
            } else {
                throw std::out_of_range("Index out of range");
            }
        }

        void remove(int index){
            if (index == 0 && currentSizeInUse == 1){
                arr[0] = Node();
                currentSizeInUse--;
            } else if (index < currentSizeInUse - 1) {
                for (int i = index; i < currentSizeInUse-1; i++) {
                    arr[i] = arr[i + 1];
                }
                currentSizeInUse--;
            } else {
                throw std::out_of_range("Index out of range");
            }
        }

        ~LimitedList() {
            delete[] arr;
        }

        T& operator[](int index) {
            return arr[index];
        }

        LimitedList& operator=(const LimitedList& other) {
            if (this != &other) {
                delete[] arr;
                arr = new T[other.capacity];
                currentSizeInUse = other.currentSizeInUse;
                capacity = other.capacity;
                for (int i = 0; i < currentSizeInUse; i++) {
                    arr[i] = other.arr[i];
                }
            }
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const LimitedList<T>& list) {
            os << "[";
            for (int i = 0; i < list.currentSizeInUse; i++) {
                if (i == list.currentSizeInUse - 1) {
                    os << list.arr[i] << "]";
                } else {
                    os << list.arr[i] << ", ";
                }
            }
            os  << std::endl;
            return os;
        }

        bool isFull() {
            return currentSizeInUse == capacity;
        }
        int getSize() {
            return currentSizeInUse;
        }
        int getCapacit(){
            return capacity;
        }
    };

    class KBucket{
        std::string prefix;
        LimitedList<Node> bucket;

    public:
        KBucket * Left = nullptr;
        KBucket * Right = nullptr;
        explicit KBucket(std::string prefix, int size = 20){
            this->bucket = LimitedList<Node>(size);
            this->prefix = std::move(prefix);
        }

        // TODO: implement this to ping nodes, if is full
        void add(const Node& node){

            if(this->bucket.isFull()){
                this->Left = new KBucket(this->prefix + "0", this->bucket.getCapacit());
                this->Right = new KBucket(this->prefix + "1", this->bucket.getCapacit());

                while (bucket.getSize() > 0) {
                    Node first = bucket.get(0);   // get the first element
                    if (first.get_bit_string().at(prefix.length()) == '0') {
                        Left->add(first);
                    } else {
                        Right->add(first);
                    }
                    bucket.remove(0);
                }

                if (node.get_bit_string().at(prefix.length()) == '0') {
                    Left->bucket.add(node);
                } else {
                    Right->bucket.add(node);
                }

            } else {
                this->bucket.add(node);
            }
        }

        LimitedList<Node> * getNodes(){
            return &bucket;
        }

        void print(KBucket * node){
            if (node->Left == nullptr && node->Right == nullptr) {
                std::cout << node->prefix << ": " << node->bucket << std::endl;
            } else {
                std::cout << node->prefix << std::endl;
                std::cout << " - " << std::string(node->prefix.length(), ' '); print(node->Left);
                std::cout << " + "  << std::string(node->prefix.length(), ' ');  print(node->Right);
            }
        }

        ~KBucket(){
            delete Left;
            delete Right;
        }

    };

    class KademliaSearchTree{
        KBucket * rootLeft = nullptr;
        KBucket * rootRight = nullptr;
        bool efficientStorage = false;
        Node curNode;
    public:
        void makeEfficient(Node myNode){
            if (rootLeft == nullptr || rootRight == nullptr){
                // It's too late, we've already created buckets
            }
            this->efficientStorage = true;
            curNode = myNode;
        }

        void insertNode(Node node){
            if (rootLeft == nullptr || rootRight == nullptr ){
                rootLeft = new KBucket("0",5);
                rootRight = new KBucket("1",5);
            }

            KBucket * current = nullptr;
            Node * nodeToInsert = &node;
            if (node.get_bit_string().at(0) == '0') {
                current = rootLeft;
            } else {
                current = rootRight;
            }
            int i = 1;
            while (current->Right != nullptr && current->Left != nullptr){
                if (nodeToInsert->get_bit_string()[i] == '0'){
                    current = current->Left;
                } else {
                    current = current->Right;
                }
                i++;
            }
            current->add(node);
        }

        ~KademliaSearchTree(){
            delete rootRight;
            delete rootLeft;
        }

        void print(){
            rootLeft->print(rootLeft);
            rootRight->print(rootRight);
        }
    };

}




#endif //KADEMLIA_RT_ROUTINGTABLE_HPP