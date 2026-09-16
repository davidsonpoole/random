#include <exception>
#include <stdexcept>
#include <vector>
#include <iostream>

template <typename T>
class SimplePool {
public:
    SimplePool(int capacity) : capacity(capacity) {
        buf = malloc(capacity * sizeof(T));
        if (buf == nullptr) {
            throw std::runtime_error("SimplePool is full");
        }
    }

    ~SimplePool() {
        ::free(buf);
    }

    T* newInstance() {
        if (offset < capacity) {
            T *newObject = new (static_cast<T*>(buf)+offset) T();
            ++offset;
            return newObject;
        }
        return nullptr;
    }

    void free(T* ptr) {
        // do nothing
    }

private:
    void* buf;
    int capacity = 0;
    int offset = 0;
};

struct MyObject {
    int a;
    int b;
    long c;
};

int main() {
    const int initialCapacity = 10;
    SimplePool<MyObject> pool(initialCapacity);
    std::vector<MyObject*> myObjects;

    for (int i=0; i<initialCapacity; i++) {
        try {
            myObjects.push_back(pool.newInstance());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
}
