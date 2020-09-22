#pragma once
#include <vector>
#include <cstdint>

namespace gr { namespace ber {

class BitVector {
public:
  BitVector() = default;

  void Clear();

  size_t bit_count() const;

  void Append(const uint8_t* bytes, int count);

  void Append(const std::vector<uint8_t>& bytes);

  void LeftShift(size_t count);

  /// retrieve 8bits from bit set
  /// \param bit_offset, less than 
  uint8_t GetByte(size_t bit_offset) const;

  static const size_t npos;

  /// find bit set
  /// \return offset of found position
  size_t Find(const uint8_t* bytes, int count) const;

  size_t Find(const std::vector<uint8_t>& bytes) const;

private:
  /// count of head bits that not belong to bit set logically
  /// valid value 0 - 7 while bit_bytes_ not empty
  size_t head_bit_count_ {0};
  std::vector<uint8_t> bit_bytes_;
};

}} // namespace gr::ber