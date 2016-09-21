#ifndef TEST_HPP_
#define TEST_HPP_

#include <boost/predef.h>

#if(BOOST_COMP_CLANG)

#if __has_warning("-Wglobal-constructors")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#include <boost/test/unit_test.hpp>
#pragma clang diagnostic pop
#endif

#else

#include <boost/test/unit_test.hpp>

#endif

#endif
