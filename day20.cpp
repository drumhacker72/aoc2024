#include <iostream>
#include <queue>
#include <ranges>
#include <set>
#include <vector>

using namespace std;

constexpr size_t grid_size = 71;

using Point = pair<size_t, size_t>;

struct Node
{
    int steps;
    Point pos;
    bool operator<(Node const& n) const { return steps > n.steps; }
};

vector<vector<int>> find_distances(vector<string> const& grid, Point start)
{
    using Node = pair<int, Point>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({0, start});
    vector<vector<int>> distances (grid.size(), vector<int>(grid[0].size(), -1));
    while (!pq.empty())
    {
        auto [steps, p] = pq.top();
        auto [x, y] = p;
        pq.pop();
        if (distances[y][x] != -1)
            continue;
        distances[y][x] = steps;
        if (x > 0 && grid[y][x-1] != '#')
            pq.push({steps+1, {x-1, y}});
        if (x < grid[y].size()-1 && grid[y][x+1] != '#')
            pq.push({steps+1, {x+1, y}});
        if (y > 0 && grid[y-1][x] != '#')
            pq.push({steps+1, {x, y-1}});
        if (y < grid.size()-1 && grid[y+1][x] != '#')
            pq.push({steps+1, {x, y+1}});
    }
    return distances;
}

int find_cheats(
        vector<string> const& grid,
        vector<vector<int>> const& from_start,
        vector<vector<int>> const& from_end,
        int target_distance,
        int cheat_length)
{
    int num_cheats = 0;
    for (int y1 = 0; y1 < grid.size(); ++y1)
    {
        for (int x1 = 0; x1 < grid[y1].size(); ++x1)
        {
            if (grid[y1][x1] == '#')
                continue;
            int ds = from_start[y1][x1];
            for (int y2 = y1 - cheat_length; y2 <= y1 + cheat_length; ++y2)
            {
                if (y2 < 0 || y2 >= grid.size())
                    continue;
                for (int x2 = x1 - cheat_length; x2 <= x1 + cheat_length; ++x2)
                {
                    if (x2 < 0 || x2 >= grid[y2].size())
                        continue;
                    if (grid[y2][x2] == '#')
                        continue;
                    int dc = abs(x1 - x2) + abs(y1 - y2);
                    if (dc > cheat_length)
                        continue;
                    int de = from_end[y2][x2];
                    if (ds + dc + de <= target_distance)
                        ++num_cheats;
                }
            }
        }
    }
    return num_cheats;
}

int main()
{
    vector<string> grid;
    string line;
    Point start, end;
    while (getline(cin, line))
    {
        auto y = grid.size();
        if (auto x = line.find('S'); x != string::npos)
            start = {x, y};
        if (auto x = line.find('E'); x != string::npos)
            end = {x, y};
        grid.push_back(line);
    }
    auto from_start = find_distances(grid, start);
    auto from_end = find_distances(grid, end);
    int distance = from_start[end.second][end.first];
    cout << find_cheats(grid, from_start, from_end, distance - 100, 2) << '\n';
    cout << find_cheats(grid, from_start, from_end, distance - 100, 20) << '\n';

    return 0;
}
