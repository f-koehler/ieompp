#ifndef IEOMPP_ALGEBRA_OPERATOR_IO_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_IO_HPP_

#include "ieompp/algebra/operator/get_index.hpp"
#include "ieompp/algebra/operator/operator.hpp"
#include "ieompp/tuple.hpp"

#include <ostream>
#include <tuple>
#include <utility>

namespace ieompp
{
    namespace algebra
    {
        struct BoolSpinPrinter {
#ifndef IEOMPP_DISABLE_ARROWS
            static void print(std::ostream& strm, bool b) {
                static const std::string up   = u8"↑";
                static const std::string down = u8"↓";
                strm << (b ? up : down);
            }
#else
            static void print(std::ostream& strm, bool b) { strm << b; }
#endif
        };

        template <typename T>
        struct DefaultPrinter {
            static void print(std::ostream& strm, const T& t) { strm << t; }
        };

        using DefaultPrinters = std::tuple<std::pair<bool, BoolSpinPrinter>>;

        template <typename T, typename PrinterList>
        struct GetPrinterType {
        };

        template <typename T>
        struct GetPrinterType<T, std::tuple<>> {
            using Type = DefaultPrinter<T>;
        };

        template <typename T, typename U, typename... Ts>
        struct GetPrinterType<T, std::tuple<U, Ts...>> {
            using Type = typename std::
                conditional<std::is_same<T, typename U::first_type>::value, typename U::second_type,
                            typename GetPrinterType<T, std::tuple<Ts...>>::Type>::type;
        };

        template <typename Index>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index>& op)
        {
            strm << 'c';
            if(op.creator) {
                strm << "^†";
            }
            strm << "_{";
            GetPrinterType<Index, DefaultPrinters>::Type::print(strm, op.index);
            strm << '}';
            return strm;
        }

        template <typename Index1, typename Index2>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index1, Index2>& op)
        {
            strm << 'c';
            if(op.creator) {
                strm << "^†";
            }
            strm << "_{";
            GetPrinterType<Index1, DefaultPrinters>::Type::print(strm, op.index1);
            strm << ',';
            GetPrinterType<Index2, DefaultPrinters>::Type::print(strm, op.index2);
            strm << '}';
            return strm;
        }

        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct PrintHelper {
                static void print(std::ostream& strm, const Operator& op)
                {
                    using Printer = typename GetPrinterType<typename IndexType<I, Operator>::Type,
                                                            DefaultPrinters>::Type;
                    Printer::print(strm, get_index<I>(op));
                    strm << ',';
                    PrintHelper<Operator, I + 1, N>::print(strm, op);
                }
            };

            template <typename Operator, std::size_t N>
            struct PrintHelper<Operator, N, N> {
                static void print(std::ostream& strm, const Operator& op)
                {
                    using Printer = typename GetPrinterType<typename IndexType<N, Operator>::Type,
                                                            DefaultPrinters>::Type;
                    Printer::print(strm, get_index<N>(op));
                }
            };
        } // namespace detail

        template <typename... IndexTypes>
        std::ostream& operator<<(std::ostream& strm, const Operator<IndexTypes...>& op)
        {
            using Op = Operator<IndexTypes...>;

            strm << 'c';
            if(op.creator) {
                strm << "^†";
            }
            strm << "_{";
            detail::PrintHelper<Op, 0, Op::number_of_indices - 1>::print(strm, op);
            strm << '}';
            return strm;
        }
    } // namespace algebra
} // namespace ieompp

#endif
