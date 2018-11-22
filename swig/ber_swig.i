/* -*- c++ -*- */

#define BER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ber_swig_doc.i"

%{
#include "ber/source.h"
#include "ber/sink.h"
%}

%include "ber/source.h"
GR_SWIG_BLOCK_MAGIC2(ber, source);

%include "ber/sink.h"
GR_SWIG_BLOCK_MAGIC2(ber, sink);