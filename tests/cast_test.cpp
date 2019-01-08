#include <catch2/catch.hpp>

#include "common.hpp"

struct A {
  int m;
};

struct B {
  virtual void foo();
};

struct C {
  virtual void foo() {}
};

struct D : C {
  void foo() override {}
};

TEST_CASE("polymorphic_cast", "[cast]")
{
  C c;
  REQUIRE(std::is_same_v<decltype(eml::polymorphic_cast<D>(c)), D&>);

  D d;
  REQUIRE(std::is_same_v<decltype(eml::polymorphic_cast<C>(d)), C&>);
}
