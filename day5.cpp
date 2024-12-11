#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

map<int, vector<int>> deps;

bool in_order(vector<int> const& update)
{
    set<int> seen;
    for (auto page : update)
    {
        for (auto dep : deps[page])
        {
            if (ranges::contains(update, dep) && !seen.contains(dep))
                return false;
        }
        seen.insert(page);
    }
    return true;
}

vector<int> make_order(set<int> pages)
{
    vector<int> update;
    while (!pages.empty())
    {
        for (auto page : pages)
        {
            bool okay = true;
            for (auto dep : deps[page])
            {
                if (pages.contains(dep))
                {
                    okay = false;
                    break;
                }
            }
            if (okay)
            {
                pages.erase(page);
                update.push_back(page);
                break;
            }
        }
    }
    return update;
}

int main()
{
    vector<vector<int>> updates;

    string line;
    while (getline(cin, line))
    {
        if (line.empty())
            break;
        int x, y;
        sscanf(line.c_str(), "%d|%d", &x, &y);
        deps[y].push_back(x);
    }
    while (getline(cin, line))
    {
        vector<int> update;
        for (auto page : views::split(line, ","sv))
            update.push_back(stoi(string{string_view{page}}));
        updates.push_back(update);
    }

    int corrects = 0;
    int incorrects = 0;
    for (auto const& update : updates)
    {
        if (in_order(update))
            corrects += update[update.size()/2];
        else
        {
            auto fixed = make_order(set(update.begin(), update.end()));
            incorrects += fixed[fixed.size()/2];
        }
    }
    cout << corrects << '\n';
    cout << incorrects << '\n';
    return 0;
}
