#pragma once

#include <ber/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace ber {

class BER_API symbol_error_rate_source : virtual public gr::block {
public:
  typedef boost::shared_ptr<symbol_error_rate_source> sptr;

  /*!
   * @brief Return a shared_ptr to a new instance of source.
   */
  static sptr make(int bits_per_symbol);
};

class BER_API symbol_error_rate_sink : virtual public gr::block {
public:
  typedef boost::shared_ptr<symbol_error_rate_sink> sptr;
  /*!
   * @brief Return a shared_ptr to a new instance of sink.
   */
  static sptr make(int bits_per_symbol);
};

} // namespace ber
} // namespace gr
