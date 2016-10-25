#ifndef IEOMPP_ALGEBRA_OPERATOR_IO_HPP_
#define IEOMPP_ALGEBRA_OPERATOR_IO_HPP_

#include <ostream>
#include <tuple>
#include <utility>

#include <ieompp/algebra/operator/get_index.hpp>
#include <ieompp/algebra/operator/operator.hpp>
#include <ieompp/tuple.hpp>

namespace ieompp
{
    namespace algebra
    {
        struct BoolSpinPrinter {
#ifndef IEOMPP_DISABLE_ARROWS
            static void print(std::ostream& strm, bool b) { strm << (b ? u8"↑" : u8"↓"); }
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
        struct get_printer_type {
        };

        template <typename T>
        struct get_printer_type<T, std::tuple<>> {
            using type = DefaultPrinter<T>;
        };

        template <typename T, typename U, typename... Ts>
        struct get_printer_type<T, std::tuple<U, Ts...>> {
            using type = typename std::
                conditional<std::is_same<T, typename U::first_type>::value, typename U::second_type,
                            typename get_printer_type<T, std::tuple<Ts...>>::type>::type;
        };

        template <typename Index>
        std::ostream& operator<<(std::ostream& strm, const Operator<Index>& op)
        {
            strm << 'c';
            if(op.creator) {
                strm << "^†";
            }
            strm << "_{";
            get_printer_type<Index, DefaultPrinters>::type::print(strm, op.index);
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
            get_printer_type<Index1, DefaultPrinters>::type::print(strm, op.index1);
            strm << ',';
            get_printer_type<Index2, DefaultPrinters>::type::print(strm, op.index2);
            strm << '}';
            return strm;
        }

        namespace detail
        {
            template <typename Operator, std::size_t I, std::size_t N>
            struct PrintHelper {
                static void print(std::ostream& strm, const Operator& op)
                {
                    using Printer = typename get_printer_type<typename IndexType<I, Operator>::type,
                                                              DefaultPrinters>::type;
                    Printer::print(strm, get_index<I>(op));
                    strm << ',';
                    PrintHelper<Operator, I + 1, N>::print(strm, op);
                }
            };

            template <typename Operator, std::size_t N>
            struct PrintHelper<Operator, N, N> {
                static void print(std::ostream& strm, const Operator& op)
                {
                    using Printer = typename get_printer_type<typename IndexType<N, Operator>::type,
                                                              DefaultPrinters>::type;
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
