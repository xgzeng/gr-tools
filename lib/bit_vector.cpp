#include "bit_vector.h"
#include <cassert>

namespace gr {
namespace ber {

#define VALIDATE_INVARIANT()                                                   \
  assert((bit_bytes_.empty() && head_bit_count_ == 0) ||                       \
         (head_bit_count_ >= 0 && head_bit_count_ < 8))

const size_t BitVector::npos = -1;

void BitVector::Clear() {
  head_bit_count_ = 0;
  bit_bytes_.clear();
}

size_t BitVector::bit_count() const {
  VALIDATE_INVARIANT();

  return bit_bytes_.size() * 8 - head_bit_count_;
}

void BitVector::Append(const uint8_t *bytes, int count) {
  bit_bytes_.insert(bit_bytes_.end(), bytes, bytes + count);
}

void BitVector::Append(const std::vector<uint8_t> &bytes) {
  bit_bytes_.insert(bit_bytes_.end(), bytes.begin(), bytes.end());
}

void BitVector::LeftShift(size_t count) {
  assert(count <= bit_count());

  const auto discard_byte_count = (head_bit_count_ + count) / 8;
  bit_bytes_.erase(bit_bytes_.begin(), bit_bytes_.begin() + discard_byte_count);

  head_bit_count_ = (head_bit_count_ + count) % 8;
}

uint8_t BitVector::GetByte(size_t bit_offset) const {
  assert(bit_offset < bit_count());

  // bit offset in vector, skip head bits
  const auto raw_bit_offset = head_bit_count_ + bit_offset;
  const auto byte_offset = raw_bit_offset / 8;
  const auto left_shift_bits = raw_bit_offset % 8;
  if (left_shift_bits == 0) {
    return bit_bytes_[byte_offset];
  } else {
    return (bit_bytes_[byte_offset] << left_shift_bits) |
           (bit_bytes_[byte_offset + 1] >> (8 - left_shift_bits));
  }
}

size_t BitVector::Find(const std::vector<uint8_t> &bytes) const {
  return Find(bytes.data(), bytes.size());
}

size_t BitVector::Find(const uint8_t *bytes, int count) const {
  if (bit_count() < count * 8) {
    return -1;
  }

  for (size_t i = 0; i <= bit_count() - count * 8; ++i) {
    bool match = true;
    for (int c = 0; c < count; ++c) {
      if (GetByte(i + c * 8) != bytes[c]) {
        match = false;
        break;
      }
    }

    if (match) {
      return i;
    }
  }

  return -1;
}

} // namespace ber
} // namespace gr
