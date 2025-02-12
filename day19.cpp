#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <vector>

using namespace std;

uint64_t matches(vector<string> const& towels, string_view design)
{
    static map<string_view, uint64_t> memo {{""sv, 1}};
    if (auto it = memo.find(design); it != memo.end())
        return it->second;
    uint64_t ms = 0;
    for (auto const& towel : towels)
    {
        if (design.starts_with(towel))
            ms += matches(towels, design.substr(towel.size()));
    }
    memo[design] = ms;
    return ms;
}

int main()
{
    string line;
    getline(cin, line);
    vector<string> towels;
    for (auto towel : views::split(line, ", "sv))
        towels.push_back(string{string_view{towel}});
    getline(cin, line);
    if (!line.empty())
        throw runtime_error{"bad input"};
    vector<string> designs;
    while (getline(cin, line))
        designs.push_back(line);

    int possible = 0;
    uint64_t ways = 0;
    regex re {"(" + ranges::to<string>(views::join_with(towels, "|"sv)) + ")*",
        regex::ECMAScript | regex::optimize};
    for (auto const& design : designs)
    {
        if (!regex_match(design, re))
            continue;
        ++possible;
        ways += matches(towels, design);
    }
    cout << possible << '\n';
    cout << ways << '\n';
    return 0;
}
