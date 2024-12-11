#include <iostream>
#include <optional>
#include <vector>

using namespace std;

using Grid = vector<string>;

optional<char> letter(Grid const& g, int x, int y)
{
    if (y < 0 || y >= g.size() || x < 0 || x >= g[y].size())
        return nullopt;
    return {g[y][x]};
}

bool has_xmas(Grid const& g, size_t x, size_t y, int dx, int dy)
{
    return letter(g, x, y) == 'X'
        && letter(g, x+dx, y+dy) == 'M'
        && letter(g, x+2*dx, y+2*dy) == 'A'
        && letter(g, x+3*dx, y+3*dy) == 'S';
}

int xmases_from(Grid const& g, size_t x, size_t y)
{
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (has_xmas(g, x, y, dx, dy))
                ++count;
        }
    }
    return count;
}

optional<bool> m_or_s(char c)
{
    if (c == 'M') return true;
    if (c == 'S') return false;
    return nullopt;
}

bool has_crossmas(Grid const& g, size_t x, size_t y)
{
    if (letter(g, x, y) != 'A')
        return false;
    auto nw = letter(g, x-1, y-1).and_then(m_or_s);
    auto ne = letter(g, x+1, y-1).and_then(m_or_s);
    auto sw = letter(g, x-1, y+1).and_then(m_or_s);
    auto se = letter(g, x+1, y+1).and_then(m_or_s);
    if (!nw || !ne || !sw || !se)
        return false;
    return (*nw != *se) && (*ne != *sw);
}

int main()
{
    vector<string> grid;
    string line;
    while (getline(cin, line))
        grid.push_back(line);
    int xmas_count = 0;
    int crossmas_count = 0;
    for (size_t y = 0; y < grid.size(); ++y)
    {
        for (size_t x = 0; x < grid[y].size(); ++x)
        {
            xmas_count += xmases_from(grid, x, y);
            if (has_crossmas(grid, x, y))
                ++crossmas_count;
        }
    }
    cout << xmas_count << '\n';
    cout << crossmas_count << '\n';
    return 0;
}
