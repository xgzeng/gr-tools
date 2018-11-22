#pragma once

#include <ber/api.h>
#include <gnuradio/block.h>

namespace gr { namespace ber {

class BER_API source : virtual public gr::block {
public:
	typedef boost::shared_ptr<source> sptr;

	/*!
	* @brief Return a shared_ptr to a new instance of source.
	*/
	static sptr make();
};

}} // namespace gr::ber
