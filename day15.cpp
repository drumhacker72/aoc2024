#include <generator>
#include <iostream>
#include <vector>

using namespace std;

struct Point
{
    size_t x;
    size_t y;

    bool operator==(Point p) const { return x == p.x && y == p.y; }
};

struct Grid
{
    vector<string> g;

    template <typename Self>
    auto&& operator[](this Self&& self, Point p) { return self.g[p.y][p.x]; }

    /*generator<Point> points() const
    {
        for (int y = 0; y < g.size(); ++y)
        {
            for (int x = 0; x < g[y].size(); ++x)
                co_yield {x, y};
        }
    }*/
};

Point try_move(Grid& grid, Point src, char movement)
{
    if (grid[src] == '.')
        throw logic_error{"cannot move nothing"};
    if (grid[src] == '#')
        return src;
    Point target;
    switch (movement)
    {
    case '^':
        target = {src.x, src.y - 1};
        break;
    case 'v':
        target = {src.x, src.y + 1};
        break;
    case '<':
        target = {src.x - 1, src.y};
        break;
    case '>':
        target = {src.x + 1, src.y};
        break;
    default:
        throw runtime_error{"bad movement"};
    }

    if (grid[target] != '.')
        try_move(grid, target, movement);

    if (grid[target] == '.')
    {
        grid[target] = grid[src];
        grid[src] = '.';
        return target;
    }
    else
        return src;
}

Point try_move2(Grid& grid, Point src, char movement)
{
    if (grid[src] == '.' )
        throw logic_error{"cannot move nothing"};
    if (grid[src] == '#')
        return src;
    Point target;
    switch (movement)
    {
    case '^':
        target = {src.x, src.y - 1};
        break;
    case 'v':
        target = {src.x, src.y + 1};
        break;
    case '<':
        target = {src.x - 1, src.y};
        break;
    case '>':
        target = {src.x + 1, src.y};
        break;
    default:
        throw runtime_error{"bad movement"};
    }
    if ((grid[src] == '[' || grid[src] == ']') && (movement == '^' || movement == 'v'))
    {
        Point src2 = grid[src] == '[' ? Point{src.x+1, src.y} : Point{src.x-1, src.y};
        Point target2 = grid[src] == '[' ? Point{target.x+1, target.y} : Point{target.x-1, target.y};
        Grid tmp = grid;
        bool good = true;
        if (tmp[target] != '.' && try_move2(tmp, target, movement) == target)
            good = false;
        else if (tmp[target2] != '.' && try_move2(tmp, target2, movement) == target2)
            good = false;
        if (good)
        {
            grid = std::move(tmp);
            grid[target2] = grid[src2];
            grid[src2] = '.';
            grid[target] = grid[src];
            grid[src] = '.';
            return target;
        }
        else
            return src;
    }
    else
    {
        if (grid[target] != '.')
            try_move2(grid, target, movement);
        if (grid[target] == '.')
        {
            grid[target] = grid[src];
            grid[src] = '.';
            return target;
        }
        else
            return src;
    }
}

uint64_t gps(Grid const& grid)
{
    uint64_t r = 0;
    for (size_t y = 0; y < grid.g.size(); ++y)
    {
        for (size_t x = 0; x < grid.g[y].size(); ++x)
        {
            if (grid[{x, y}] == 'O' || grid[{x, y}] == '[')
                r += 100 * y + x;
        }
    }
    return r;
}

int main()
{
    Grid grid;
    Grid grid2;
    string line;
    Point robot;
    while (getline(cin, line))
    {
        if (line.empty())
            break;
        grid.g.push_back(line);
        string line2;
        for (char c : line)
        {
            switch (c)
            {
            case '#':
                line2 += "##";
                break;
            case 'O':
                line2 += "[]";
                break;
            case '.':
                line2 += "..";
                break;
            case '@':
                line2 += "@.";
                break;
            default:
                throw runtime_error{"bad character"};
            }
        }
        grid2.g.push_back(line2);
        if (auto pos = line.find('@'); pos != string::npos)
            robot = {pos, grid.g.size()-1};
    }
    Point robot2 = {robot.x*2, robot.y};
    char movement;
    while (cin >> movement)
    {
        robot = try_move(grid, robot, movement);
        robot2 = try_move2(grid2, robot2, movement);
    }
    cout << gps(grid) << '\n';
    cout << gps(grid2) << '\n';
    return 0;
}
