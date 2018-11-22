#pragma once

#include <ber/api.h>
#include <gnuradio/block.h>

namespace gr { namespace ber {

class BER_API source : virtual public gr::block {
public:
	typedef boost::shared_ptr<source> sptr;

	/*!
	* @brief Return a shared_ptr to a new instance of source.
	*
	* To avoid accidental use of raw pointers, source's
	* constructor is private.  limesdr::source::make is the public
	* interface for creating new instances.
	* 
	* @param serial Device serial number. Cannot be left blank.
	*
	* @param device_type LimeSDR-Mini(1),LimeSDR-USB(2).
	* 
	* @return a new limesdr source block object
	*/
	static sptr make();
};

}} // namespace gr::ber
