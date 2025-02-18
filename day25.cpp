#include <iostream>
#include <vector>

using namespace std;

constexpr size_t columns = 5;
constexpr size_t rows = 7;

using Heights = array<int, columns>;

bool overlap(Heights const& a, Heights const& b)
{
    for (int i = 0; i < columns; ++i)
    {
        if (a[i] + b[i] > rows)
            return true;
    }
    return false;
}

int main()
{
    vector<Heights> locks, keys;
    for (;;)
    {
        bool is_lock;
        Heights h {};
        for (int i = 0; i < rows; ++i)
        {
            string line;
            getline(cin, line);
            if (!i)
                is_lock = line[0] == '#';
            for (int j = 0; j < columns; ++j)
            {
                if (line[j] == '#')
                    ++h[j];
            }
        }
        if (is_lock)
            locks.push_back(h);
        else
            keys.push_back(h);
        string blank;
        if (!getline(cin, blank))
            break;
        if (!blank.empty())
            throw runtime_error{"bad input"};
    }
    int fits = 0;
    for (auto const& lock : locks)
    {
        for (auto const& key : keys)
        {
            if (!overlap(lock, key))
                ++fits;
        }
    }
    cout << fits << '\n';
    return 0;
}
