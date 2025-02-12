#include <algorithm>
#include <iostream>
#include <ranges>
#include <set>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

using namespace std;

namespace grammar
{
    namespace dsl = lexy::dsl;
    template <char R>
    struct register_line
    {
        static constexpr auto rule = LEXY_LIT("Register ") + dsl::lit_c<R>
            + LEXY_LIT(": ") + dsl::integer<uint64_t> + dsl::newline;
        static constexpr auto value = lexy::construct<uint64_t>;
    };
    struct program
    {
        static constexpr auto rule = dsl::list(dsl::integer<uint8_t>, dsl::sep(dsl::comma));
        static constexpr auto value = lexy::as_list<vector<uint8_t>>;
    };
    struct program_line
    {
        static constexpr auto rule = LEXY_LIT("Program: ") + dsl::p<program> + dsl::if_(dsl::newline);
        static constexpr auto value = program::value;
    };
    struct input
    {
        static constexpr auto rule = dsl::p<register_line<'A'>>
            + dsl::p<register_line<'B'>>
            + dsl::p<register_line<'C'>>
            + dsl::newline
            + dsl::p<program_line>
            + dsl::eof;
        static constexpr auto value = lexy::construct<tuple<uint64_t, uint64_t, uint64_t, vector<uint8_t>>>;
    };
}

vector<uint8_t> run(uint64_t a, uint64_t b, uint64_t c, vector<uint8_t> const& program)
{
    size_t ip = 0;
    vector<uint8_t> output;
    while (ip < program.size())
    {
        auto opcode = program[ip++];
        uint64_t literal_operand = program[ip++];
        optional<uint8_t> combo_operand;
        switch (literal_operand)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            combo_operand = literal_operand;
            break;
        case 4:
            combo_operand = a;
            break;
        case 5:
            combo_operand = b;
            break;
        case 6:
            combo_operand = c;
            break;
        default:
            break;
        }
        switch (opcode)
        {
        case 0: //adv
            a = a >> *combo_operand;
            break;
        case 1: //bxl
            b = b ^ literal_operand;
            break;
        case 2: //bst
            b = *combo_operand % 8;
            break;
        case 3: //jnz
            if (a)
                ip = literal_operand;
            break;
        case 4: //bxc
            b = b ^ c;
            break;
        case 5: //out
            output.push_back(*combo_operand % 8);
            break;
        case 6: //bdv
            b = a >> *combo_operand;
            break;
        case 7: //cdv
            c = a >> *combo_operand;
            break;
        default:
            throw runtime_error{"bad opcode"};
        }
    }
    return output;
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    auto [a, b, c, program] = result.value();
    auto output = run(a, b, c, program);
    cout << static_cast<int>(output[0]);
    for (int i = 1; i < output.size(); ++i)
        cout << ',' << static_cast<int>(output[i]);
    cout << '\n';
    /* assuming program:
        bst a
        bxl 5
        cdv b
        bxc
        adv 3
        bxl 6
        out b
        jnz 0
    */
    set<uint64_t> as {0};
    for (auto out : views::reverse(program))
    {
        set<uint64_t> new_as;
        for (auto ra : as)
        {
            for (auto b : views::iota(0ULL, 8ULL))
            {
                uint64_t a = ra << 3 | b;
                b ^= 5;
                c = a >> b;
                b ^= c;
                b ^= 6;
                if (b % 8 == out)
                    new_as.insert(a);
            }
        }
        as = std::move(new_as);
    }
    cout << *as.begin() << '\n';
    return 0;
}
