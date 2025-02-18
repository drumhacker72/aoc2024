#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

using namespace std;

enum class Gate { and_, or_, xor_ };

struct Connection
{
    string in1;
    Gate gate;
    string in2;
    string out;
};

namespace grammar
{
    namespace dsl = lexy::dsl;

    struct wire_value
    {
        static constexpr auto rule = dsl::integer<int, dsl::binary>;
        static constexpr auto value = lexy::as_integer<int>;
    };
    struct wire_name
    {
        static constexpr auto rule = dsl::identifier(dsl::ascii::alpha, dsl::ascii::alnum);
        static constexpr auto value = lexy::as_string<string>;
    };
    struct initial
    {
        static constexpr auto rule = dsl::p<wire_name> >> LEXY_LIT(": ") + dsl::p<wire_value>;
        static constexpr auto value = lexy::construct<pair<string, int>>;
    };
    struct initial_list
    {
        static constexpr auto rule = dsl::list(dsl::p<initial>, dsl::trailing_sep(dsl::newline));
        static constexpr auto value = lexy::as_collection<map<string, int>>;
    };
    struct and_
    {
        static constexpr auto rule = LEXY_LIT(" AND ");
        static constexpr auto value = lexy::constant(Gate::and_);
    };
    struct or_
    {
        static constexpr auto rule = LEXY_LIT(" OR ");
        static constexpr auto value = lexy::constant(Gate::or_);
    };
    struct xor_
    {
        static constexpr auto rule = LEXY_LIT(" XOR ");
        static constexpr auto value = lexy::constant(Gate::xor_);
    };
    struct gate
    {
        static constexpr auto rule = dsl::p<and_> | dsl::p<or_> | dsl::p<xor_>;
        static constexpr auto value = lexy::forward<Gate>;
    };
    struct connection
    {
        static constexpr auto rule = dsl::p<wire_name> >> dsl::p<gate> + dsl::p<wire_name> + LEXY_LIT(" -> ") + dsl::p<wire_name>;
        static constexpr auto value = lexy::construct<Connection>;
    };
    struct connection_list
    {
        static constexpr auto rule = dsl::list(dsl::p<connection>, dsl::trailing_sep(dsl::newline));
        static constexpr auto value = lexy::as_list<vector<Connection>>;
    };
    struct input
    {
        static constexpr auto rule = dsl::p<initial_list> + dsl::newline + dsl::p<connection_list> + dsl::eof;
        static constexpr auto value = lexy::construct<pair<map<string, int>, vector<Connection>>>;
    };
}

map<string, int> simulate(map<string, int> values, vector<Connection> const& connections)
{
    for (int t = 0; t < connections.size(); ++t)
    {
        for (auto const& [in1_n, gate, in2_n, out_n] : connections)
        {
            auto in1 = values.find(in1_n);
            auto in2 = values.find(in2_n);
            if (!values.contains(out_n) && in1 != values.end() && in2 != values.end())
            {
                switch (gate)
                {
                case Gate::and_:
                    values[out_n] = in1->second & in2->second;
                    break;
                case Gate::or_:
                    values[out_n] = in1->second | in2->second;
                    break;
                case Gate::xor_:
                    values[out_n] = in1->second ^ in2->second;
                }
            }
        }
    }
    return values;
}

uint64_t get_number(map<string, int> const& values, char start)
{
    char key[4];
    key[0] = start;
    key[3] = 0;
    uint64_t output = 0;
    uint64_t cursor = 1;
    for (char i = '0'; i <= '9'; ++i)
    {
        key[1] = i;
        for (char j = '0'; j <= '9'; ++j)
        {
            key[2] = j;
            auto it = values.find(key);
            if (it == values.end())
                return output;
            if (it->second)
                output |= cursor;
            cursor <<= 1;
        }
    }
    return output;
}

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    auto [values, connections] = result.value();
    auto values1 = simulate(values, connections);
    cout << get_number(values1, 'z') << '\n';

    //highlight oddities with ***, solve by hand
    map<string, tuple<string, Gate, string>> deps;
    for (auto [in1, gate, in2, out] : connections)
        deps[out] = {in1, gate, in2};
    char key[4];
    key[0] = 'z';
    key[3] = 0;
    set<string> last;
    for (char i = '0'; i <= '9'; ++i)
    {
        key[1] = i;
        for (char j = '0'; j <= '9'; ++j)
        {
            key[2] = j;
            if (!deps.contains(key))
                return 0;
            auto [dep1, g, dep2] = deps[key];
            cout << key << ": " << dep1 << ' ' << dep2 << '\n';
            if (g != Gate::xor_)
                cout << "***\n";
            if (deps.contains(dep1))
            {
                auto [dep11, g1, dep12] = deps[dep1];
                cout << '\t' << dep1 << ": " << dep11 << ' ' << dep12 << '\n';
                if (deps.contains(dep11))
                {
                    auto [dep111, g11, dep112] = deps[dep11];
                    cout << "\t\t" << dep11 << ": " << dep111 << ' ' << dep112 << '\n';
                    if (!isdigit(dep111[1]) && last != set{dep111, dep112})
                        cout << "\t\t***\n";
                    auto [dep121, g12, dep122] = deps[dep12];
                    cout << "\t\t" << dep12 << ": " << dep121 << ' ' << dep122 << '\n';
                    if (!isdigit(dep121[1]) && last != set{dep121, dep122})
                        cout << "\t\t***\n";
                }
                else if (g1 != Gate::xor_)
                    cout << "\t***\n";
            }
            if (deps.contains(dep2))
            {
                auto [dep21, g2, dep22] = deps[dep2];
                cout << '\t' << dep2 << ": " << dep21 << ' ' << dep22 << '\n';
                if (deps.contains(dep21))
                {
                    auto [dep211, g21, dep212] = deps[dep21];
                    cout << "\t\t" << dep21 << ": " << dep211 << ' ' << dep212 << '\n';
                    if (!isdigit(dep211[1]) && last != set{dep211, dep212})
                        cout << "\t\t***\n";
                    auto [dep221, g22, dep222] = deps[dep22];
                    cout << "\t\t" << dep22 << ": " << dep221 << ' ' << dep222 << '\n';
                    if (!isdigit(dep221[1]) && last != set{dep221, dep222})
                        cout << "\t\t***\n";
                } else if (g2 != Gate::xor_)
                    cout << "\t***\n";
            }
            last = {dep1, dep2};
        }
    }
    /*
z25: tsw XOR vst
    tsw: y25 XOR x25
    vst: ngb OR krb
z26: cbj XOR fnf
    cbj: hcp OR jgn
        hcp: y25 AND x25
        jgn: vst AND tsw
    fnf: y26 XOR x26
z27: vgg OR pph
    vgg: x27 AND y27
    pph: gcc AND ntr

bfq: ntr XOR gcc
    ntr: x27 XOR y27

*** bfq <=> z27

z31: y31 AND x31

hkh: fqh XOR ctc
    fqh: x31 XOR y31

*** z31 <=> hkh

z18: jss AND mdg

hmt: mdg XOR yss
    jss: x18 XOR y18

*** z18 <=> hmt

z38: cjr XOR vvr
z39: hsf XOR bng
    hsf: mdm OR cwb
        mdm: vvr AND cjr
        cwb: y38 AND x38
    bng: y39 AND x39

z40: vbm XOR qqb
    vbm: tkf OR fjp
        fjp: x39 XOR y39

*** bng <=> fjp

bfq,bng,fjp,hkh,hmt,z18,z27,z31
    */

    return 0;
}
