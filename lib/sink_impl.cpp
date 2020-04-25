#include "sink_impl.h"
#include <cassert>

namespace gr {
namespace ber {

sink::sptr sink::make() { return boost::make_shared<sink_impl>(); }

sink_impl::sink_impl()
    : block("ber_sink", io_signature::make(1, 1, 1),
            io_signature::make(0, 0, 0)) {}

bool sink_impl::start() {
  synched_ = false;
  bits_.Clear();
  last_dump_time_ = std::chrono::system_clock::now();

  for (int i = 0; i < 16; ++i) {
    sync_bytes_.push_back(prbs_.Next());
  }
  prbs_.Reset();
  return true;
}

static int count_ones(uint8_t b) {
  static const uint8_t NIBBLE_LOOKUP[16] = {0, 1, 1, 2, 1, 2, 2, 3,
                                            1, 2, 2, 3, 2, 3, 3, 4};

  return NIBBLE_LOOKUP[b & 0x0F] + NIBBLE_LOOKUP[b >> 4];
}

int sink_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items) {
  auto items = reinterpret_cast<const uint8_t *>(input_items[0]);
  auto item_count = ninput_items[0];

  bits_.Append(items, item_count);
  consume(0, item_count);

  if (!synched_) {
    const auto sync_pos = bits_.Find(sync_bytes_);
    if (sync_pos != BitVector::npos) {
      GR_INFO("ber_sink", "stream synced");
      bits_.LeftShift(sync_pos);
      synched_ = true;
      prbs_.Reset();
    } else {
      bits_.LeftShift(bits_.bit_count() - sync_bytes_.size() * 8);
    }
  }

  if (synched_) {
    int i = 0;
    for (; i < bits_.bit_count() / 8; ++i) {
      uint8_t received = bits_.GetByte(i * 8);
      recv_byte_count_ += 1;

      uint8_t expected = prbs_.Next();
      if (expected != received) {
        error_byte_count_ += 1;
        error_bit_count_ += count_ones(expected ^ received);

        continue_error_byte_count_++;
        if (continue_error_byte_count_ >= max_error_bytes_allowed_) {
          GR_INFO("ber_sink", "continue error bytes, stream out of sync");
          synched_ = false;
          break;
        }
      } else {
        continue_error_byte_count_ = 0;
      }
    }
    bits_.LeftShift(i * 8);
  }

  // DUMP static information every 5 seconds
  auto now = std::chrono::system_clock::now();
  if ((now - last_dump_time_) > std::chrono::seconds(5)) {
    if (!synched_) {
      GR_INFO("ber_sink: ", "OUT_OF_SYNC");
    } else {
      // GR_INFO("ber_sink: ", "processed=" << (recv_byte_count_ * 8.0 / 5 /
      // 1000) << "kbps  "
      //     << "error_byte=" << error_byte_count_ << "B  "
      //     << "error_bits=" << error_bit_count_  << "b  "
      //     << "BER=" << (error_bit_count_ / 8.0) / recv_byte_count_);
    }
    last_dump_time_ = now;
    recv_byte_count_ = 0;
    error_byte_count_ = 0;
    error_bit_count_ = 0;
  }

  return 0;
}

} // namespace ber
} // namespace gr
