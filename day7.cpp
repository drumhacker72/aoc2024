#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

using namespace std;

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

namespace {

struct Equation
{
    int64_t test_value;
    vector<int64_t> numbers;
};

namespace grammar {
    namespace dsl = lexy::dsl;
    struct number_list
    {
        static constexpr auto rule = dsl::list(dsl::integer<int64_t>, dsl::sep(dsl::lit_c<' '>));
        static constexpr auto value = lexy::as_list<vector<int64_t>>;
    };
    struct equation
    {

        static constexpr auto rule = dsl::integer<int64_t> >> LEXY_LIT(": ") + dsl::p<number_list>;
        static constexpr auto value = lexy::construct<Equation>;
    };
    struct equation_list
    {
        static constexpr auto rule = dsl::list(dsl::p<equation>, dsl::trailing_sep(dsl::ascii::newline));
        static constexpr auto value = lexy::as_list<vector<Equation>>;
    };
    using input = equation_list;
}

bool operators_found1(int64_t test, int64_t acc, ranges::forward_range auto numbers)
{
    if (acc > test)
        return false;
    if (numbers.empty())
        return test == acc;
    return operators_found1(test, acc + numbers.front(), numbers | views::drop(1))
        || operators_found1(test, acc * numbers.front(), numbers | views::drop(1));
}

bool operators_found1(int64_t test, ranges::forward_range auto numbers)
{
    assert(!numbers.empty());
    return operators_found1(test, numbers.front(), numbers | views::drop(1));
}

int64_t concat(int64_t a, int64_t b)
{
    ostringstream ss;
    ss << a << b;
    return stoll(ss.str());
}

bool operators_found2(int64_t test, int64_t acc, ranges::forward_range auto numbers)
{
    if (acc > test)
        return false;
    if (numbers.empty())
        return test == acc;
    return operators_found2(test, acc + numbers.front(), numbers | views::drop(1))
        || operators_found2(test, acc * numbers.front(), numbers | views::drop(1))
        || operators_found2(test, concat(acc, numbers.front()), numbers | views::drop(1));
}

bool operators_found2(int64_t test, ranges::forward_range auto numbers)
{
    assert(!numbers.empty());
    return operators_found2(test, numbers.front(), numbers | views::drop(1));
}
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    vector<Equation> equations = result.value();
    int64_t calibration1 = 0;
    int64_t calibration2 = 0;
    for (auto const& equation : equations)
    {
        if (operators_found1(equation.test_value, span(equation.numbers)))
            calibration1 += equation.test_value;
        if (operators_found2(equation.test_value, span(equation.numbers)))
            calibration2 += equation.test_value;
    }
    cout << calibration1 << '\n';
    cout << calibration2 << '\n';
    return 0;
}
