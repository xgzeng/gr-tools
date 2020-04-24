#include "bit_vector.h"
#include <boost/optional/optional_io.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace gr::ber;

TEST_CASE("BitVector default construction") {
  BitVector bv;

  REQUIRE(bv.bit_count() == 0);
}

TEST_CASE("BitVector basic operation") {
  BitVector bv;

  uint8_t test_bytes[] {0x0F, 0xE0, 0xA7};
  bv.Append(test_bytes, sizeof(test_bytes));

  REQUIRE(bv.bit_count() == 24);
  REQUIRE(bv.GetByte(0) == 0x0F);
  REQUIRE(bv.GetByte(8) == 0xE0);

  WHEN("LeftShift 4") {
    bv.LeftShift(4);
    REQUIRE(bv.bit_count() == 20);
    REQUIRE(bv.GetByte(0) == 0x0FE);
  }

  WHEN("LeftShift 7") {
    bv.LeftShift(7);
    REQUIRE(bv.bit_count() == 17);
    REQUIRE(bv.GetByte(0) == 0xF0);
  }

  WHEN("LeftShift 15") {
    bv.LeftShift(15);
    REQUIRE(bv.bit_count() == 9);
    REQUIRE(bv.GetByte(0) == 0x53);
    REQUIRE(bv.GetByte(1) == 0xA7);
  }

  WHEN("LeftShift 16") {
    bv.LeftShift(16);
    REQUIRE(bv.bit_count() == 8);
    REQUIRE(bv.GetByte(0) == 0xA7);
  }
}

TEST_CASE("BitVector find") {
  uint8_t pattern[] {0x12, 0x34, 0x56};

  WHEN("BitVector is empty") {
    BitVector bv;
    REQUIRE_FALSE(bv.Find(pattern, sizeof(pattern)));
  }

  WHEN("BitVector is shorter then patten") {
    BitVector bv;
    bv.Append({0x12, 0x34});
    bv.Find(pattern, sizeof(pattern));
    REQUIRE_FALSE(bv.Find(pattern, sizeof(pattern)));

    bv.Append({0x56});
    REQUIRE(bv.Find(pattern, sizeof(pattern)).value() == 0);
  }

  WHEN("Patten data is in middle") {
    BitVector bv;
    bv.Append({0x01, 0x23, 0x45, 0x67});
    bv.Find(pattern, sizeof(pattern));
    REQUIRE(bv.Find(pattern, sizeof(pattern)).value() == 4);
  }

  WHEN("No Patten Data") {
    BitVector bv;
    bv.Append({0x01, 0x23, 0x45, 0x56});
    bv.Find(pattern, sizeof(pattern));
    REQUIRE_FALSE(bv.Find(pattern, sizeof(pattern)));
  }
}
