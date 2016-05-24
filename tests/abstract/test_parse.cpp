#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ieompp/abstract/parse.hpp"

using namespace ieompp;
using namespace abstract;

TEST_CASE("regexes::exponent")
{
    std::smatch m;
    std::string s = u8"†";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::exponent));
    REQUIRE(m[1] == u8"†");

    s = "\\dagger";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::exponent));
    REQUIRE(m[1] == "\\dagger");
}

TEST_CASE("regexes::index")
{
    std::smatch m;
    std::string s = u8"+ρ";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::index));
    REQUIRE(m[1] == u8"+ρ");

    s = "r'";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::index));
    REQUIRE(m[1] == u8"r'");

    // TODO: investigage why this fails
    /* s = "-2"; */
    /* REQUIRE(std::regex_match(s, m, parse_new::regexes::index)); */
    /* REQUIRE(m[1] == u8"-2"); */

    s = "R_1";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::index));
    REQUIRE(m[1] == u8"R_1");
}

TEST_CASE("regexes::spin")
{
    std::smatch m;
    std::string s = u8"+S";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::spin));
    REQUIRE(m[1] == u8"+S");

    s = "σ'";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::spin));
    REQUIRE(m[1] == u8"σ'");

    // TODO: investigage why this fails
    /* s = "-1"; */
    /* REQUIRE(std::regex_match(s, m, parse_new::regexes::spin)); */
    /* REQUIRE(m[1] == u8"-1"); */

    s = "S_1";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::spin));
    REQUIRE(m[1] == u8"S_1");
}

TEST_CASE("regexes::subscript")
{
    std::smatch m;
    std::string s = u8"{a,↑}";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::subscript));
    REQUIRE(m[1] == u8"a");
    REQUIRE(m[2] == u8"↑");
}

TEST_CASE("regexes::creator")
{
    std::smatch m;
    std::string s = u8"c_{0,↓}^†";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::creator));
    REQUIRE(m[1] == "c");
    REQUIRE(m[2] == "0");
    REQUIRE(m[3] == "↓");
    REQUIRE(m[4] == "†");

    s = u8"c_{0,↓}^\\dagger";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::creator));
    REQUIRE(m[1] == "c");
    REQUIRE(m[2] == "0");
    REQUIRE(m[3] == "↓");
    REQUIRE(m[4] == "\\dagger");

    // TODO: fix this
    s = u8"c^†_{0,↓}";
    /* REQUIRE(std::regex_match(s, m, parse_new::regexes::creator)); */
    /* REQUIRE(m[1] == "c"); */
    /* REQUIRE(m[2] == "†"); */
    /* REQUIRE(m[3] == "↓"); */
    /* REQUIRE(m[4] == "0"); */

    // TODO: fix this
    /* s = u8"c^\\dagger_{0,↓}"; */
    /* REQUIRE(std::regex_match(s, m, parse_new::regexes::creator)); */
    /* REQUIRE(m[1] == "c"); */
    /* REQUIRE(m[2] == "\\dagger"); */
    /* REQUIRE(m[3] == "0"); */
    /* REQUIRE(m[4] == "↓"); */
}

TEST_CASE("regexes::annihilator")
{
    std::smatch m;
    std::string s = u8"c_{0,↓}";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::annihilator));
    REQUIRE(m[1] == "c");
    REQUIRE(m[2] == "0");
    REQUIRE(m[3] == u8"↓");
}

TEST_CASE("regexes::commutator")
{
    std::smatch m;
    std::string s = u8"[c_{1,↑}^† c_{0,↑};c_{0,↑}]";
    REQUIRE(std::regex_match(s, m, parse_new::regexes::commutator));
    REQUIRE(m[1] == u8"c_{1,↑}^† c_{0,↑}"); 
    REQUIRE(m[2] == u8"c_{0,↑}");
}

TEST_CASE("parse_commutator")
{
    auto result = parse_new::parse_commutator(u8"[c_{1,↑}^† c_{0,↑};c_{0,↑}]");
    REQUIRE(std::get<0>(result));
    REQUIRE(std::get<1>(result) == u8"c_{1,↑}^† c_{0,↑}");
    REQUIRE(std::get<2>(result) == u8"c_{0,↑}");
}

/* TEST_CASE("is_commutator") */
/* { */
/*     auto result = parse::is_commutator("[a;b]"); */
/*     REQUIRE(std::get<0>(result)); */
/*     REQUIRE(std::get<1>(result) == "a"); */
/*     REQUIRE(std::get<2>(result) == "b"); */
/* } */

/* TEST_CASE("is_operator") */
/* { */
/*     SECTION("plain ASCII") */
/*     { */
/*         auto result = parse::is_operator(u8"c_{k_1,up}^\\dagger"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k_1"); */
/*         REQUIRE(std::get<2>(result) == "up"); */
/*         REQUIRE(std::get<3>(result)); */

/*         result = parse::is_operator("c_{k,\\spinU}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k"); */
/*         REQUIRE(std::get<2>(result) == "\\spinU"); */
/*         REQUIRE(!std::get<3>(result)); */
/*     } */

/*     SECTION("unicode") */
/*     { */
/*         auto result = parse::is_operator(u8"c_{k,σ}^†"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k"); */
/*         REQUIRE(std::get<2>(result) == "σ"); */
/*         REQUIRE(std::get<3>(result)); */

/*         result = parse::is_operator("c_{k,σ}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k"); */
/*         REQUIRE(std::get<2>(result) == "σ"); */
/*         REQUIRE(!std::get<3>(result)); */
/*     } */

/*     SECTION("complex indices") */
/*     { */
/*         auto result = parse::is_operator("c_{k+q,σ-s}^†"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k+q"); */
/*         REQUIRE(std::get<2>(result) == "σ-s"); */
/*         REQUIRE(std::get<3>(result)); */

/*         result = parse::is_operator("c_{k+q,σ-s}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k+q"); */
/*         REQUIRE(std::get<2>(result) == "σ-s"); */
/*         REQUIRE(!std::get<3>(result)); */
/*     } */
/* } */

/* TEST_CASE("is_kronecker") */
/* { */
/*     SECTION("plain ASCII") */
/*     { */
/*         auto result = parse::is_kronecker("\\delta_{k_1,k_2}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k_1"); */
/*         REQUIRE(std::get<2>(result) == "k_2"); */
/*     } */

/*     SECTION("unicode") */
/*     { */
/*         auto result = parse::is_kronecker("δ_{←,→}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "←"); */
/*         REQUIRE(std::get<2>(result) == "→"); */
/*     } */

/*     SECTION("complex indices") */
/*     { */
/*         auto result = parse::is_kronecker("δ_{k_1+q,σ-σ'}"); */
/*         REQUIRE(std::get<0>(result)); */
/*         REQUIRE(std::get<1>(result) == "k_1+q"); */
/*         REQUIRE(std::get<2>(result) == "σ-σ'"); */
/*     } */
/* } */

/* TEST_CASE("is_number") */
/* { */
/*     auto result = parse::is_number("(12.123e-12,-4.0)"); */
/*     REQUIRE(std::get<0>(result)); */
/*     REQUIRE(std::get<1>(result) == "12.123e-12"); */
/*     REQUIRE(std::get<2>(result) == "-4.0"); */
/* } */

/* TEST_CASE("parse_term") */
/* { */
/*     auto expression = "c_{k_1,up}^\\dagger c_{k_2,up}^{\\dagger}"; */
/*     auto result = parse::parse_term(expression); */
/*     REQUIRE(result.operators.size() == 2); */
/* } */
