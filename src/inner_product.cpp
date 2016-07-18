#include <iostream>
using namespace std;

#include <ieompp/inner_product/inner_product.hpp>
#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
namespace algebra = ieompp::algebra;

int main()
{
    auto t1 =
        algebra::make_term(1., {algebra::make_creator(0, true), algebra::make_creator(0, false),
                                algebra::make_annihilator(0, false)});
    auto t2 = algebra::make_term(1., {algebra::make_creator(0, true)});

    ieompp::inner_product::InnerProductTable container;
    container.emplace(
        ieompp::inner_product::InnerProduct(ieompp::inner_product::get_term_structure(t1),
                                            ieompp::inner_product::get_term_structure(t2)));
}
