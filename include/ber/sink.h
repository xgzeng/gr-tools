#pragma once

#include <ber/api.h>
#include <gnuradio/block.h>

namespace gr { namespace ber {

class BER_API sink : virtual public gr::block {
public:
  typedef boost::shared_ptr<sink> sptr;

  /*!
  * @brief Return a shared_ptr to a new instance of sink.
  */
  static sptr make();
};

} // namespace ber
} // namespace gr
