#ifndef IEOMPP_IO_EXCEPTION_HPP_
#define IEOMPP_IO_EXCEPTION_HPP_

#include "ieompp/exception.hpp"

namespace ieompp
{
    namespace io
    {
        class IOException : public Exception
        {
        public:
            IOException(const std::string& msg) : Exception(msg) {}
            IOException(const char* msg) : Exception(msg) {}
        };
    } // namespace io
} // namespace ieompp

#endif
