#ifndef SITE_OCCUPATION_HPP_
#define SITE_OCCUPATION_HPP_

#include "../include/logging.hpp"

#include <ieompp/models/hubbard_real_space/site_occupation.hpp>

template <typename Basis, typename ExpectationValueFunction>
auto init_site_occupation(const Basis& basis, const ExpectationValueFunction& ev)
{
    static_cast<void>(basis);
    get_loggers().main->info(u8"Init <n_{0,↑}> observable for half-filled model");
    auto site_occupation = ieompp::models::hubbard_real_space::SiteOccupation<double, Basis>{
        [&ev](const typename Basis::Monomial::Operator& a,
              const typename Basis::Monomial::Operator& b) { return ev(a, b); }};
    get_loggers().main->info(u8"Finished initializing <n_{0,↑}> observable");
    return site_occupation;
}

#endif
