#include <iostream>
#include <set>
#include <vector>

using namespace std;

using Grid = vector<vector<bool>>;

struct Vec2
{
    int x;
    int y;
    Vec2 operator+(Vec2 const& other) const
    {
        return {x + other.x, y + other.y};
    }
    bool operator==(Vec2 const& other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator<(Vec2 const& other) const
    {
        return x < other.x || x == other.x && y < other.y;
    }
};

bool is_valid(Grid const& g, Vec2 const& p)
{
    return p.y >= 0 && p.y < g.size()
            && p.x >= 0 && p.x < g[p.y].size();
}

Vec2 turn_right(Vec2 const& delta)
{
    return {-delta.y, delta.x};
}

int num_visited(Grid const& g, Vec2 guard, Vec2 delta)
{
    set<Vec2> visited;
    for (;;)
    {
        visited.insert(guard);
        auto next = guard + delta;
        if (!is_valid(g, next))
            return visited.size();
        if (g[next.y][next.x])
            delta = turn_right(delta);
        else
            guard = next;
    }
}

bool creates_loop(Grid const& g, Vec2 guard, Vec2 delta, Vec2 const& obst)
{
    set<pair<Vec2, Vec2>> seen;
    for (;;)
    {
        seen.insert({guard, delta});
        auto next = guard + delta;
        if (!is_valid(g, next))
            return false;
        if (g[next.y][next.x] || next == obst)
            delta = turn_right(delta);
        else
            guard = next;
        if (seen.contains({guard, delta}))
            return true;
    }
}

int main()
{
    Grid grid;

    string line;
    Vec2 guard;
    int y = 0;
    while (getline(cin, line))
    {
        vector<bool> row;
        for (int x = 0; x < line.size(); ++x)
        {
            switch (line[x])
            {
            case '^':
                guard = {x, y};
                [[fallthrough]];
            case '.':
                row.push_back(false);
                break;
            case '#':
                row.push_back(true);
            }
        }
        grid.push_back(row);
        ++y;
    }

    Vec2 delta {0, -1}; //up

    cout << num_visited(grid, guard, delta) << '\n';

    int num_loops = 0;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[y].size(); ++x)
        {
            Vec2 obst {x, y};
            if (obst == guard)
                continue;
            if (grid[obst.y][obst.x])
                continue;
            if (creates_loop(grid, guard, delta, obst))
                ++num_loops;
        }
    }
    cout << num_loops << '\n';
    return 0;
}
