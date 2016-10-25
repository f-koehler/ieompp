#ifndef IEOMPP_EXCEPTION_HPP_
#define IEOMPP_EXCEPTION_HPP_

#include <sstream>
#include <stdexcept>
#include <string>

namespace ieompp
{
    class Exception : public std::exception
    {
    private:
        const std::string _msg;

    public:
        Exception(std::string msg) : _msg(std::move(msg)) {}
        Exception(const char* msg) : _msg(msg) {}
        const char* what() const noexcept override { return _msg.c_str(); }
    };

    class NotImplemented : public Exception
    {
    public:
        NotImplemented(const std::string& msg) : Exception(msg) {}
        NotImplemented(const char* msg) : Exception(msg) {}
    };

    template <typename T>
    T create_exception(const std::string& file, std::size_t line, const std::string& msg)
    {
        std::ostringstream strm;
        strm << file << ':' << line << ": " << msg;
        return T(strm.str());
    }
} // namespace ieompp

#define THROW(type, msg) throw ieompp::create_exception<type>(__FILE__, __LINE__, msg);

#endif
