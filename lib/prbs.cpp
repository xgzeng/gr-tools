#include "prbs.h"
#include <vector>

#if 0
// https://en.wikipedia.org/wiki/Pseudorandom_binary_sequence
// https://blog.kurttomlinson.com/posts/prbs-pseudo-random-binary-sequence

int main(int argc, char* argv[]) {
  //                       (Binary Form w/o x0 Term)
  // PRBS5  x^5 + x^3 + 1      0b10100
  // PRBS7  x^7 + x^6 + 1      0b1100000
  // PRBS31 x^31 + x ^ 28 + 1  0x48000000
  const uint32_t PBS7  = 0b1100000;
  const uint32_t PBS31 = 0x48000000;
  
  const uint32_t POLYNOMIAL = PBS31;
  
  const uint32_t START_STATE = 0x01;
  
  uint32_t lfsr = START_STATE;
  
  std::string output_bits;
  
  uint32_t period = 0;
  do {
    uint32_t lsb = lfsr & 0x01; // LSB, output bit
    
    // output_bits += lsb ? '1' : '0';
    
    lfsr >>= 1;
    if (lsb) {
      lfsr ^= POLYNOMIAL;
    }
    ++period;
  } while (lfsr != START_STATE);
  
  std::cout << "period is " << period << std::endl;
  // std::cout << "output bits: " << output_bits << std::endl;
  return 0;
}
#endif

void PRBSGenerator::Reset() {
  state_ = 0x10;
  seq_pos_ = 0;
}

uint8_t PRBSGenerator::Next() {
  uint8_t result = 0;
  for (int i = 0; i < 8; ++i) {
    uint32_t lsb = state_ & 0x01; // LSB, output bit

    // output_bits += lsb ? '1' : '0';
    result = (result << 1) | lsb;

    state_ >>= 1;
    if (lsb) {
      state_ ^= polynomial_;
    }
  }

  seq_pos_ += 1;
  if (seq_pos_ >= max_length_) {
    Reset();
  }
  return result;
}
