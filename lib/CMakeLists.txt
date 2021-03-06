# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file was generated by gr_modtool, a tool from the GNU Radio framework
# This file is a part of gr-ber
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

set(CMAKE_CXX_STANDARD 11)

add_library(gnuradio-ber SHARED
    prbs.h prbs.cpp
    source_impl.h source_impl.cpp
    bit_vector.h bit_vector.cpp
    sink_impl.h sink_impl.cpp
    symbol_error_rate_impl.h symbol_error_rate_impl.cpp
)

target_include_directories(gnuradio-ber
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(gnuradio-ber
    gnuradio::gnuradio-runtime
    gnuradio::gnuradio-pmt
    Log4Cpp::log4cpp
)

target_compile_definitions(gnuradio-ber PUBLIC "gnuradio_ber_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-ber PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

install(TARGETS gnuradio-ber)

########################################################################
# Install built library files
########################################################################
# include(GrMiscUtils)
# GR_LIBRARY_FOO(gnuradio-ber RUNTIME_COMPONENT "ber_runtime" DEVEL_COMPONENT "ber_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

add_executable(test-ber
    bit_vector.h bit_vector.cpp
    bit_vector_test.cpp
)

target_link_libraries(test-ber gnuradio-ber)

GR_ADD_TEST(test_ber test-ber)
