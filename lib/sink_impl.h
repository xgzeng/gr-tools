#pragma once
#include "ber/sink.h"
#include "bit_vector.h"
#include "prbs.h"
#include <chrono>
#include <vector>

namespace gr { namespace ber {

class sink_impl : public sink {
public:
  sink_impl();

  bool start() override;

  int general_work(int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items) override;

private:
  bool synched_ = false;
  BitVector bits_;

  PRBSGenerator prbs_;

  std::vector<uint8_t> sync_bytes_;

  int continue_error_byte_count_ {0};
  int recv_byte_count_  = 0;
  int error_byte_count_ = 0;
  int error_bit_count_ = 0;

  std::chrono::system_clock::time_point last_dump_time_;
};

}} // namespace gr::ber
