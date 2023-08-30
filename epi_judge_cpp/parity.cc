#include <bitset>
#include <iostream>
#include "test_framework/generic_test.h"

template <typename T>
void printBitRepresentation(T num) {
    std::bitset<sizeof(T) * 8> bits(num);
    std::cout << bits << std::endl;
}

short Parity(unsigned long long x) {
  printBitRepresentation(x);

  short result = 0;
  while (x) {
    x = x & (x-1); // remove least significant 1
    result ^= 1; // toggle 0/1
    // std::cout << "result: " << result << std::endl;
    // printBitRepresentation(x);
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "parity.cc", "parity.tsv", &Parity,
                         DefaultComparator{}, param_names);
}
