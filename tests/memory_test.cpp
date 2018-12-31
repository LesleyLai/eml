#include <catch2/catch.hpp>

#include <memory_resource>
#include <vector>

#include "memory.hpp"

// TEST_CASE("GC Memory Resource")
//{
//  eml::GCMemoryResource mr;

//  SECTION("Basic allocation and deallocation")
//  {
//    std::pmr::vector<int> vec{&mr};
//    vec.push_back(1);
//    assert(vec.at(0) == 1);
//  }
//}
