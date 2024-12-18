#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

using namespace std;

#include <lexy/action/parse.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

using Report = vector<int>;

namespace {
namespace grammar {
    namespace dsl = lexy::dsl;
    struct report
    {
        static constexpr auto rule = dsl::list(dsl::integer<int>, dsl::sep(dsl::lit_c<' '>));
        static constexpr auto value = lexy::as_list<Report>;
    };
    struct report_list
    {
        static constexpr auto rule = dsl::list(dsl::p<report>, dsl::trailing_sep(dsl::ascii::newline)) + dsl::eof;
        static constexpr auto value = lexy::as_list<vector<Report>>;
    };
    using input = report_list;
}

bool is_safe(const Report& r)
{
    vector<int> ds;
    for (int i = 0; i < r.size() - 1; ++i)
        ds.push_back(r[i+1] - r[i]);
    bool inc = all_of(ds.begin(), ds.end(), [](int d) { return d > 0; });
    bool dec = all_of(ds.begin(), ds.end(), [](int d) { return d < 0; });
    bool close = all_of(ds.begin(), ds.end(), [](int d) { return 1 <= abs(d) && abs(d) <= 3; });
    return (inc || dec) && close;
}
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    vector<Report> rs = result.value();
    int num_safe = 0;
    for (auto&& r : rs)
    {
        if (is_safe(r))
            ++num_safe;
    }
    cout << num_safe << '\n';
    int close_to_safe = 0;
    for (auto&& r : rs)
    {
        bool s = false;
        if (is_safe(r))
            s = true;
        else
        {
            for (int i = 0; i < r.size(); ++i)
            {
                if (is_safe(r | views::enumerate
                              | views::filter([i](auto&& e) { return i != get<0>(e); })
                              | views::values
                              | ranges::to<Report>()))
                {
                    s = true;
                    break;
                }
            }
        }
        if (s)
            ++close_to_safe;
    }
    cout << close_to_safe << '\n';
	return 0;
}
