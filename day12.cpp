#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Point
{
    int x;
    int y;

    constexpr Point up() const { return {x, y-1}; }
    constexpr Point down() const { return {x, y+1}; }
    constexpr Point left() const { return {x-1, y}; }
    constexpr Point right() const { return {x+1, y}; }
    constexpr bool operator<(Point p) const
    {
        return x < p.x || x == p.x && y < p.y;
    }
};

struct Grid
{
    vector<string> plots;

    int width() const { return plots[0].size(); }
    int height() const { return plots.size(); }
    char operator[](Point p) const
    {
        if (!is_valid(p)) return ' ';
        else return plots[p.y][p.x];
    }
    bool is_valid(Point p) const
    {
        return p.x >= 0 && p.x < width() && p.y >= 0 && p.y < height();
    }
};

struct Region
{
    char plant;
    int area;
    int perimeter;
    int corners;

    bool try_merge(Region* r)
    {
        if (this == r)
            return false;
        if (plant != r->plant)
            return false;
        area += r->area;
        perimeter += r->perimeter;
        corners += r->corners;
        *r = {' ', 0, 0, 0};
        return true;
    }
    int price1() const
    {
        return area * perimeter;
    }
    int price2() const
    {
        return area * corners;
    }
};

int main()
{
    Grid grid;
    string line;
    while (getline(cin, line))
        grid.plots.push_back(line);

    vector<Region> regions;
    regions.reserve(grid.width() * grid.height());
    vector<Region*> last_row;
    for (int y = 0; y < grid.height(); ++y)
    {
        vector<Region*> this_row;
        for (int x = 0; x < grid.width(); ++x)
        {
            Point p {x, y};
            int perimeter = 0;
            if (grid[p] != grid[p.up()])
                ++perimeter;
            if (grid[p] != grid[p.down()])
                ++perimeter;
            if (grid[p] != grid[p.left()])
                ++perimeter;
            if (grid[p] != grid[p.right()])
                ++perimeter;
            int corners = 0;
            if (grid[p] != grid[p.up()] && grid[p] != grid[p.left()])
                ++corners;
            if (grid[p] != grid[p.up()] && grid[p] != grid[p.right()])
                ++corners;
            if (grid[p] != grid[p.down()] && grid[p] != grid[p.left()])
                ++corners;
            if (grid[p] != grid[p.down()] && grid[p] != grid[p.right()])
                ++corners;
            if (grid[p] == grid[p.up()] && grid[p] == grid[p.left()] && grid[p] != grid[p.up().left()])
                ++corners;
            if (grid[p] == grid[p.up()] && grid[p] == grid[p.right()] && grid[p] != grid[p.up().right()])
                ++corners;
            if (grid[p] == grid[p.down()] && grid[p] == grid[p.left()] && grid[p] != grid[p.down().left()])
                ++corners;
            if (grid[p] == grid[p.down()] && grid[p] == grid[p.right()] && grid[p] != grid[p.down().right()])
                ++corners;
            regions.push_back({grid[p], 1, perimeter, corners});
            Region* region = &regions.back();
            if (x > 0 && this_row.back()->try_merge(region))
                region = this_row.back();
            if (y > 0 && last_row[x]->try_merge(region))
            {
                ranges::replace(this_row, region, last_row[x]);
                region = last_row[x];
            }
            this_row.push_back(region);
        }
        last_row = this_row;
    }

    int total_price1 = 0;
    int total_price2 = 0;
    for (auto const& r : regions)
    {
        total_price1 += r.price1();
        total_price2 += r.price2();
    }
    cout << total_price1 << '\n';
    cout << total_price2 << '\n';

    return 0;
}
