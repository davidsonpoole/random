#include <iostream>

bool isPrime(long x) {
    for (long i=2; i<x; i++) {
        if (x%i == 0) {
            return false;
        }
    }
    return true;
}
int main() {

    long i = 0;

    while (true) {
        if (isPrime(i)) {
            std::cout << i<< std::endl;
        } 
        i++;
    }
}
