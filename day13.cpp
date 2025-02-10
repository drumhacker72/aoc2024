#include <iostream>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

using namespace std;

struct Vec2
{
    uint64_t x;
    uint64_t y;
    constexpr Vec2 scale(uint64_t s) const { return {x*s, y*s}; }
    constexpr Vec2 operator+(Vec2 v) const { return {x+v.x, y+v.y}; }
    constexpr bool operator==(Vec2 v) const { return x==v.x && y==v.y; }
};

struct Machine
{
    Vec2 a;
    Vec2 b;
    Vec2 prize;
};

namespace grammar
{
    namespace dsl = lexy::dsl;
    template <char B>
    struct button
    {
        static constexpr auto rule = LEXY_LIT("Button ") >> dsl::lit_c<B>
            + LEXY_LIT(": X+") + dsl::integer<uint64_t>
            + LEXY_LIT(", Y+") + dsl::integer<uint64_t>;
        static constexpr auto value = lexy::construct<Vec2>;
    };
    struct prize
    {
        static constexpr auto rule = LEXY_LIT("Prize: X=") + dsl::integer<uint64_t>
            + LEXY_LIT(", Y=") + dsl::integer<uint64_t>;
        static constexpr auto value = lexy::construct<Vec2>;
    };
    struct machine
    {
        static constexpr auto rule = dsl::p<button<'A'>> >> dsl::newline
            + dsl::p<button<'B'>> + dsl::newline
            + dsl::p<prize> + dsl::if_(dsl::newline);
        static constexpr auto value = lexy::construct<Machine>;
    };
    struct machine_list
    {
        static constexpr auto rule = dsl::list(dsl::p<machine>, dsl::trailing_sep(dsl::newline));
        static constexpr auto value = lexy::as_list<vector<Machine>>;
    };
    struct input
    {
        static constexpr auto rule = dsl::p<machine_list> + dsl::eof;
        static constexpr auto value = lexy::as_list<vector<Machine>>;
    };
}

optional<pair<uint64_t, uint64_t>> ratio_differences(uint64_t n1, uint64_t n2, uint64_t d1, uint64_t d2)
{
    if (n1 <= n2 && d1 < d2)
        return make_pair(n2 - n1, d2 - d1);
    else if (n2 <= n1 && d2 < d1)
        return make_pair(n1 - n2, d1 - d2);
    else
        return nullopt;
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    vector<Machine> machines = result.value();
    int total_tokens1 = 0;
    for (auto const& machine : machines)
    {
        optional<int> min_tokens;
        for (int a = 0; a <= 100; ++a)
        {
            for (int b = 0; b <= 100; ++b)
            {
                if (machine.a.scale(a) + machine.b.scale(b) == machine.prize)
                {
                    int tokens = 3*a + b;
                    if (!min_tokens || tokens < *min_tokens)
                        min_tokens = tokens;
                }
            }
        }
        if (min_tokens)
            total_tokens1 += *min_tokens;
    }
    cout << total_tokens1 << '\n';
    // a * ax + b * bx = px
    // a * ay + b * by = py
    // a,b integers; minimize 3a + b
    // a * ax * by + b * bx * by = px * by
    // a * ay * bx + b * bx * by = py * bx
    // a = (px * by - py * bx) / (ax * by - ay * bx)
    // b = (px * ay - py * ax) / (bx * ay - by * ax)
    uint64_t total_tokens2 = 0;
    for (auto m : machines)
    {
        m.prize = m.prize + Vec2{10000000000000, 10000000000000};
        auto ar = ratio_differences(m.prize.x * m.b.y, m.prize.y * m.b.x, m.a.x * m.b.y, m.a.y * m.b.x);
        auto br = ratio_differences(m.prize.x * m.a.y, m.prize.y * m.a.x, m.b.x * m.a.y, m.b.y * m.a.x);
        if (!ar || !br)
            continue;
        if (ar->first % ar->second == 0 && br->first % br->second == 0)
            total_tokens2 += 3 * ar->first / ar->second + br->first / br->second;
    }
    cout << total_tokens2 << '\n';

    return 0;
}
