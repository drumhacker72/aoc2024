#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

using namespace std;

constexpr array numpad {"789"sv, "456"sv, "123"sv, "\0000A"sv};
constexpr array dirpad {"\000^A"sv, "<v>"sv};

using Point = pair<int, int>;

template <class T>
vector<string> find_paths(T const& pad, Point start, Point end)
{
    char c = pad[start.second][start.first];
    if (!c)
        return {};
    if (start == end)
        return {"A"};
    vector<string> paths;
    if (start.first != end.first)
    {
        int dx = start.first < end.first ? 1 : -1;
        char c = start.first < end.first ? '>' : '<';
        paths = find_paths(pad, {start.first+dx, start.second}, end);
        for (auto& path : paths)
            path = c + path;
    }
    if (start.second != end.second)
    {
        int dy = start.second < end.second ? 1 : -1;
        char c = start.second < end.second ? 'v' : '^';
        auto ypaths = find_paths(pad, {start.first, start.second+dy}, end);
        for (auto& ypath : ypaths)
            ypath = c + ypath;
        paths.insert(paths.end(), ypaths.begin(), ypaths.end());
    }
    return paths;
}

using Paths_map = map<char, map<char, vector<string>>>;

template <typename T>
Paths_map map_paths(T const& pad)
{
    Paths_map paths;
    for (int y1 = 0; y1 < pad.size(); ++y1)
    {
        for (int x1 = 0; x1 < pad[y1].size(); ++x1)
        {
            char c1 = pad[y1][x1];
            if (!c1)
                continue;
            for (int y2 = 0; y2 < pad.size(); ++y2)
            {
                for (int x2 = 0; x2 < pad[y2].size(); ++x2)
                {
                    char c2 = pad[y2][x2];
                    if (!c2)
                        continue;
                    paths[c1][c2] = find_paths(pad, {x1, y1}, {x2, y2});
                }
            }
        }
    }
    return paths;
}

using Cost_map = map<char, map<char, uint64_t>>;

constexpr auto dirkeys = "^<v>A"sv;

int main()
{
    auto numpaths = map_paths(numpad);
    auto dirpaths = map_paths(dirpad);

    Cost_map costs, costs2, costs25;
    for (char c1 : dirkeys)
    {
        for (char c2 : dirkeys)
            costs[c1][c2] = 1;
    }

    for (int i = 0; i < 25; ++i)
    {
        if (i == 2)
            costs2 = costs;
        Cost_map new_costs;
        for (char c1 : dirkeys)
        {
            for (char c2 : dirkeys)
            {
                optional<uint64_t> best_cost;
                for (auto path : dirpaths[c1][c2])
                {
                    char last = 'A';
                    uint64_t cost = 0;
                    for (char button : path)
                    {
                        cost += costs[last][button];
                        last = button;
                    }
                    if (!best_cost || cost < *best_cost)
                        best_cost = cost;
                }
                new_costs[c1][c2] = *best_cost;
            }
        }
        costs = std::move(new_costs);
    }
    costs25 = std::move(costs);

    constexpr int num_codes = 5;
    array<string, num_codes> codes;
    for (int i = 0; i < num_codes; ++i)
        cin >> codes[i];
    uint64_t complexity2 = 0;
    uint64_t complexity25 = 0;
    for (auto const& code : codes)
    {
        uint64_t total_cost2 = 0;
        uint64_t total_cost25 = 0;
        char last_num = 'A';
        for (char num : code)
        {
            optional<uint64_t> best_cost2;
            optional<uint64_t> best_cost25;
            for (auto path : numpaths[last_num][num])
            {
                char last_dir = 'A';
                uint64_t cost2 = 0;
                uint64_t cost25 = 0;
                for (char dir : path)
                {
                    cost2 += costs2[last_dir][dir];
                    cost25 += costs25[last_dir][dir];
                    last_dir = dir;
                }
                if (!best_cost2 || cost2 < *best_cost2)
                    best_cost2 = cost2;
                if (!best_cost25 || cost25 < *best_cost25)
                    best_cost25 = cost25;
            }
            last_num = num;
            total_cost2 += *best_cost2;
            total_cost25 += *best_cost25;
        }
        complexity2 += stoi(code) * total_cost2;
        complexity25 += stoi(code) * total_cost25;
    }
    cout << complexity2 << '\n';
    cout << complexity25 << '\n';
    return 0;
}
