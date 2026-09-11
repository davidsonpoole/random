#include <mutex>
#include <thread>
#include <vector>
#include <iostream>

#define NUM_THREADS 8

std::atomic<int> running{NUM_THREADS};
std::mutex logLock;

inline void swap(std::vector<int>& v, int i, int j) {
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void unrank(std::vector<int>& v, int n, int r) {
    if (n > 0) {
        swap(v, n-1, r % n);
        unrank(v, n-1, r/n);
    }
}

void thread_fn(const std::vector<int>& initial, int n, int start, int end) {

    std::cout << "Launched thread" << std::endl;

    for (int i=start; i<end; i++) {
        auto v = initial;
        unrank(v, n, i);
        std::lock_guard<std::mutex> lock(logLock);
        for (auto i : v) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    running.fetch_sub(1);
}

int main() {

    const int n = 10;

    long n_permutations = n;
    for (int i=1; i<n; i++) {
        n_permutations *= n-i;
    }
    std::cout << "Num permutations: " << n_permutations << std::endl;

    std::vector<int> initial;
    for (int i=0; i<n; i++) {
        initial.push_back(i);
    }

    for (int i=0; i<NUM_THREADS; i++) {
        int startRange = n_permutations/NUM_THREADS * i;
        int endRange = n_permutations/NUM_THREADS * (i+1);
        std::cout << "Spawning thread with range [" << startRange << "," << endRange << ")" << std::endl;
        std::thread t0{thread_fn, initial, n, startRange, endRange};
        t0.detach();
    }

    while (running.load() != 0);
}
