#ifndef KADEMLIA_RT_ROUTINGTABLE_HPP
#define KADEMLIA_RT_ROUTINGTABLE_HPP

#include <bitset>
#include <vector>
#include <array>
#include <ostream>



namespace Kademlia {

    class Node {
        std::bitset<160> sb;
        std::string ip_address;
        short port = 0;
    public:
        Node()=default;
        Node(std::bitset<160> sb, std::string ip_address, short port){
            this->sb = sb;
            this->ip_address = std::move(ip_address);
            this->port = port;
        }
    };

    template <class T>
    class LimitedList {
    private:
        int currentSizeInUse = 0;
        int capacity = 0;
        T * arr;

    public:
        explicit LimitedList(int i){
            this->arr = new T[i];
            this->capacity = i;
        }

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
            if (index < currentSizeInUse) {
                for (int i = index; i < currentSizeInUse; i++) {
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
            for (int i = 0; i < list.currentSizeInUse; i++) {
                if (i == list.currentSizeInUse - 1) {
                    os << list.arr[i];
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

    class KSTNode{
//        LimitedList<Node> data(10);
        std::string prefix;
        KSTNode * Left = nullptr;
        KSTNode * Right = nullptr;
    public:
        explicit KSTNode(std::string prefix){
            this->prefix = std::move(prefix);
        }

        Node * getNodes(){

        }

        ~KSTNode(){
            delete Left;
            delete Right;

        }

    };

    class KademliaSearchTree{
        KSTNode * rootLeft = nullptr;
        KSTNode * rootRight = nullptr;

        void insertBitString(std::string ID){
            if (rootLeft == nullptr || rootRight == nullptr ){
                rootLeft = new KSTNode("0");
                rootRight = new KSTNode("1");
            }

            KSTNode * current = nullptr;
            if (ID[0] == '0'){
                current = rootLeft;
            } else {
                current = rootRight;
            }


        }

        ~KademliaSearchTree(){
            delete rootRight;
            delete rootLeft;
        }

    };

}




#endif //KADEMLIA_RT_ROUTINGTABLE_HPP