#ifndef COLLECTIONS_ARRAY_HPP
#define COLLECTIONS_ARRAY_HPP

#include <stdexcept>

#include "allocator.hpp"


template<Allocatable T>
class Array {

    T* begin_ptr;
    T* end_ptr;
    T* capacity;

public:

    Array(T* begin, T* capacity) :
        begin_ptr { begin },
        end_ptr { begin },
        capacity { capacity } { }


    static Array<T>& create(Allocator& allocator, size_t size) {
        T* begin = allocator.allocate<T>(size);
        T* capacity = begin + size;

        return allocator.create<Array<T>>(begin, capacity);
    }

    bool is_full() const {
        return end_ptr == capacity;
    }

    bool empty() const {
        return end_ptr == begin_ptr;
    }

    size_t size() const {
        return end_ptr - begin_ptr;
    }

    T& append(T value) {
        if (is_full()) {
            throw std::out_of_range { "Array is full" };
        }
        T* t = new(end_ptr) T { std::move(value) };

        return *t;
    }

    auto begin(this auto& self) {
        return self.begin_ptr;
    }

    auto end(this auto& self) {
        return self.end_ptr;
    }

    auto& operator[](this auto& self, size_t i) {
        return self.begin_ptr[i];
    }

    auto& at(this auto& self, size_t i) {
        if (i > self.size()) {
            throw std::out_of_range { "Array index out_of_range" };
        }
        return self[i];
    }

};

#endif  // COLLECTIONS_ARRAY_HPP
