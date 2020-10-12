#include "prbs.h"
#include <vector>

// https://en.wikipedia.org/wiki/Pseudorandom_binary_sequence
// https://blog.kurttomlinson.com/posts/prbs-pseudo-random-binary-sequence

PRBSGenerator::PRBSGenerator() {
  Reset();
}

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
  // if (seq_pos_ >= max_length_) {
  //   Reset();
  // }
  return result;
}
