#pragma once
#include "ber/symbol_error_rate.h"
#include "prbs.h"

namespace gr { namespace ber {

class SymbolErrorRateSourceImpl : public symbol_error_rate_source {
public:
  SymbolErrorRateSourceImpl();
  SymbolErrorRateSourceImpl(int bits_per_symbol);

  int general_work(int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items) override;

private:
  const int bits_per_symbol_ = 1;
  PRBSGenerator prbs_;
};

class SymbolErrorRateSinkImpl : public symbol_error_rate_sink {
public:
  SymbolErrorRateSinkImpl();
  SymbolErrorRateSinkImpl(int bits_per_symbol);

  bool start() override;

  int general_work(int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items) override;

private:
  const int bits_per_symbol_ = 1;

  const int error_limits_ = 64;

  bool synched_ = false;

  PRBSGenerator prbs_;

  std::vector<uint8_t> inputs_;

  std::vector<uint8_t> sync_symbols_;

  int recv_symbol_count_  = 0;
  int error_symbol_count_ = 0;
  int continue_error_count_ = 0;

  std::chrono::system_clock::time_point last_dump_time_;
};

}} // namespace gr::ber