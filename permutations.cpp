#include <mutex>
#include <thread>
#include <vector>
#include <iostream>

#define NUM_THREADS 8

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

    std::vector<char> buf(1024*1024);
    int offset = 0;

    for (int i=start; i<end; i++) {
        auto v = initial;
        unrank(v, n, i);

        if (offset + v.size() + 1 > buf.size()) {
            std::cout << "Flushing because v is " << v.size() << " and buf is " << buf.size() << std::endl;
            // flush buf
            std::lock_guard<std::mutex> lock(logLock);
            for (auto i=0; i<offset; i++) {
                std::cout << buf[i];
            }
            offset=0;
        } 

        for (auto i : v) {
            buf[offset++] = '0' + i;
        }
        buf[offset++] = '\n';
    }

    std::cout << "Last flush" << std::endl;
    std::lock_guard<std::mutex> lock(logLock);
    for (auto i=0; i<offset; i++) {
        std::cout << buf[i];
    }
    offset=0;
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

    std::vector<std::thread> threads;
    for (int i=0; i<NUM_THREADS; i++) {
        int startRange = n_permutations/NUM_THREADS * i;
        int endRange = n_permutations/NUM_THREADS * (i+1);
        std::cout << "Spawning thread with range [" << startRange << "," << endRange << ")" << std::endl;
        threads.emplace_back(thread_fn, initial, n, startRange, endRange);
    }

    for (auto& t : threads) {
        t.join();
    }
}
