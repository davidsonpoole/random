#include <stdexcept>
#include <cstdlib>

template <typename T>
class LinkedPool {
public:
    LinkedPool(int capacity) : capacity(capacity) {
        if (capacity <= 0) {
            throw std::invalid_argument("Pool capacity must be positive");
        }

        buf = static_cast<Entry*>(::operator new(sizeof(Entry) * capacity));

        // link elements
        head = &buf[0];
        head->prev = nullptr;
        for (int i=1; i<capacity; i++) {
            Entry *e = &buf[i-1];
            e->live = false;
            Entry *next = &buf[i];

            e->next = next;
            next->prev = e;
        }
        buf[capacity-1].next = nullptr;
        buf[capacity-1].live = false;
    }

    ~LinkedPool() {
        for (int i=0; i<capacity; i++) {
            Entry *e = &buf[i];
            if (e->live) {
                e->data.~T();
                e->live = false;
            }

        }
        ::operator delete(buf);
    }

    T* newInstance() {

        if (head != nullptr) {
            Entry *e = head;
            Entry *next = e->next;
            if (next != nullptr) {
                next->prev = nullptr;
            }
            e->next = nullptr;
            head = next;
            e->live = true;
            return new (&e->data) T();
        } else {
            throw std::runtime_error("Pool is at capacity");
        }
    }

    void free(T* ptr) {

        Entry *e = reinterpret_cast<Entry*>(ptr);
        if (!e->live) return;
        e->data.~T();
        e->live = false;

        if (head != nullptr) {
            head->prev = e;
        }
        e->next = head;
        head = e;
    }

private:
    int capacity;
    struct Entry {
        T data;
        bool live;
        Entry *next;
        Entry *prev;
    };

    Entry *buf;
    Entry *head;
};
