#pragma once

#include <ber/api.h>
#include <gnuradio/block.h>

namespace gr { namespace ber {

class BER_API sink : virtual public gr::block {
public:
  typedef boost::shared_ptr<sink> sptr;

  /*!
  * @brief Return a shared_ptr to a new instance of sink.
  *
  * To avoid accidental use of raw pointers, sink's
  * constructor is private.  limesdr::sink::make is the public
  * interface for creating new instances.
  * 
  * @param serial Device serial number. Cannot be left blank.
  *
  * @param device_type LimeSDR-Mini(1), LimeSDR-USB(2).
  * @return a new limesdr sink block object
  */
  static sptr make();
};

} // namespace ber
} // namespace gr
