#pragma once
#include "ber/source.h"
#include "prbs.h"

namespace gr { namespace ber {

class source_impl : public source {
public:
  source_impl();

  int general_work(int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items) override;

private:
  PRBSGenerator prbs_;
};

}} // namespace gr::ber