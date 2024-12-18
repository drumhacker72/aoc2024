#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

using Dependencies = map<int, set<int>>;
using Update = vector<int>;

namespace {
namespace grammar {
    namespace dsl = lexy::dsl;
    struct order_rule_list
    {
        static constexpr auto rule = [] {
            auto line = dsl::integer<int> >> dsl::vbar + dsl::integer<int>;
            return dsl::list(line, dsl::trailing_sep(dsl::ascii::newline));
        }();
        static constexpr auto value = lexy::fold_inplace<Dependencies>(
            initializer_list<Dependencies::value_type>{},
            [](auto& deps, int before, int after) { deps[after].insert(before); }
        );
    };
    struct update
    {
        static constexpr auto rule = dsl::list(dsl::integer<int>, dsl::sep(dsl::comma));
        static constexpr auto value = lexy::as_list<Update>;
    };
    struct update_list
    {
        static constexpr auto rule = dsl::list(dsl::p<update>, dsl::trailing_sep(dsl::ascii::newline));
        static constexpr auto value = lexy::as_list<vector<Update>>;
    };
    struct input
    {
        static constexpr auto rule = dsl::p<order_rule_list> + dsl::ascii::newline + dsl::p<update_list> + dsl::eof;
        static constexpr auto value = lexy::construct<pair<Dependencies, vector<Update>>>;
    };
}

Dependencies deps;

bool in_order(vector<int> const& update)
{
    set<int> seen;
    for (auto page : update)
    {
        for (auto dep : deps[page])
        {
            if (ranges::contains(update, dep) && !seen.contains(dep))
                return false;
        }
        seen.insert(page);
    }
    return true;
}

vector<int> make_order(set<int> pages)
{
    vector<int> update;
    while (!pages.empty())
    {
        for (auto page : pages)
        {
            bool okay = true;
            for (auto dep : deps[page])
            {
                if (pages.contains(dep))
                {
                    okay = false;
                    break;
                }
            }
            if (okay)
            {
                pages.erase(page);
                update.push_back(page);
                break;
            }
        }
    }
    return update;
}
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    deps = get<0>(result.value());
    auto updates = get<1>(result.value());

    int corrects = 0;
    int incorrects = 0;
    for (auto const& update : updates)
    {
        if (in_order(update))
            corrects += update[update.size()/2];
        else
        {
            auto fixed = make_order(set(update.begin(), update.end()));
            incorrects += fixed[fixed.size()/2];
        }
    }
    cout << corrects << '\n';
    cout << incorrects << '\n';
    return 0;
}
