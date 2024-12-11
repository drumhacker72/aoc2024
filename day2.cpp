#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

using namespace std;

using Report = vector<int>;

bool is_safe(const Report& r)
{
    vector<int> ds;
    for (int i = 0; i < r.size() - 1; ++i)
        ds.push_back(r[i+1] - r[i]);
    bool inc = all_of(ds.begin(), ds.end(), [](int d) { return d > 0; });
    bool dec = all_of(ds.begin(), ds.end(), [](int d) { return d < 0; });
    bool close = all_of(ds.begin(), ds.end(), [](int d) { return 1 <= abs(d) && abs(d) <= 3; });
    return (inc || dec) && close;
}

int main()
{
    vector<Report> rs;
    string line;
    while (getline(cin, line))
    {
        std::istringstream ss {line};
        Report r;
        int level;
        while (ss >> level)
            r.push_back(level);
        rs.push_back(r);
    }
    int num_safe = 0;
    for (auto&& r : rs)
    {
        if (is_safe(r))
            ++num_safe;
    }
    cout << num_safe << '\n';
    int close_to_safe = 0;
    for (auto&& r : rs)
    {
        bool s = false;
        if (is_safe(r))
            s = true;
        else
        {
            for (int i = 0; i < r.size(); ++i)
            {
                if (is_safe(r | views::enumerate
                              | views::filter([i](auto&& e) { return i != get<0>(e); })
                              | views::values
                              | ranges::to<Report>()))
                {
                    s = true;
                    break;
                }
            }
        }
        if (s)
            ++close_to_safe;
    }
    cout << close_to_safe << '\n';
	return 0;
}
