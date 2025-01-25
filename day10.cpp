#include <iostream>
#include <set>
#include <vector>

using namespace std;

template <class T>
void insert_range(T& a, T const& b)
{
    a.insert(b.begin(), b.end());
}

int main()
{
    vector<string> grid;
    string row;
    while (getline(cin, row))
        grid.push_back(row);
    int const w = grid[0].size();
    int const h = grid.size();

    vector<vector<set<pair<int, int>>>> reachable (h, vector<set<pair<int, int>>>(w));
    int trailhead_scores = 0;
    for (char alt = '9'; alt >= '0'; --alt)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                if (grid[y][x] != alt)
                    continue;
                auto& r = reachable[y][x];
                if (alt == '9')
                {
                    r.insert({x, y});
                    continue;
                }
                if (y > 0 && grid[y-1][x] == alt+1)
                    insert_range(r, reachable[y-1][x]);
                if (y < h-1 && grid[y+1][x] == alt+1)
                    insert_range(r, reachable[y+1][x]);
                if (x > 0 && grid[y][x-1] == alt+1)
                    insert_range(r, reachable[y][x-1]);
                if (x < w-1 && grid[y][x+1] == alt+1)
                    insert_range(r, reachable[y][x+1]);
                if (alt == '0')
                    trailhead_scores += r.size();
            }
        }
    }
    cout << trailhead_scores << '\n';

    vector<vector<int>> ratings (h, vector<int>(w, 1));
    int trailhead_ratings = 0;
    for (char alt = '8'; alt >= '0'; --alt)
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                if (grid[y][x] != alt)
                    continue;
                int rating = 0;
                if (y > 0 && grid[y-1][x] == alt+1)
                    rating += ratings[y-1][x];
                if (y < h-1 && grid[y+1][x] == alt+1)
                    rating += ratings[y+1][x];
                if (x > 0 && grid[y][x-1] == alt+1)
                    rating += ratings[y][x-1];
                if (x < w-1 && grid[y][x+1] == alt+1)
                    rating += ratings[y][x+1];
                ratings[y][x] = rating;
                if (alt == '0')
                    trailhead_ratings += rating;
            }
        }
    }
    cout << trailhead_ratings << '\n';

    return 0;
}
