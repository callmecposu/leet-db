#include <fstream>
#include<iostream>

struct Collection{
    void* data;
    size_t size;
};

int main(){
    // create a collection
    Collection col;
    col.data = new int(3);
    *(int*)col.data = 1;
    *((int*)col.data + 1) = 2;
    *((int*)col.data + 2) = 3;

    std::cout << *(int*)col.data << " ";
    std::cout << *((int*)col.data + 1) << " ";
    std::cout << *((int*)col.data + 2)<<  std::endl;;

    return 0;
}