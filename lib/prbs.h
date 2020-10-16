#pragma
#include <cstdint>
#include <vector>

class PRBSGenerator {
public:
  static constexpr uint32_t PBS7  = 0b1100000;
  static constexpr uint32_t PBS15 = 0b110000000000000;
  static constexpr uint32_t PBS23 = 0b10000100000000000000000;
  static constexpr uint32_t PBS31 = 0x48000000;
  
  PRBSGenerator();
  
  void Reset();

  uint8_t Next();

  uint8_t Next(int bits);

private:
  int max_length_ = 1024 * 64;
  uint64_t seq_pos_ = 0;
  uint32_t state_ = 0x01;
  uint32_t polynomial_ = PBS15;
};
