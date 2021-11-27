#ifndef TESTS_HPP
#define TESTS_HPP

#include <catch2/catch_test_macros.hpp>


#include <iostream>

#include "ringbuffer.hpp"

TEST_CASE( "RingBuffer bracketted access is tested", "[bracketted_access_to_RingBuffer]" ) {
    RingBuffer<long> rb(100);
    std::cout << std::endl;
    for (long i=0; i<100; ++i)
        rb.insert(i);
    for (long i=0; i<100; ++i)
        std::cout << i << "  " << rb[i] << std::endl;

    for (long i=0; i<98; ++i)
            REQUIRE(rb[i] == i);
}

TEST_CASE( "RingBuffer getlenghtToRead() is tested", "[getlenghtToRead_of_RingBuffer]" ) {
    RingBuffer<double> rb(5);
    REQUIRE(rb.getlenghtToRead() == 0);
    for (unsigned long i=0; i<100; ++i)
    {
        if (i < 5)
            REQUIRE(rb.getlenghtToRead() == i);
        else
            REQUIRE(rb.getlenghtToRead() == 4);
        double insVar = i;
        rb.insert(insVar);
    }
}

#endif // TESTS_HPP

