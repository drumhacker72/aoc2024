#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <ranges>
#include <utility>
#include <vector>

using namespace std;

uint next_secret(uint secret)
{
    constexpr uint prune = 16777216;
    secret ^= secret * 64;
    secret %= prune;
    secret ^= secret / 32;
    secret %= prune;
    secret ^= secret * 2048;
    secret %= prune;
    return secret;
}

int main()
{
    vector<int> secrets;
    int secret;
    while (cin >> secret)
        secrets.push_back(secret);
    int n = secrets.size();
    vector<deque<int>> changes (n);
    map<deque<int>, pair<int, vector<bool>>> bananas;
    for (int t = 0; t < 2000; ++t)
    {
        for (int m = 0; m < n; ++m)
        {
            auto last_secret = secrets[m];
            auto secret = next_secret(last_secret);
            secrets[m] = secret;
            int last_price = last_secret % 10;
            int price = secret % 10;
            int change = price - last_price;
            changes[m].push_back(change);
            if (changes[m].size() > 4)
                changes[m].pop_front();
            auto& [b, ms] = bananas[changes[m]];
            ms.resize(n);
            if (!ms[m])
            {
                b += price;
                ms[m] = true;
            }
        }
    }
    cout << ranges::fold_left(secrets, 0ULL, plus{}) << '\n';
    int best_bananas = 0;
    for (int a = -9; a <= 9; ++a)
    {
        for (int b = -9; b <= 9; ++b)
        {
            for (int c = -9; c <= 9; ++c)
            {
                for (int d = -9; d <= 9; ++d)
                {
                    auto [count, ms] = bananas[deque{a, b, c, d}];
                    if (count > best_bananas)
                        best_bananas = count;
                }
            }
        }
    }
    cout << best_bananas << '\n';
    return 0;
}
