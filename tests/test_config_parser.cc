#include "catch.hpp"

#include "CRADLE/ConfigParser.h"

TEST_CASE("Test general help") {
  const int argc = 2;
  const char* argv[argc] = {"test", "-h"};
  CRADLE::ParseOptions("", argc, argv);
}
