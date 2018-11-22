INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BER ber)

FIND_PATH(
    BER_INCLUDE_DIRS
    NAMES ber/api.h
    HINTS $ENV{BER_DIR}/include
        ${PC_BER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BER_LIBRARIES
    NAMES gnuradio-ber
    HINTS $ENV{BER_DIR}/lib
        ${PC_BER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BER DEFAULT_MSG BER_LIBRARIES BER_INCLUDE_DIRS)
MARK_AS_ADVANCED(BER_LIBRARIES BER_INCLUDE_DIRS)

