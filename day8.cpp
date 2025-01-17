#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

struct Point
{
    int x;
    int y;

    bool operator<(Point p) const
    {
        return x < p.x || x == p.x && y < p.y;
    }
};

struct Grid
{
    map<char, vector<Point>> antennas;
    int width;
    int height;

    bool valid(Point p) const
    {
        return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
    }
};

istream& operator>>(istream& is, Grid& grid)
{
    string line;
    getline(is, line);
    grid.width = line.size();
    int y = 0;
    do
    {
        for (int x = 0; x < grid.width; ++x)
        {
            char freq = line[x];
            if (freq != '.')
                grid.antennas[freq].push_back({x, y});
        }
        ++y;
    } while (getline(is, line));
    grid.height = y;
    return is;
}

int part1(Grid const& grid)
{
    set<Point> antinodes;
    for (auto& [freq, ps] : grid.antennas)
    {
        for (int i = 0; i < ps.size(); ++i)
        {
            for (int j = i+1; j < ps.size(); ++j)
            {
                int dx = ps[j].x - ps[i].x;
                int dy = ps[j].y - ps[i].y;
                Point a1 {ps[j].x + dx, ps[j].y + dy};
                Point a2 {ps[i].x - dx, ps[i].y - dy};
                if (grid.valid(a1))
                    antinodes.insert(a1);
                if (grid.valid(a2))
                    antinodes.insert(a2);
            }
        }
    }
    return antinodes.size();
}

int part2(Grid const& grid)
{
    set<Point> antinodes;
    for (auto& [freq, ps] : grid.antennas)
    {
        for (int i = 0; i < ps.size(); ++i)
        {
            for (int j = i+1; j < ps.size(); ++j)
            {
                int dx = ps[j].x - ps[i].x;
                int dy = ps[j].y - ps[i].y;
                int d = gcd(dx, dy);
                dx /= d;
                dy /= d;
                Point a = ps[i];
                while (grid.valid(a))
                {
                    antinodes.insert(a);
                    a = {a.x + dx, a.y + dy};
                }
                a = ps[i];
                while (grid.valid(a))
                {
                    antinodes.insert(a);
                    a = {a.x - dx, a.y - dy};
                }
            }
        }
    }
    return antinodes.size();
}

int main()
{
    Grid grid;
    cin >> grid;
    cout << part1(grid) << '\n';
    cout << part2(grid) << '\n';
    return 0;
}
