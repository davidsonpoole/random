#include <array>
#include <atomic>
#include <stdexcept>

/*
 * A non-owning thread-safe SPSC queue for managing object ordering
 */
template <typename T, int _size>
class Queue {
public:
    Queue() {
    }

    void push(const T &e) {

        constexpr int capacity = _size;

        size_t pushIdx = _pushIdx.load(std::memory_order_relaxed);
        size_t popIdx = _popIdx.load(std::memory_order_acquire);

        if (pushIdx - popIdx == capacity) {
            return;
        }

        int offset = pushIdx % capacity;

        arr[offset] = e;

        _pushIdx.store(pushIdx+1, std::memory_order_release);
    }

    T pop() {

        constexpr int capacity = _size;

        size_t popIdx = _popIdx.load(std::memory_order_relaxed);
        size_t pushIdx = _pushIdx.load(std::memory_order_acquire);

        if (pushIdx == popIdx) {
            throw std::runtime_error("Queue is empty");
        }

        int offset = popIdx % capacity;

        T item = arr[offset];

        _popIdx.store(popIdx+1, std::memory_order_release);

        return item;
    }

    size_t size() {



    }

private:
    std::array<T, _size> arr;
    std::atomic<size_t> _pushIdx{0};
    std::atomic<size_t> _popIdx{0};
};

int main() {
    Queue<int, 20> q;

    q.push(1);
}
