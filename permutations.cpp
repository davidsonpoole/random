#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define NUM_THREADS 8

std::ofstream fileStream;

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

void thread_fn(const std::vector<int>& initial, const std::string& a, int n, int start, int end) {

    std::vector<char> buf(1024*1024);
    int offset = 0;

    for (int i=start; i<end; i++) {
        auto v = initial;
        unrank(v, n, i);

        if (offset + v.size() + 1 > buf.size()) {
            // flush buf
            fileStream.write(buf.data(), offset);
            offset = 0;
        }

        for (auto i : v) {
            buf[offset++] = a[i];
        }
        buf[offset++] = '\n';
    }

    fileStream.write(buf.data(), offset);
    offset=0;
}

int main() {

    std::vector<std::string> strings = {
        "a",
        "ab",
        "abc",
        "abcd",
        "abcde",
        "abcdef",
        "abcdefg",
        "abcdefgh",
        "abcdefghi",
        "abcdefghij",
        "abcdefghijk",
        "abcdefghijkl",
    };

    for (auto& a : strings) {

        std::cout << "Permutating " << a << std::endl;

        const int n = a.length();

        long n_permutations = n;
        for (int i=1; i<n; i++) {
            n_permutations *= n-i;
        }
        std::cout << "Num permutations: " << n_permutations << std::endl;
        fileStream.open(a + "_permutations.txt");

        std::vector<int> initial;
        for (int i=0; i<n; i++) {
            initial.push_back(i);
        }

        auto start = std::chrono::steady_clock::now();

        std::vector<std::thread> threads;
        for (int i=0; i<NUM_THREADS; i++) {
            long startRange = n_permutations/NUM_THREADS * i;
            long endRange = i < NUM_THREADS-1 ? n_permutations/NUM_THREADS * (i+1) : n_permutations;
            threads.emplace_back(thread_fn, initial, a, n, startRange, endRange);
        }

        for (auto& t : threads) {
            t.join();
        }
        auto end = std::chrono::steady_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << ns.count() << " ns\n";
        
    }
}
