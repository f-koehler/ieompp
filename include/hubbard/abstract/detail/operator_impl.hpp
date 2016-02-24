#include "hubbard/abstract/operator.hpp"

namespace hubbard
{
    namespace abstract
    {
        inline AbstractOperator make_creator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{true, AbstractIndex{index}, AbstractSpin{spin}};
        }

        inline AbstractOperator make_annihilator(const std::string& index, const std::string& spin)
        {
            return AbstractOperator{false, AbstractIndex{index}, AbstractSpin{spin}};
        }

        std::ostream& operator<<(std::ostream& strm,
                                 const hubbard::abstract::AbstractOperator::ContractionType& rhs)
        {
            strm << "\\left<" << std::get<0>(rhs) << " " << std::get<1>(rhs) << "\\right>";
            return strm;
        }
    }
}
