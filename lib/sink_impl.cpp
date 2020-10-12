#include "sink_impl.h"
#include <cassert>

namespace gr {
namespace ber {

sink::sptr sink::make() { return boost::make_shared<sink_impl>(); }

sink_impl::sink_impl()
    : block("ber_sink", io_signature::make(1, 1, 1),
            io_signature::make(0, 0, 0)) {}

bool sink_impl::start() {
  GR_LOG_NOTICE(this->d_logger, "started");

  synched_ = false;
  bits_.Clear();
  last_dump_time_ = std::chrono::system_clock::now();

  for (int i = 0; i < 32; ++i) {
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

  if (!synched_ && (bits_.bit_count() >= sync_bytes_.size() * 8)) {
    const auto sync_pos = bits_.Find(sync_bytes_);
    if (sync_pos != BitVector::npos) {
      GR_LOG_INFO(this->d_logger, "byte stream synced");
      if (bits_.bit_count() % 8 != 0) {
        GR_LOG_INFO(this->d_logger, "byte stream not byte aligned");
      }

      bits_.LeftShift(sync_pos);
      synched_ = true;
      byte_pos_ = 0;
      prbs_.Reset();
    } else {
      // GR_LOG_INFO(this->d_logger, "byte stream not sync");
      // GR_LOG_INFO(this->d_logger, bits_.bit_count());
      bits_.LeftShift(bits_.bit_count() - sync_bytes_.size() * 8 - 1);
      // GR_LOG_INFO(this->d_logger, bits_.bit_count());
    }
  }

  if (synched_) {
    int i = 0;
    for (; i < bits_.bit_count() / 8; ++i) {
      uint8_t received = bits_.GetByte(i * 8);
      recv_byte_count_ += 1;
      
      uint8_t expected = prbs_.Next();
      if (expected != received) {
        // char msg[128];
        // sprintf(msg, "error byte at %d, %x != %x", byte_pos_, (int)received, (int)expected);
        // GR_LOG_INFO(this->d_logger, msg);

        error_byte_count_ += 1;
        error_bit_count_ += count_ones(expected ^ received);

        continue_error_byte_count_++;
        if (continue_error_byte_count_ >= error_limits_) {
          GR_LOG_INFO(this->d_logger, "continue error bytes, stream out of sync");
          synched_ = false;
          break;
        }
      } else {
        continue_error_byte_count_ = 0;
      }

      byte_pos_ += 1;
    }
    bits_.LeftShift(i * 8);
  }

  // DUMP static information every 5 seconds
  auto now = std::chrono::system_clock::now();
  if ((now - last_dump_time_) > std::chrono::seconds(5)) {
    if (synched_) {
      char msg[128];
      sprintf(msg,
              "processed %d(%fkbps), error_bytes=%d, error_bits=%d, BER=%f",
              recv_byte_count_, (recv_byte_count_ * 8.0 / 5 / 1000),
              error_byte_count_, error_bit_count_,
              ((error_bit_count_ / 8.0) / recv_byte_count_));
      GR_LOG_INFO(this->d_logger, msg);
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
