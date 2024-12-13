#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

namespace {
namespace grammar {
    namespace dsl = lexy::dsl;
    struct line
    {
        static constexpr auto rule = dsl::peek(dsl::digit<>)
                >> dsl::integer<int> + dsl::whitespace(dsl::ascii::blank) + dsl::integer<int>;
        static constexpr auto value = lexy::construct<pair<int, int>>;
    };
    struct input
    {
        static constexpr auto rule = dsl::list(dsl::p<line>, dsl::trailing_sep(dsl::ascii::newline));
        static constexpr auto value = lexy::as_list<vector<pair<int, int>>>;
    };
}
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    vector<int> ls, rs;
    for (auto const& line : result.value())
    {
        ls.push_back(get<0>(line));
        rs.push_back(get<1>(line));
    }
    sort(ls.begin(), ls.end());
    sort(rs.begin(), rs.end());
    int d = 0;
    for (int i = 0; i < ls.size(); ++i)
        d += abs(ls[i] - rs[i]);
    cout << d << '\n';
    int s = 0;
    for (auto&& l : ls)
        s += l * count(rs.begin(), rs.end(), l);
    cout << s << '\n';
    return 0;
}
