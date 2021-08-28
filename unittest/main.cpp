#include <string>

#include "tinytest.h"
#include "string_test.hpp"
//#include "mylib.h"


int main()
{
  RUN(test_random_string);
  //RUN(test_cheese);
  return TEST_REPORT();
}
