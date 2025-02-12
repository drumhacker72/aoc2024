#include <iostream>
#include <queue>
#include <ranges>
#include <set>
#include <vector>

using namespace std;

constexpr size_t grid_size = 71;

struct Node
{
    int steps;
    int x;
    int y;
    bool operator<(Node const& n) const { return steps > n.steps; }
};

optional<int> find_path(vector<vector<bool>> const& grid)
{
    priority_queue<Node> pq;
    pq.push({0, 0, 0});
    set<pair<int, int>> seen;
    while (!pq.empty())
    {
        auto [steps, x, y] = pq.top();
        pq.pop();
        if (seen.contains({x, y}))
            continue;
        seen.insert({x, y});
        if (x == grid_size-1 && y == grid_size-1)
            return steps;
        if (x > 0 && !grid[y][x-1])
            pq.push({steps+1, x-1, y});
        if (x < grid_size-1 && !grid[y][x+1])
            pq.push({steps+1, x+1, y});
        if (y > 0 && !grid[y-1][x])
            pq.push({steps+1, x, y-1});
        if (y < grid_size-1 && !grid[y+1][x])
            pq.push({steps+1, x, y+1});
    }
    return nullopt;
}

int main()
{
    vector<pair<int, int>> ps;
    int x, y;
    while (cin >> x)
    {
        char comma;
        cin >> comma;
        if (comma != ',')
            throw runtime_error{"bad input"};
        cin >> y;
        ps.push_back({x, y});
    }
    vector<vector<bool>> grid (grid_size, vector<bool>(grid_size, false));
    for (auto [x, y] : ps | views::take(1024))
        grid[y][x] = true;
    cout << *find_path(grid) << '\n';
    for (auto [x, y] : ps | views::drop(1024))
    {
        grid[y][x] = true;
        if (!find_path(grid))
        {
            cout << x << ',' << y << '\n';
            break;
        }
    }
    return 0;
}
