#include <bitset>
#include <iostream>
#include "test_framework/generic_test.h"

constexpr auto bucket_size = 1 << 16; // space for 16 bit words
static_assert(bucket_size == 65536);
std::array<short, bucket_size> lut; // making this consteval would be neat


template <typename T>
void printBitRepresentation(T num) {
    std::bitset<sizeof(T) * 8> bits(num);
    std::cout << bits << std::endl;
}

short Parity(unsigned long long x) {
  //printBitRepresentation(x);

  short result = 0;
  while (x) {
    x = x & (x-1); // remove least significant 1
    result ^= 1; // toggle 0/1
    // std::cout << "result: " << result << std::endl;
    // printBitRepresentation(x);
  }
  return result;
}

// splits parity calculation of 64 bit word into 4x16 bit words lookup
short ParityLUT(unsigned long long x) {
  short result = 0;
  constexpr auto maskSize = 16;
  constexpr auto mask = 0xFFFF;
  result += lut[(x >> 0 * maskSize) & mask];
  result += lut[(x >> 1 * maskSize) & mask];
  result += lut[(x >> 2 * maskSize) & mask];
  result += lut[(x >> 3 * maskSize) & mask];
  return result % 2;
}

short ParityLogN(unsigned long long x) {
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1; 
  return x & 1;
}

int main(int argc, char* argv[]) {
  for(auto i = 0; i < lut.size(); i++) {
    lut[i] = Parity(i); // initialize lookup table
  }

  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "parity.cc", "parity.tsv", &ParityLogN, DefaultComparator{}, param_names);
}
