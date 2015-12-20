#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hubbard/operators.hpp"
#include <iostream>

TEST_CASE("anticommutates", "[operators]") {
    SECTION("creator & creator") {
        auto ret = anticommutates(
            make_creator(0, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);

        ret = anticommutates(
            make_creator(0, false),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_creator(1, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
    }
    
    SECTION("annihilator & creator") {
        auto ret = anticommutates(
            make_annihilator(0, true),
            make_creator(0, true)
        );
        REQUIRE(ret == false);

        ret = anticommutates(
            make_annihilator(0, false),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_annihilator(1, true),
            make_creator(0, true)
        );
        REQUIRE(ret == true);
    }
    
    SECTION("annihilator & annihilator") {
        auto ret = anticommutates(
            make_annihilator(0, true),
            make_annihilator(0, true)
        );
        REQUIRE(ret == true);

        ret = anticommutates(
            make_annihilator(0, false),
            make_annihilator(0, true)
        );
        REQUIRE(ret == true);
        
        ret = anticommutates(
            make_annihilator(1, true),
            make_annihilator(0, true)
        );
        REQUIRE(ret == true);
    }
}

TEST_CASE("commutator", "[operators]") {
    using Operator = Operator<int, bool>;
    using Term = Term<Operator>;

    SECTION("[cc,c]") {
        Term term1 = {
            { 1., 0. },
            {
                make_creator(-1, true),
                make_annihilator(0, true)
            }
        };
        Term term2 = {
            { 1., 0. },
            {
                make_creator(0, true)
            }
        };
        auto result = commutate(term1, term2);
        REQUIRE(result.size() == 1);
        REQUIRE(result.front() == (Term {
            { 1., 0. },
            {
                make_creator(-1, true)
            }
        }));
    }

    SECTION("[c,cc]") {
        Term term1 = {
            { 1., 0. },
            {
                make_creator(0, true)
            }
        };
        Term term2 = {
            { 1., 0. },
            {
                make_creator(-1, true),
                make_annihilator(0, true)
            }
        };
        auto result = commutate(term1, term2);
        REQUIRE(result.size() == 1);
        REQUIRE(result.front() == (Term {
            { -1., 0. },
            {
                make_creator(-1, true)
            }
        }));
    }

    SECTION("[cc,cc]") {
        Term term1 = {
            { 1., 0. },
            {
                make_creator(0, true),
                make_annihilator(1, true)
            }
        };
        Term term2 = {
            { 1., 0. },
            {
                make_creator(1, true),
                make_annihilator(0, true)
            }
        };
        auto result = commutate(term1, term2);
        for(auto& term : result)
            std::cout << term << std::endl;
        REQUIRE(result[0] == (Term {
            { -1., 0. },
            {
                make_creator(1, true),
                make_annihilator(1, true)
            }
        }));
        REQUIRE(result[1] == (Term {
            { 1., 0. },
            {
                make_creator(0, true),
                make_annihilator(0, true)
            }
        }));
    }
}
