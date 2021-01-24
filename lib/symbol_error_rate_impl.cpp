#include "symbol_error_rate_impl.h"

namespace gr {
namespace ber {

boost::shared_ptr<symbol_error_rate_source> symbol_error_rate_source::make(int bits_per_symbol) {
  return boost::make_shared<SymbolErrorRateSourceImpl>(bits_per_symbol);
}

boost::shared_ptr<symbol_error_rate_sink> symbol_error_rate_sink::make(int bits_per_symbol) {
  return boost::make_shared<SymbolErrorRateSinkImpl>(bits_per_symbol);
}

SymbolErrorRateSourceImpl::SymbolErrorRateSourceImpl()
    : block("ser_source", io_signature::make(0, 0, 0),
            io_signature::make(1, 1, 1 /* byte item */)) {}

SymbolErrorRateSourceImpl::SymbolErrorRateSourceImpl(int bits_per_symbol)
: block("ser_source", io_signature::make(0, 0, 0),
        io_signature::make(1, 1, 1 /* byte item */)), bits_per_symbol_(bits_per_symbol) {

}

int SymbolErrorRateSourceImpl::general_work(
    int noutput_items, gr_vector_int &ninput_items,
    gr_vector_const_void_star &input_items, gr_vector_void_star &output_items) {

  uint8_t *output_buf = reinterpret_cast<uint8_t *>(output_items[0]);

  for (int i = 0; i < noutput_items; ++i) {
    output_buf[i] = prbs_.Next(bits_per_symbol_);
  }

  produce(0, noutput_items);
  return WORK_CALLED_PRODUCE;
}

SymbolErrorRateSinkImpl::SymbolErrorRateSinkImpl()
    : block("ser_sink", io_signature::make(1, 1, 1),
            io_signature::make(0, 0, 0)) {}


SymbolErrorRateSinkImpl::SymbolErrorRateSinkImpl(int bits_per_symbol)
    : block("ser_sink", io_signature::make(1, 1, 1),
            io_signature::make(0, 0, 0)), bits_per_symbol_(bits_per_symbol) {
}

bool SymbolErrorRateSinkImpl::start() {
  synched_ = false;

  last_dump_time_ = std::chrono::system_clock::now();

  for (int i = 0; i < 32; ++i) {
    sync_symbols_.push_back(prbs_.Next(bits_per_symbol_));
  }

  prbs_.Reset();
  return true;
}

int SymbolErrorRateSinkImpl::general_work(
    int noutput_items, gr_vector_int &ninput_items,
    gr_vector_const_void_star &input_items, gr_vector_void_star &output_items) {

  const auto items = reinterpret_cast<const uint8_t *>(input_items[0]);
  const auto item_count = ninput_items[0];
  inputs_.insert(inputs_.end(), items, items + item_count);
  consume(0, item_count);

  if (!synched_) {
    if (inputs_.size() < sync_symbols_.size())
      return 0;

    auto found = std::search(inputs_.begin(), inputs_.end(),
                             sync_symbols_.begin(), sync_symbols_.end());

    if (found != inputs_.end()) {
      // found
      GR_LOG_INFO(this->d_logger, "symbol stream synced");
      inputs_.erase(inputs_.begin(), found);
      synched_ = true;
      prbs_.Reset();
    } else {
      const auto item_to_remove = inputs_.size() - sync_symbols_.size() + 1;
      inputs_.erase(inputs_.begin(), inputs_.begin() + item_to_remove);
      return 0;
    }
  }

  assert(synched_);

  for (size_t i = 0; i < inputs_.size(); ++i) {
    recv_symbol_count_ += 1;

    if (inputs_[i] != prbs_.Next(bits_per_symbol_)) {
      error_symbol_count_ += 1;
      if (++continue_error_count_ >= error_limits_) {
        GR_LOG_INFO(this->d_logger, "symbol stream out of sync");
        synched_ = false;
        break;
      }
    } else {
      continue_error_count_ = 0;
    }
  }
  inputs_.clear();

  // DUMP static information every 5 seconds
  auto now = std::chrono::system_clock::now();
  if ((now - last_dump_time_) > std::chrono::seconds(5)) {
    char msg[128];
    sprintf(msg, "processed %d(%fksps), error_symbols=%d, Symbol Error Rate=%f",
            recv_symbol_count_, (recv_symbol_count_ / 5.0 / 1000),
            error_symbol_count_,
            error_symbol_count_ * 1.0 / recv_symbol_count_);
    GR_LOG_INFO(this->d_logger, msg);

    last_dump_time_ = now;
    recv_symbol_count_ = 0;
    error_symbol_count_ = 0;
  }

  return 0;
}

} // namespace ber
} // namespace gr