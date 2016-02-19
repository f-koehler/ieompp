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
    }
}
