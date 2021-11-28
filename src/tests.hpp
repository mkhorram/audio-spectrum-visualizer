#ifndef TESTS_HPP
#define TESTS_HPP

#include <catch2/catch_test_macros.hpp>


#include <iostream>

#include "ringbuffer.hpp"



TEST_CASE( "Test of RingBuffer bracketted access", "[bracketted_access_to_RingBuffer]" ) {
    RingBuffer<long> rb(100);
    std::cout << std::endl;
    for (long i=0; i<100; ++i)
        rb.insert(i);
    for (long i=0; i<100; ++i)
        std::cout << i << "  " << rb[i] << std::endl;

    for (long i=0; i<100; ++i)
            REQUIRE(rb[i] == i);
}

TEST_CASE( "Test of RingBuffer over write", "[overwrite_of_RingBuffer]" ) {
    RingBuffer<int> rb(3);
    int var = 0;    rb.insert(var);
    var++;          rb.insert(var);
    var++;          rb.insert(var);
    for (int i=var+1; i<1000; ++i)
    {
        rb.insert(i);
        bool result = rb[0] == (i-2) && rb[1] == (i-1) && rb[2] == i;
        REQUIRE(result);
    }
}

TEST_CASE( "Test of RingBuffer getlenghtToRead()", "[getlenghtToRead_of_RingBuffer]" ) {
    RingBuffer<double> rb(5);
    REQUIRE(rb.getlenghtToRead() == 0);
    for (unsigned long i=0; i<100; ++i)
    {
        if (i < 5)
            REQUIRE(rb.getlenghtToRead() == i);
        else
            REQUIRE(rb.getlenghtToRead() == 5);
        double insVar = i;
        rb.insert(insVar);
    }
}

#endif // TESTS_HPP

