#include "allocator.hpp"


int main() {
    Allocator allocator;

    struct S1 {
        int x;
        bool y;
    };

    S1* s1 = allocator.allocate<S1>();
    S1* s2 = allocator.allocate<S1>(2);


    struct S2 {
        char chars[5];
    };

    S2* s3 = allocator.allocate<S2>();
    S2* s4 = allocator.allocate<S2>(2);

    struct S3 {
        size_t arr[32];
    };

    S3* s5 = allocator.allocate<S3>();
    S3* s6 = allocator.allocate<S3>(2);

    std::println("alignof(S1): {}", alignof(S1));
    std::println("alignof(S2): {}", alignof(S2));
    std::println("alignof(S3): {}", alignof(S3));

    std::println("s1: {}", static_cast<void*>(s1));
    std::println("s2: {}", static_cast<void*>(s2));
    std::println("s3: {}", static_cast<void*>(s3));
    std::println("s4: {}", static_cast<void*>(s4));
    std::println("s5: {}", static_cast<void*>(s5));
    std::println("s6: {}", static_cast<void*>(s6));

    size_t ss1 = reinterpret_cast<size_t>(s1);
    size_t ss2 = reinterpret_cast<size_t>(s2);
    size_t ss3 = reinterpret_cast<size_t>(s3);
    size_t ss4 = reinterpret_cast<size_t>(s4);
    size_t ss5 = reinterpret_cast<size_t>(s5);
    size_t ss6 = reinterpret_cast<size_t>(s6);
    
    std::println("s1: {}", ss1);
    std::println("s2: {}", ss2);
    std::println("s3: {}", ss3);
    std::println("s4: {}", ss4);
    std::println("s5: {}", ss5);
    std::println("s6: {}", ss6);

    std::println("s1: {:b}", ss1);
    std::println("s2: {:b}", ss2);
    std::println("s3: {:b}", ss3);
    std::println("s4: {:b}", ss4);
    std::println("s5: {:b}", ss5);
    std::println("s6: {:b}", ss6);

    std::println("s2 - s1: {}", ss2 - ss1);
    std::println("s3 - s2: {}", ss3 - ss2);
    std::println("s4 - s3: {}", ss4 - ss3);
    std::println("s5 - s4: {}", ss5 - ss4);
    std::println("s6 - s5: {}", ss6 - ss5);
}

