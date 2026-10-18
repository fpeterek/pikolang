#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <bit>
#include <cstddef>
#include <memory>
#include <print>
#include <stdexcept>
#include <type_traits>
#include <new>


struct OutOfMemory : std::runtime_error {
    OutOfMemory() noexcept : std::runtime_error( "out of memory") { }
};

struct FailedToAllocateMemory : std::runtime_error {
    FailedToAllocateMemory() noexcept : std::runtime_error("failed to allocate memory") { }
};


template<typename T>
concept Allocatable = std::is_trivially_destructible_v<T>;


class Allocator {

    std::byte* begin = nullptr;
    std::byte* end = nullptr;
    size_t allocated = 0;

    size_t available_system_memory();

public:

    void swap(Allocator& other) noexcept {
        std::swap(begin, other.begin);
        std::swap(end, other.end);
        std::swap(allocated, other.allocated);
    }

    size_t used() const {
        return static_cast<size_t>(end - begin);
    }

    size_t available() const {
        return allocated - used();
    }

    Allocator() {
        allocated = available_system_memory();

        try {
            begin = new std::byte[allocated];
        } catch (const std::bad_alloc&) {
            throw FailedToAllocateMemory();
        }

        end = begin;
    }

    Allocator(Allocator& other) = delete;
    Allocator& operator=(Allocator& other) = delete;

    Allocator(Allocator&& other) {
        swap(other);
    }

    Allocator& operator=(Allocator&& other) {
        swap(other);
        return *this;
    }

    ~Allocator() {
        delete begin;
    }

    std::byte* align_to(size_t alignment) {
        alignment = std::bit_ceil(alignment);

        size_t integral = reinterpret_cast<size_t>(end);
        size_t rem = integral % alignment;
        size_t skip = 0;

        if (rem > 0) {
            skip = alignment - rem;
        }

        std::println("current {}", integral);
        std::println("aligning to {}", alignment);
        std::println("remainder {}", rem);
        std::println("skip {}", skip);
        std::println("final address {}", reinterpret_cast<void*>(end + skip));

        return end + skip;
    }

    template<Allocatable T>
    T* allocate(size_t count = 1) {
        size_t required = count * sizeof(T);
        size_t alignment = alignof(T);

        std::byte* ptr = align_to(alignment);

        size_t left = allocated - static_cast<size_t>(ptr - begin);

        if (required > left) {
            throw OutOfMemory();
        }

        end = ptr + required;

        return std::launder(reinterpret_cast<T*>(ptr));
    }

    template<Allocatable T, typename... Args>
    T& create(Args&& ...args) {
        T* memory = allocate<T>(1);

        new(memory) T { std::forward<Args...>(args...) };
    }

};


#endif  // ALLOCATOR_HPP
