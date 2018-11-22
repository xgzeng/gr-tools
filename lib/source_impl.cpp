#include "source_impl.h"
#include <boost/dynamic_bitset.hpp>

namespace gr { namespace ber {

source::sptr source::make() {
  return boost::make_shared<source_impl>();
}

source_impl::source_impl()
: block("ber_source", io_signature::make(0, 0, 0), io_signature::make(1, 1, 1 /* byte item */)) {
}

int source_impl::general_work(int noutput_items,
                              gr_vector_int &ninput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items) {
  uint8_t* output_buf = reinterpret_cast<uint8_t*>(output_items[0]);

  for (int i = 0; i < noutput_items; ++i) {
    output_buf[i] = prbs_.Next();
  }

  produce(0, noutput_items);
  return WORK_CALLED_PRODUCE;
}

}} // namespace gr::ber
